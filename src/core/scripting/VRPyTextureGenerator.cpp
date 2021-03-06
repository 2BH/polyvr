#include "VRPyTextureGenerator.h"
#include "VRPyGeometry.h"
#include "VRPyDevice.h"
#include "VRPyBaseT.h"
#include "VRPyImage.h"
#include "VRPyPath.h"

#include "core/math/path.h"

using namespace OSG;

simpleVRPyType(TextureGenerator, New_ptr);

PyMethodDef VRPyTextureGenerator::methods[] = {
    {"add", (PyCFunction)VRPyTextureGenerator::add, METH_VARARGS, "Add a layer - add(str type, float amount, [r,g,b], [r,g,b])\n\ttype can be: 'Perlin', 'Bricks'"
                                                                    "\n\t add(str type, float amount, [r,g,b,a], [r,g,b,a])" },
    {"drawFill", (PyCFunction)VRPyTextureGenerator::drawFill, METH_VARARGS, "Fill whole texture - drawFill([r,g,b,a])" },
    {"drawPixel", (PyCFunction)VRPyTextureGenerator::drawPixel, METH_VARARGS, "Set a pixel color - drawPixel([x,y,z], [r,g,b,a])" },
    {"drawLine", (PyCFunction)VRPyTextureGenerator::drawLine, METH_VARARGS, "Add a line, coordinates go from 0 to 1 - drawLine([x1,y1,z1], [x2,y2,z2], [r,g,b,a], flt width)" },
    {"drawPath", (PyCFunction)VRPyTextureGenerator::drawPath, METH_VARARGS, "Add a path, use normalized coordinates from 0 to 1 - drawPath(path, [r,g,b,a], flt width)" },
    {"setSize", (PyCFunction)VRPyTextureGenerator::setSize, METH_VARARGS, "Set the size - setSize([width, height, depth] | bool hasAlphaChannel)\n   set depth to 1 for 2D textures" },
    {"getSize", (PyCFunction)VRPyTextureGenerator::getSize, METH_NOARGS, "Get the size - [width, height, depth] getSize()" },
    {"compose", (PyCFunction)VRPyTextureGenerator::compose, METH_VARARGS, "Bake the layers into an image - img compose( int seed )" },
    {"readSharedMemory", (PyCFunction)VRPyTextureGenerator::readSharedMemory, METH_VARARGS, "Read an image from shared memory - img readSharedMemory( string segment, string data )" },
    {"addSimpleNoise", PyWrapOpt(TextureGenerator, addSimpleNoise, "Add simple noise based on front and backcolor, texture dimension, alpha channel, fg, bg, amount", "1", void, Vec3i, bool, Color4f, Color4f, float) },
    {NULL}  /* Sentinel */
};

PyObject* VRPyTextureGenerator::getSize(VRPyTextureGenerator* self) {
    return toPyTuple( self->objPtr->getSize() );
}

PyObject* VRPyTextureGenerator::compose(VRPyTextureGenerator* self, PyObject* args) {
    return VRPyImage::fromSharedPtr( self->objPtr->compose( parseInt(args) ) );
}

PyObject* VRPyTextureGenerator::readSharedMemory(VRPyTextureGenerator* self, PyObject* args) {
	if (!self->valid()) return NULL;
    const char *segment, *data;
    if (! PyArg_ParseTuple(args, "ss", (char*)&segment, (char*)&data)) return NULL;
    if (segment && data) return VRPyImage::fromSharedPtr( self->objPtr->readSharedMemory(segment, data) );
    Py_RETURN_NONE;
}

PyObject* VRPyTextureGenerator::drawFill(VRPyTextureGenerator* self, PyObject* args) {
	if (!self->valid()) return NULL;
    PyObject* c;
    if (! PyArg_ParseTuple(args, "O", &c)) return NULL;
    self->objPtr->drawFill(Vec4f(parseVec4dList(c)));
    Py_RETURN_TRUE;
}

PyObject* VRPyTextureGenerator::drawPixel(VRPyTextureGenerator* self, PyObject* args) {
	if (!self->valid()) return NULL;
    PyObject *p, *c;
    if (! PyArg_ParseTuple(args, "OO", &p, &c)) return NULL;
    self->objPtr->drawPixel(parseVec3iList(p), Vec4f(parseVec4dList(c)));
    Py_RETURN_TRUE;
}

PyObject* VRPyTextureGenerator::drawLine(VRPyTextureGenerator* self, PyObject* args) {
	if (!self->valid()) return NULL;
    PyObject *p1, *p2, *c;
    float w;
    if (! PyArg_ParseTuple(args, "OOOf", &p1, &p2, &c, &w)) return NULL;
    self->objPtr->drawLine(parseVec3dList(p1), parseVec3dList(p2), Vec4f(parseVec4dList(c)), w);
    Py_RETURN_TRUE;
}

PyObject* VRPyTextureGenerator::drawPath(VRPyTextureGenerator* self, PyObject* args) {
	if (!self->valid()) return NULL;
    VRPyPath* p;
    PyObject* c;
    float w;
    if (! PyArg_ParseTuple(args, "OOf", &p, &c, &w)) return NULL;
    self->objPtr->drawPath(p->objPtr, Vec4f(parseVec4dList(c)), w);
    Py_RETURN_TRUE;
}

PyObject* VRPyTextureGenerator::add(VRPyTextureGenerator* self, PyObject* args) {
	if (!self->valid()) return NULL;
    PyObject *type, *c1, *c2;
    float amount;
    if (! PyArg_ParseTuple(args, "OfOO", &type, &amount, &c1, &c2)) return NULL;
    if (pySize(c1) == 3) self->objPtr->add(PyString_AsString(type), amount, Vec3f(parseVec3dList(c1)), Vec3f(parseVec3dList(c2)));
    if (pySize(c1) == 4) self->objPtr->add(PyString_AsString(type), amount, Vec4f(parseVec4dList(c1)), Vec4f(parseVec4dList(c2)));
    Py_RETURN_TRUE;
}

PyObject* VRPyTextureGenerator::setSize(VRPyTextureGenerator* self, PyObject* args) {
	if (!self->valid()) return NULL;
    PyObject* s; int b = 0;
    if (! PyArg_ParseTuple(args, "O|i", &s, &b)) return NULL;
    self->objPtr->setSize( parseVec3iList(s), b );
    Py_RETURN_TRUE;
}
