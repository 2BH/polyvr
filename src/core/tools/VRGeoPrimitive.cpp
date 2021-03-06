#include "VRGeoPrimitive.h"
#include "core/objects/geometry/VRPrimitive.h"
#include "core/objects/geometry/VRHandle.h"
#include "core/objects/material/VRMaterial.h"
#include "core/utils/toString.h"
#include "core/utils/VRFunction.h"
#include "core/tools/selection/VRSelector.h"
#include "VRAnnotationEngine.h"

#include <boost/bind.hpp>

using namespace OSG;

VRGeoPrimitive::VRGeoPrimitive(string name) : VRGeometry(name) {
    type = "GeoPrimitive";
    selector = VRSelector::create();

    params_geo = VRAnnotationEngine::create();
    params_geo->getMaterial()->setDepthTest(GL_ALWAYS);
    params_geo->getMaterial()->setLit(0);
    params_geo->setSize(0.015);
    params_geo->setBillboard(1);
    params_geo->setScreensize(1);
    params_geo->setColor(Color4f(0,0,0,1));
    params_geo->setBackground(Color4f(1,1,1,1));
    params_geo->setPersistency(0);
}

VRGeoPrimitivePtr VRGeoPrimitive::create(string name) {
    auto p = VRGeoPrimitivePtr( new VRGeoPrimitive(name) );
    p->addChild(p->params_geo);
    p->setPrimitive("Box");
    return p;
}

VRGeoPrimitivePtr VRGeoPrimitive::ptr() { return static_pointer_cast<VRGeoPrimitive>( shared_from_this() ); }

VRAnnotationEnginePtr VRGeoPrimitive::getLabels() { return params_geo; }

VRHandlePtr VRGeoPrimitive::getHandle(int i) {
    if (i < 0 || i >= int(handles.size())) return 0;
    return handles[i];
}

void VRGeoPrimitive::select(bool b) {
    if (selected == b) return;
    selected = b;
    params_geo->setVisible(b);
    for (auto h : handles) h->setVisible(b);

    if (!selector) return;
    if (b) selector->select(ptr(), false); // TODO: does not play nice with params_geo!!
    else selector->clear();
}

void VRGeoPrimitive::update(int i, float v) {
    if (!primitive) return;
    auto params = splitString(primitive->toString(), ' ');
    string args;
    for (uint j=0; j<params.size(); j++) {
        if (i != int(j)) args += params[j];
        else args += toString(v);
        if (j < params.size()-1) args += " ";
    }
    VRGeometry::setPrimitive(primitive->getType(), args);

    auto h = getHandle(i);
    if (!params_geo || !h) return;
    auto a = h->getAxis();
    auto o = h->getOrigin()->pos();
    string lbl = h->getBaseName() + " " + toString( v*1000, 4 ) + " mm";
    params_geo->set(i, (a*v*0.5 + o)*0.5, lbl);
}

void VRGeoPrimitive::setupHandles() {
    for (auto h : handles) subChild(h);
    handles.clear();

    string type = primitive->getType();
    int N = primitive->getNParams();
    auto params = splitString(primitive->toString(), ' ');
    auto param_names = VRPrimitive::getTypeParameter(type);

    for (int i=0; i<N; i++) {
        string n = param_names[i];
        string param = params[i];

        if (n == "Segments x") continue;
        if (n == "Segments y") continue;
        if (n == "Segments z") continue;
        if (n == "Iterations") continue;
        if (n == "Do bottom") continue;
        if (n == "Do top") continue;
        if (n == "Do sides") continue;
        if (n == "Sides") continue;
        if (n == "Segments") continue;
        if (n == "Rings") continue;
        if (n == "Number of teeth") continue;

        auto h = VRHandle::create(n);
        h->setPersistency(0);
        handles.push_back(h);
        addChild(h);

        auto cb = VRFunction<float>::create( "geo_prim_update", boost::bind(&VRGeoPrimitive::update, this, i, _1) );

        if (n == "Scale") h->configure(cb, VRHandle::LINEAR, Vec3d(1,0,0), 1, true);
        if (n == "Size x") h->configure(cb, VRHandle::LINEAR, Vec3d(1,0,0), 0.5, true);
        if (n == "Size y") h->configure(cb, VRHandle::LINEAR, Vec3d(0,1,0), 0.5, true);
        if (n == "Size z") h->configure(cb, VRHandle::LINEAR, Vec3d(0,0,1), 0.5, true);
        if (n == "Radius") h->configure(cb, VRHandle::LINEAR, Vec3d(1,0,0), 1, true);
        if (n == "Height") {
            h->configure(cb, VRHandle::LINEAR, Vec3d(0,1,0), 0.5, true);
            if (type == "Arrow") h->configure(cb, VRHandle::LINEAR, Vec3d(0,0,1), 1, true);
        }
        if (n == "Width") h->configure(cb, VRHandle::LINEAR, Vec3d(1,0,0), 0.5, true);
        if (n == "Trunc") h->configure(cb, VRHandle::LINEAR, Vec3d(1,0,0), 0.5, true);
        if (n == "Hat") h->configure(cb, VRHandle::LINEAR, Vec3d(0,0,1), 1, true);
        if (n == "Inner radius") h->configure(cb, VRHandle::LINEAR, Vec3d(0,1,0), 1, true);
        if (n == "Outer radius") h->configure(cb, VRHandle::LINEAR, Vec3d(1,0,0), 0.5, true);

        if (n == "Hole") h->configure(cb, VRHandle::LINEAR, Vec3d(1,0,0), 1, true);
        if (n == "Pitch") h->configure(cb, VRHandle::LINEAR, Vec3d(0,0,1), 2, true);
        if (n == "Teeth size") h->configure(cb, VRHandle::LINEAR, Vec3d(0,1,0), 1, true);
        if (n == "Bevel") h->configure(cb, VRHandle::LINEAR, Vec3d(1,1,0), 1, true);
        if (n == "Length") h->configure(cb, VRHandle::LINEAR, Vec3d(0,0,1), 1, true);

        float v = toFloat(param);
        h->set( pose::create(), v );
        string lbl = h->getBaseName() + " " + toString( v*1000, 4 ) + " mm";
        auto a = h->getAxis();
        auto o = h->getOrigin()->pos();
        params_geo->set(i, (a*v*0.5 + o)*0.5, lbl);
    }
}

void VRGeoPrimitive::setPrimitive(string prim, string args) {
    VRGeometry::setPrimitive(prim, args);
    setupHandles(); // change primitive type
    select(true);
}

vector<VRHandlePtr> VRGeoPrimitive::getHandles() { return handles; }
