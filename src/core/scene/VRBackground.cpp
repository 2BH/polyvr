#include "VRBackground.h"

#include <OpenSG/OSGBackground.h>
#include <OpenSG/OSGSkyBackground.h>
#include <OpenSG/OSGSimpleTexturedMaterial.h>
#include <OpenSG/OSGTextureBackground.h>
#include <OpenSG/OSGSolidBackground.h>
#include <OpenSG/OSGImage.h>

#include <boost/filesystem.hpp>

#include "core/setup/VRSetup.h"
#include "core/scene/VRScene.h"
#include "core/utils/toString.h"
#include "core/utils/VRStorage_template.h"
#include "core/objects/geometry/VRSky.h"

OSG_BEGIN_NAMESPACE;
using namespace std;


class VRBackgroundBase {
    public:
        BackgroundRecPtr bg;
        SkyBackgroundRecPtr skybg;
        SolidBackgroundRecPtr sbg;
        TextureBackgroundRecPtr tbg;
        VRSkyPtr sky;
        vector<ImageRecPtr> skyImgs;

        int type;
        string path;
        string format;
        Color3f color;

        TextureObjChunkRecPtr createSkyTexture();
        void updateSkyTextures();
        void initSky();

        void updateImgTexture();
        void initImg();
};

TextureObjChunkRecPtr VRBackgroundBase::createSkyTexture() {
    ImageRecPtr image = Image::create();
    skyImgs.push_back(image);
    TextureObjChunkRecPtr chunk = TextureObjChunk::create();

    chunk->setImage(image);
    chunk->setMinFilter( GL_LINEAR_MIPMAP_LINEAR );
    chunk->setMagFilter( GL_LINEAR );
    chunk->setWrapS( GL_CLAMP_TO_EDGE );//GL_CLAMP //GL_REPEAT
    chunk->setWrapT( GL_CLAMP_TO_EDGE );//GL_CLAMP_TO_EDGE //GL_REPEAT
    //chunk->setEnvMode( GL_REPLACE );

    return chunk;
}

string normPath(string p) {
    if (boost::filesystem::exists(p)) return boost::filesystem::canonical(p).string();
    return p;
}

void VRBackgroundBase::updateSkyTextures() {
    string tmp;

    tmp = path + "_back" + format;
    skyImgs[0]->read(normPath(tmp).c_str());
    tmp = path + "_front" + format;
    skyImgs[1]->read(normPath(tmp).c_str());
    tmp = path + "_left" + format;
    skyImgs[2]->read(normPath(tmp).c_str());
    tmp = path + "_right" + format;
    skyImgs[3]->read(normPath(tmp).c_str());
    tmp = path + "_down" + format;
    skyImgs[4]->read(normPath(tmp).c_str());
    tmp = path + "_up" + format;
    skyImgs[5]->read(normPath(tmp).c_str());
}

void VRBackgroundBase::initSky() {
    skybg = SkyBackground::create();
    skybg->setBackTexture( createSkyTexture() );
    skybg->setFrontTexture( createSkyTexture() );
    skybg->setLeftTexture( createSkyTexture() );
    skybg->setRightTexture( createSkyTexture() );
    skybg->setBottomTexture( createSkyTexture() );
    skybg->setTopTexture( createSkyTexture() );
}

void VRBackgroundBase::updateImgTexture() {
    skyImgs[6]->read(normPath(path).c_str());
}

void VRBackgroundBase::initImg() {
    tbg = TextureBackground::create();
    tbg->setTexture( createSkyTexture() );
}


VRBackground::VRBackground () {
    base = shared_ptr<VRBackgroundBase>( new VRBackgroundBase() );

    base->sbg = SolidBackground::create();
    base->initSky();
    base->initImg();

    base->color = Color3f(0.6, 0.6, 0.6);
    setBackground(SOLID);

    base->format = ".png";

    setStorageType("Background");
    store("type", &base->type);
    store("color", &base->color);
    store("path", &base->path);
    store("format", &base->format);
}

VRBackground::~VRBackground() {}

VRSkyPtr VRBackground::getSky() { return base->sky; }

void VRBackground::setBackground(TYPE t) {
    base->type = t;
    if (base->sky) base->sky->hide();
    switch(t) {
        case SOLID:
            base->bg = base->sbg;
            base->sbg->setColor(base->color);
            break;
        case IMAGE:
            base->bg = base->tbg;
            base->updateImgTexture();
            break;
        case SKYBOX:
            base->bg = base->skybg;
            base->updateSkyTextures();
            break;
        case SKY:
            base->bg = base->sbg; // set solid bg
            base->sbg->setColor(base->color);
            if (!base->sky) {
                base->sky = VRSky::create();
                auto scene = VRScene::getCurrent();
                scene->getRoot()->addLink( base->sky );
            } else base->sky->reloadShader();
            base->sky->show();
            break;
    }

    updateBackground();
}

void VRBackground::setBackgroundColor(Color3f c) {
    base->color = c;
    base->sbg->setColor(c);
    setBackground(TYPE(base->type));
}

void VRBackground::setBackgroundPath(string s) {
    base->path = s;
    if (base->type == IMAGE) base->updateImgTexture();
    if (base->type == SKYBOX) base->updateSkyTextures();
    setBackground(TYPE(base->type));
}

BackgroundRecPtr VRBackground::getBackground() { return base->bg; }
void VRBackground::setSkyBGExtension(string f) { base->format = f; base->updateSkyTextures(); }
string VRBackground::getSkyBGExtension() { return base->format; }
VRBackground::TYPE VRBackground::getBackgroundType() { return TYPE(base->type); }
Color3f VRBackground::getBackgroundColor() { return base->color; }
string VRBackground::getBackgroundPath() { return base->path; }

void VRBackground::updateBackground() {
    auto setup = VRSetup::getCurrent();
    if (setup) setup->setViewBackground(getBackground());
}

void VRBackground::update() { setBackground(TYPE(base->type)); }


OSG_END_NAMESPACE;
