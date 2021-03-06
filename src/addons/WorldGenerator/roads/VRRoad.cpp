#include "VRRoad.h"
#include "../terrain/VRTerrain.h"
#include "core/utils/toString.h"
#include "core/math/path.h"
#include "core/objects/geometry/VRStroke.h"
#include "addons/Semantics/Reasoning/VROntology.h"
#include "addons/Semantics/Reasoning/VRProperty.h"
#include "addons/Semantics/Reasoning/VREntity.h"

using namespace OSG;

VRRoad::VRRoad() : VRRoadBase("Road") {}
VRRoad::~VRRoad() {}

VRRoadPtr VRRoad::create() { return VRRoadPtr( new VRRoad() ); }

float VRRoad::getWidth() {
    float width = 0;
    for (auto lane : entity->getAllEntities("lanes")) width += toFloat( lane->get("width")->value );
    return width;
}

VREntityPtr VRRoad::getNodeEntry( VREntityPtr node ) {
    /*string rN = entity->getName();
    string nN = node->getName();
    auto nodeEntry = entity->ontology.lock()->process("q(e):NodeEntry(e);Node("+nN+");Road("+rN+");has("+rN+".path,e);has("+nN+",e)");
    return nodeEntry[0];*/

    for (auto rp : entity->getAllEntities("path")) {
        for (auto rnE : rp->getAllEntities("nodes")) {
            for (auto nE : node->getAllEntities("paths")) {
                if (rnE == nE) return nE;
            }
        }
    }
    return 0;
}

bool VRRoad::hasMarkings() {
    string type = "road";
    if (auto t = getEntity()->get("type")) type = t->value;
    return (type != "unclassified" && type != "service" && type != "footway");
}

posePtr VRRoad::getRightEdge(Vec3d pos) {
    auto path = toPath(getEntity()->getEntity("path"), 16);
    float t = path->getClosestPoint(pos); // get nearest road path position to pos
    auto pose = path->getPose(t);
    Vec3d p = pose->pos() + pose->x()*(getWidth()*0.5 + offset);
    pose->setPos(p);
    return pose;
}

VRRoad::edgePoint& VRRoad::getEdgePoints( VREntityPtr node ) {
    if (edgePoints.count(node) == 0) {
        float width = getWidth();
        VREntityPtr rEntry = getNodeEntry( node );
        Vec3d norm = rEntry->getVec3("direction") * toInt(rEntry->get("sign")->value);
        Vec3d x = Vec3d(0,1,0).cross(norm);
        x.normalize();
        Vec3d pNode = node->getVec3("position");
        Vec3d p1 = pNode - x * 0.5 * width; // right
        Vec3d p2 = pNode + x * 0.5 * width; // left
        edgePoints[node] = edgePoint(p1,p2,norm,rEntry);
    }
    return edgePoints[node];
}

VRGeometryPtr VRRoad::createGeometry() {
    auto strokeGeometry = [&]() {
	    float width = getWidth();
		float W = width*0.5;
		vector<Vec3d> profile;
		profile.push_back(Vec3d(-W+offset,0,0));
		profile.push_back(Vec3d(W+offset,0,0));

		auto geo = VRStroke::create("road");
		vector<pathPtr> paths;
		for (auto p : entity->getAllEntities("path")) {
            paths.push_back( toPath(p,32) );
		}
		geo->setPaths( paths );
		geo->strokeProfile(profile, 0, 0);
		if (terrain) terrain->elevateVertices(geo, roadTerrainOffset);
		return geo;
	};

	auto geo = strokeGeometry();
	setupTexCoords( geo, entity );
	addChild(geo);
	return geo;
}


void VRRoad::computeMarkings() {
    if (!hasMarkings()) return;
    float mw = markingsWidth;

    // road data
    vector<VREntityPtr> nodes;
    vector<Vec3d> normals;
    VREntityPtr pathEnt = entity->getEntity("path");
    if (!pathEnt) return;

    VREntityPtr nodeEntryIn = pathEnt->getEntity("nodes",0);
    VREntityPtr nodeEntryOut = pathEnt->getEntity("nodes",1);
    VREntityPtr nodeIn = nodeEntryIn->getEntity("node");
    VREntityPtr nodeOut = nodeEntryOut->getEntity("node");
    Vec3d normIn = nodeEntryIn->getVec3("direction");
    Vec3d normOut = nodeEntryOut->getVec3("direction");

    float roadWidth = getWidth();
    auto lanes = entity->getAllEntities("lanes");
    int Nlanes = lanes.size();

    auto add = [&](Vec3d pos, Vec3d n) {
        if (terrain) terrain->elevatePoint(pos, 0.06);
        if (terrain) terrain->projectTangent(n, pos);
        nodes.push_back(addNode(0, pos));
        normals.push_back(n);
    };

    // compute markings nodes
    auto path = toPath(pathEnt, 12);
    for (auto point : path->getPoints()) {
        Vec3d p = point.pos();
        Vec3d n = point.dir();
        Vec3d x = point.x();
        x.normalize();

        float widthSum = -roadWidth*0.5 - offset;
        for (int li=0; li<Nlanes; li++) {
            auto lane = lanes[li];
            float width = toFloat( lane->get("width")->value );
            float k = widthSum;
            if (li == 0) k += mw*0.5;
            add(-x*k + p, n);
            widthSum += width;
        }
        add(-x*(roadWidth*0.5 - mw*0.5 - offset) + p, n);
    }

    // markings
    float Ldash = 2;
    int pathN = path->size();
    float L = path->getLength();
    string Ndots = toString(int(L/Ldash));
    int lastDir = 0;
    for (int li=0; li<Nlanes+1; li++) {
        vector<VREntityPtr> nodes2;
        vector<Vec3d> normals2;
        for (int pi=0; pi<pathN; pi++) {
            int i = pi*(Nlanes+1)+li;
            nodes2.push_back( nodes[i] );
            normals2.push_back( normals[i] );
        }
        auto mL = addPath("RoadMarking", name, nodes2, normals2);
        mL->set("width", toString(mw));
        entity->add("markings", mL->getName());

        if (li != Nlanes) {
            auto lane = lanes[li];
            if (!lane->is_a("Lane")) { lastDir = 0; continue; }
            int direction = toInt( lane->get("direction")->value );
            if (li != 0 && lastDir*direction > 0) {
                mL->set("style", "dashed");
                mL->set("dashNumber", Ndots);
            }
            lastDir = direction;
        }
    }
}

void VRRoad::addParkingLane( int direction, float width, int capacity, string type ) {
	auto l = ontology->addEntity( entity->getName()+"Lane", "ParkingLane");
	l->set("width", toString(width));
	l->set("direction", toString(direction));
	entity->add("lanes", l->getName());
	l->set("road", entity->getName());
	l->set("capacity", toString(capacity));
}

void VRRoad::setOffset(float o) { offset = o; }




