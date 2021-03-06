#ifndef VRPYWORLDGENERATOR_H_INCLUDED
#define VRPYWORLDGENERATOR_H_INCLUDED

#include "core/scripting/VRPyBase.h"
#include "VRWorldGenerator.h"
#include "roads/VRAsphalt.h"
#include "roads/VRRoadNetwork.h"
#include "roads/VRRoad.h"

struct VRPyWorldGenerator : VRPyBaseT<OSG::VRWorldGenerator> {
    static PyMethodDef methods[];
};

struct VRPyRoadBase : VRPyBaseT<OSG::VRRoadBase> {
    static PyMethodDef methods[];
};

struct VRPyRoad : VRPyBaseT<OSG::VRRoad> {
    static PyMethodDef methods[];
};

struct VRPyAsphalt : VRPyBaseT<OSG::VRAsphalt> {
    static PyMethodDef methods[];
};

struct VRPyRoadNetwork : VRPyBaseT<OSG::VRRoadNetwork> {
    static PyMethodDef methods[];
};

#endif // VRPYWORLDGENERATOR_H_INCLUDED
