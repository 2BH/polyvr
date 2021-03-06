#include "VRPyRobotArm.h"
#include "core/scripting/VRPyBaseT.h"
#include "core/scripting/VRPyTypeCaster.h"
#include "core/scripting/VRPyGeometry.h"
#include "core/scripting/VRPyPath.h"

using namespace OSG;

simpleVRPyType(RobotArm, New_ptr);

PyMethodDef VRPyRobotArm::methods[] = {
    {"setParts", PyWrap(RobotArm, setParts, "Set robot parts", void, vector<VRTransformPtr> ) },
    {"setAngleOffsets", PyWrap(RobotArm, setAngleOffsets, "Set angle offset for each part", void, vector<float> ) },
    {"setAngleDirections", PyWrap(RobotArm, setAngleDirections, "Set angles rotation direction - setAngleDirections([1/-1])", void, vector<int> ) },
    {"setAxis", PyWrap(RobotArm, setAxis, "Set rotation axis for each part - setAxis([int a])\n a: 0 = 'x', 1 = 'y', 2 = 'z'", void, vector<int> ) },
    {"setLengths", PyWrap(RobotArm, setLengths, "Set kinematic lengths between joints - setLengths([base_height, upper_arm length, forearm length, grab position])", void, vector<float> ) },
    {"moveTo", PyWrap(RobotArm, moveTo, "Move the end effector to a certain position - moveTo([x,y,z])", void, posePtr ) },
    {"setGrab", PyWrap(RobotArm, setGrab, "Set grab state - setGrab(float d)\n d: 0 is closed, 1 is open", void, float ) },
    {"toggleGrab", PyWrap(RobotArm, toggleGrab, "Toggle the grab - toggleGrab()", void ) },
    {"setAngles", PyWrap(RobotArm, setAngles, "Set joint angles - setAngles( angles )", void, vector<float> ) },
    {"getAngles", PyWrap(RobotArm, getAngles, "Get joint angles - getAngles()", vector<float> ) },
    //{"getForwardKinematics", PyWrap(RobotArm, getForwardKinematics, "Get end effector pose from angles - p,d,u getForwardKinematics( angles )") },
    //{"getBackwardKinematics", PyWrap(RobotArm, getBackwardKinematics, "Get angles from end effector pose - angles getBackwardKinematics( p,d,u )") },
    {"setPath", PyWrap(RobotArm, setPath, "Set robot path - setPath()", void, pathPtr ) },
    {"getPath", PyWrap(RobotArm, getPath, "Get robot path - getPath()", pathPtr ) },
    {"moveOnPath", PyWrapOpt(RobotArm, moveOnPath, "Move robot on internal path - moveOnPath(float t0, float t1, bool loop)", "0", void, float, float, bool) },
    {NULL}  /* Sentinel */
};
