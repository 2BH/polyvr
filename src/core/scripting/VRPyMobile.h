#ifndef VRPYMOBILE_H_INCLUDED
#define VRPYMOBILE_H_INCLUDED

#include "VRPyBase.h"
#include "core/setup/devices/VRServer.h"

struct VRPyMobile : VRPyBaseT<OSG::VRServer> {
    static PyMethodDef methods[];

    static PyObject* answer(VRPyMobile* self, PyObject* args);
};

#endif // VRPYMOBILE_H_INCLUDED
