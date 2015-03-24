#include "VRSprite.h"

#include "core/tools/VRText.h"
#include "core/objects/material/VRMaterial.h"
#include <OpenSG/OSGNameAttachment.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <sstream>


OSG_BEGIN_NAMESPACE;
using namespace std;

VRSprite::VRSprite (string name, bool alpha, float w, float h) : VRGeometry(name) {

    width = w;
    height = h;
    type = "Sprite";

    setMesh(makePlaneGeo(width, height, 1, 1));
    setAt(Vec3f(0,0,-1));

    font = "SANS 20";
    fontColor = Color4f(0,0,0,255);
    label = "";
}

void VRSprite::setLabel (string l, float res) {
    if (l == label) return;
    int h = 30*res;
    int w = l.size()*h*0.55;
    label = l;
    auto labelMat = VRText::get()->getTexture(l, font, w, h, fontColor, Color4f(0,0,0,0) );
    labelMat->setSortKey(1);
    setMaterial(labelMat);
}

void VRSprite::setTexture(string path){
    getMaterial()->setTexture(path);
    setMaterial(getMaterial());
}

void VRSprite::setFont(string f) { font = f; }

void VRSprite::setFontColor(Color4f c) { fontColor = c; }

string VRSprite::getLabel() { return label; }

void VRSprite::setSize(float w, float h) {
    width = w;
    height = h;
    setMesh(makePlaneGeo(width, height, 1, 1));
}

OSG_END_NAMESPACE;
