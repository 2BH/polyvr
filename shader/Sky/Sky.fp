#version 400 compatibility
#define M_PI 3.1415926535897932384626433832795

// gen
in vec3 norm;
in vec4 pos;
in vec2 tcs;
in mat3 miN;
in mat4 miP;
vec3 fragDir;
vec4 color;
uniform vec2 OSGViewportSize;

// sun
uniform vec3 sunPos; //define sun direction
uniform float theta_s;

// clouds
uniform sampler2D tex;
uniform vec2 cloudOffset; //to shift cloud texture
uniform float cloudScale; // 1e-5
uniform float cloudDensity;
uniform float cloudHeight; // 1000.

//sky
uniform vec3 xyY_z;
uniform vec3 A;
uniform vec3 B;
uniform vec3 C;
uniform vec3 D;
uniform vec3 E;

float gamma;
float theta;

vec3 real_fragDir;
vec3 colClouds = vec3(0.9);
vec4 colGround = vec4(0.7, 0.7, 0.65, 1.0);
float rad_earth = 6.371e6;


void computeDirection() {
	real_fragDir = miN * (miP * pos).xyz;
	float tol = 1e-5;
	fragDir = real_fragDir;
	if(fragDir.y<tol) fragDir.y = tol;
}

vec3 xyYRGB(vec3 xyY) {
	float Y = xyY[2];
	float X = xyY[0] * Y / xyY[1];
	float Z = (1. - xyY[0] - xyY[1]) * Y / xyY[1];


	// scale XYZ from https://www.w3.org/Graphics/Color/srgb

	X = 0.0125313*(X - 0.1901);
	Y = 0.0125313*(Y - 0.2);
	Z = 0.0125313*(Z - 0.2178);
/*
	mat4 m_inv = mat4(3.2406255, -0.9689307, 0.0557101, 0., 
			-1.537208,  1.8757561,  -0.02040211, 0.,
			-0.4986286, 0.0415175,  1.0569959, 0., 
			0., 0., 0., 0.);
*/
	mat4 m_inv = mat4(3.06322, -0.96924, 0.06787, 0., 
			-1.39333,  1.87597,  -0.22883, 0.,
			-0.47580, 0.04156,  1.06925, 0., 
			0., 0., 0., 0.);

	vec4 rgb = m_inv * vec4(X, Y, Z, 0.);

	// add non-linearity
	for (int i=0; i<3; ++i) {
		clamp(rgb[i], 0.0, 1.0);
		if (rgb[i] > 0.0031308) {
			rgb[i] = 1.055 * pow(rgb[i], 1.0/2.4) - 0.055;
		} else {
			rgb[i] = 12.92 * rgb[i];
		}
	}

	return vec3(rgb[0], rgb[1], rgb[2]);
}



// returns the x-z coordinates on plane y = h
// at which the ray fragDir intersects
vec2 planeIntersect(float h){
	vec2 pt = vec2(0.);
	// r = h / cos \phi
	pt = - h * fragDir.xz / fragDir.y ;
	return pt;
}

vec2 sphereIntersect(float h) {
	float r = rad_earth;
	float R = rad_earth + h;
	vec2 pt = vec2(0.);

	float b = -2*real_fragDir.y*r;
	float c = r*r-R*R;

	float disc = sqrt(b*b-4*c);
	float lambda = 0;
	lambda = max(b - disc, b + disc)*0.5;

	pt.x = lambda*real_fragDir.x;
	pt.y = lambda*real_fragDir.z;
	return pt;
}

float computeLuminanceOvercast() {
	// overcast luminance model (CIE - from preetham)
	// Y_z(1 + 2 cos \theta) / 3
	return xyY_z.z * (1 + 2 * fragDir.y) / 3;
}

float computeCloudLuminance() {
	float factor = 0.5;
	float offset = 0.4;
	return factor*(offset + (1 - offset)*computeLuminanceOvercast()); 
}


void computeClouds() {
	
	if (fragDir.y > 0 && cloudDensity > 0.0) {
		float density = cloudDensity;
		float scale = cloudScale;
		//density = 0.7;

		vec2 cmap_l = sphereIntersect(cloudHeight);
		vec2 cmap_u = sphereIntersect(1.5*cloudHeight);
		float c_l = texture(tex, scale * cmap_l + cloudOffset).x;
		float c_u = texture(tex, scale * cmap_u + 0.25 *cloudOffset).x;
		
		// compute luminance of clouds based on angle
		float y = computeCloudLuminance();
	
		// write higher layers of cloud first
		//c_u = smoothstep( 0.25*density, 0.5*density + 0.5, 0.5*c_u);
		c_u = smoothstep(0.0, 1.0, (1.0 - 0.5*density)* c_u);
		color = mix(vec4(y * colClouds, 1.0), color, c_u);

		// overlay darker base layer last
		c_l = smoothstep(0.2, 1.0, (1.0 - density)* c_l);
		//c_l = smoothstep(0.5*density, 0.5*density + 0.5, 0.5 * c_l);
		color = mix(vec4(0.8 * y * colClouds, 1.0), color, c_l);
	}
}

void addSun() {
	float sunAngSize = 0.00873; // define sun size in radians
	//sun angular size ~0.5 degrees = 0.00873 rad http://astronomy.swin.edu.au/cosmos/A/Angular+Diameter
	// if angle of fragment to sun is small enough, paint white
	color = mix(vec4(1.0, 1.0, 1.0, 1.0), color, smoothstep( sunAngSize * 0.7, sunAngSize * 1.0, gamma));

	//float moon_gamma = acos(dot(fragDir, - sunPos));
	//color = mix(vec4(0.8, 0.8, 0.8, 1.0), color, smoothstep( sunAngSize * 0.7, sunAngSize * 1.0, moon_gamma));
}

void addGround() {
	float offset = 0.75;
	float factor = 0.3 * (offset + (1 - offset)*xyY_z.z);
	color = mix(factor * colGround, color, smoothstep( -0.05, 0.0, real_fragDir.y) );
}

vec3 f_f0() {
	// f  = (1 + A*exp(B/cos(theta)))
	//      * (1 + C*exp(D*gamma) + E*pow(cos(gamma),2))
	vec3 xyY = vec3(0,0,0);
	for (int i=0; i<3; ++i) {
		float f = (1 + A[i] * exp( B[i] / cos(theta) ) )
		    * (1 + C[i]*exp( D[i] * gamma ) + E[i] * pow( cos( gamma ), 2 ) );

		float f0 = (1 + A[i] * exp( B[i] ) )
		    * (1 + C[i] * exp( D[i] * theta_s ) + E[i] * pow( cos( theta_s ), 2) );
		if (f0 != 0) xyY[i] = f/f0;
	}
	return xyY;
} 

void main() {

	computeDirection();

	// \gamma is angle between viewing direction (fragDir) and sun (sunPos)
	gamma = acos(dot(fragDir, sunPos));
	// \theta is angle of fragDir to zenith
	theta = acos(fragDir.y);

	vec3 xyY = f_f0(); 
	//xyY.x = xyY_z.x;
	//xyY.y = xyY_z.y;
	xyY.x *= xyY_z.x;
	xyY.y *= xyY_z.y;
	xyY.z *= xyY_z.z;

	//vec3 xyY = vec3(0.150017, 0.060007, 7.22);	
	color = vec4(xyYRGB(xyY), 1);

	// add cloud cover
	computeClouds();

	// add sun
	addSun();
	addGround();

	gl_FragColor = color;
	gl_FragDepth = 1.0; // depth is infinite at 1.0? behind all else (check)
}
