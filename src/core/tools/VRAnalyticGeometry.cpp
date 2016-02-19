#include "VRAnalyticGeometry.h"
#include "VRAnnotationEngine.h"

#include "core/objects/geometry/VRGeometry.h"
#include "core/objects/material/VRMaterial.h"

#include <OpenSG/OSGGeometry.h>
#include <OpenSG/OSGGeoProperties.h>

using namespace OSG;

VRAnalyticGeometry::VRAnalyticGeometry() : VRObject("AnalyticGeometry") {
    ae = VRAnnotationEngine::create();
    vectorLinesGeometry = VRGeometry::create("AGLines");
    vectorEndsGeometry = VRGeometry::create("AGPoints");

    // lines
    GeoPnt3fPropertyRecPtr pos = GeoPnt3fProperty::create();
    GeoVec3fPropertyRecPtr cols = GeoVec3fProperty::create();
    GeoUInt32PropertyRecPtr lengths = GeoUInt32Property::create();
    lengths->addValue(0);

    vectorLinesGeometry->setType(GL_LINES);
    vectorLinesGeometry->setPositions(pos);
    vectorLinesGeometry->setColors(cols);
    vectorLinesGeometry->setLengths(lengths);

    auto mat = VRMaterial::create("AnalyticGeometry");
    mat->setLit(false);
    mat->setLineWidth(3);
    mat->setDepthTest(GL_ALWAYS);
    vectorLinesGeometry->setMaterial(mat);

    // ends
    pos = GeoPnt3fProperty::create();
    cols = GeoVec3fProperty::create();
    lengths = GeoUInt32Property::create();
    lengths->addValue(0);

    vectorEndsGeometry->setType(GL_POINTS);
    vectorEndsGeometry->setPositions(pos);
    vectorEndsGeometry->setColors(cols);
    vectorEndsGeometry->setLengths(lengths);

    mat = VRMaterial::create("AnalyticGeometry2");
    mat->setLit(false);
    mat->setPointSize(11);
    mat->setDepthTest(GL_ALWAYS);
    vectorEndsGeometry->setMaterial(mat);
}

VRAnalyticGeometry::~VRAnalyticGeometry() {}

VRAnalyticGeometryPtr VRAnalyticGeometry::ptr() { return static_pointer_cast<VRAnalyticGeometry>( shared_from_this() ); }
VRAnalyticGeometryPtr VRAnalyticGeometry::create()  {
    auto ptr = shared_ptr<VRAnalyticGeometry>(new VRAnalyticGeometry() );
    ptr->init();
    return ptr;
}

void VRAnalyticGeometry::init() {
    addChild(vectorLinesGeometry);
    addChild(vectorEndsGeometry);
    addChild(ae);

    ae->getMaterial()->setDepthTest(GL_ALWAYS);
}

void VRAnalyticGeometry::setLabelParams(float size, bool screen_size, bool billboard) {
    ae->setSize(size);
    ae->setBillboard(billboard);
    ae->setScreensize(screen_size);
}

void VRAnalyticGeometry::resize(int i, int j) {
    auto pos = vectorLinesGeometry->getMesh()->getPositions();
    auto cols = vectorLinesGeometry->getMesh()->getColors();
    auto lengths = vectorLinesGeometry->getMesh()->getLengths();
    while (i >= (int)pos->size()) {
        pos->addValue(Pnt3f());
        cols->addValue(Vec3f());
        lengths->setValue(pos->size(), 0);
    }

    pos = vectorEndsGeometry->getMesh()->getPositions();
    cols = vectorEndsGeometry->getMesh()->getColors();
    lengths = vectorEndsGeometry->getMesh()->getLengths();
    while (j >= (int)pos->size()) {
        pos->addValue(Pnt3f());
        cols->addValue(Vec3f());
        lengths->setValue(pos->size(), 0);
    }
}

void VRAnalyticGeometry::setAngle(int i, Vec3f p, Vec3f v1, Vec3f v2, Vec3f c1, Vec3f c2, string label) {
    ae->set(i, p+v1*0.1+v2*0.1, label);
    resize(2*i+1);

    // line
    auto pos = vectorLinesGeometry->getMesh()->getPositions();
    auto cols = vectorLinesGeometry->getMesh()->getColors();
    pos->setValue(p+v1*0.1, 2*i);
    pos->setValue(p+v2*0.1, 2*i+1);
    cols->setValue(c1, 2*i);
    cols->setValue(c2, 2*i+1);
}

void VRAnalyticGeometry::setVector(int i, Vec3f p, Vec3f vec, Vec3f color, string label) {
    ae->set(i, p+vec*0.5, label);
    resize(2*i+1, i);

    // line
    auto pos = vectorLinesGeometry->getMesh()->getPositions();
    auto cols = vectorLinesGeometry->getMesh()->getColors();
    pos->setValue(p, 2*i);
    pos->setValue(p+vec, 2*i+1);
    cols->setValue(color, 2*i);
    cols->setValue(color, 2*i+1);

    // end
    pos = vectorEndsGeometry->getMesh()->getPositions();
    cols = vectorEndsGeometry->getMesh()->getColors();
    pos->setValue(p+vec, i);
    cols->setValue(color, i);
}

void VRAnalyticGeometry::clear() {;}
