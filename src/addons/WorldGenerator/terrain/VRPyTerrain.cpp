#include "VRPyTerrain.h"
#include "core/scripting/VRPyBaseT.h"
#include "core/scripting/VRPyBaseFactory.h"

using namespace OSG;

template<> PyObject* VRPyTypeCaster::cast<VRTerrainPtr>(VRTerrainPtr e) { return VRPyTerrain::fromSharedPtr(e); }

simpleVRPyType(Terrain, New_VRObjects_ptr);
simpleVRPyType(Planet, New_VRObjects_ptr);

PyMethodDef VRPyTerrain::methods[] = {
    {"setParameters", PyWrapOpt(Terrain, setParameters, "Set the terrain parameters, size, resolution and height scale", "1", void, Vec2d, double, double) },
    {"loadMap", PyWrapOpt(Terrain, loadMap, "Load height map", "3", void, string, int) },
    {"setMap", PyWrapOpt(Terrain, setMap, "Set height map", "3", void, VRTexturePtr, int) },
    {"physicalize", PyWrap(Terrain, physicalize, "Physicalize terrain", void, bool) },
    {"projectOSM", PyWrap(Terrain, projectOSM, "Load an OSM file and project surface types onto terrain, OSM path, N, E", void, string, double, double) },
    {NULL}  /* Sentinel */
};

PyMethodDef VRPyPlanet::methods[] = {
    {"addSector", PyWrap(Planet, addSector, "Add sector to planet", VRTerrainPtr, int, int) },
    {"getSector", PyWrap(Planet, getSector, "Return sector at N E", VRTerrainPtr, double, double) },
    {"getMaterial", PyWrap(Planet, getMaterial, "Get planet material", VRMaterialPtr) },
    {"setParameters", PyWrap(Planet, setParameters, "Set planet parameters: radius", void, double) },
    {"addPin", PyWrap(Planet, addPin, "Add a pin: label, north, east", int, string, double, double) },
    {"remPin", PyWrap(Planet, remPin, "Remove a pin: ID", void, int) },
    {"fromLatLongPosition", PyWrap(Planet, fromLatLongPosition, "Get Position on planet based on lat and long", Vec3d, double, double) },
    {NULL}  /* Sentinel */
};
