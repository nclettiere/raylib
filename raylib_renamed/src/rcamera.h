/*******************************************************************************************
*
*   rcamera - Basic camera system with support for multiple camera modes
*
*   CONFIGURATION:
*       #define RCAMERA_IMPLEMENTATION
*           Generates the implementation of the library into the included file.
*           If not defined, the library is in header only mode and can be included in other headers
*           or source files without problems. But only ONE file should hold the implementation.
*
*       #define RCAMERA_STANDALONE
*           If defined, the library can be used as standalone as a camera system but some
*           functions must be redefined to manage inputs accordingly.
*
*   CONTRIBUTORS:
*       Ramon Santamaria:   Supervision, review, update and maintenance
*       Christoph Wagner:   Complete redesign, using raymath (2022)
*       Marc Palau:         Initial implementation (2014)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2022-2026 Christoph Wagner (@Crydsch) and Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef RCAMERA_H
#define RCAMERA_H

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Function specifiers definition

// Function specifiers in case library is build/used as a shared library (Windows)
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #if defined(__TINYC__)
            #define __declspec(x) __attribute__((x))
        #endif
        #define rl_RLAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define rl_RLAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

#ifndef rl_RLAPI
    #define rl_RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

#if defined(RCAMERA_STANDALONE)
    #define CAMERA_CULL_DISTANCE_NEAR      0.05
    #define CAMERA_CULL_DISTANCE_FAR    4000.0
#else
    #define CAMERA_CULL_DISTANCE_NEAR   RL_CULL_DISTANCE_NEAR
    #define CAMERA_CULL_DISTANCE_FAR    RL_CULL_DISTANCE_FAR
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
// NOTE: Below types are required for standalone usage
//----------------------------------------------------------------------------------
#if defined(RCAMERA_STANDALONE)
    // rl_Vector2, 2 components
    typedef struct rl_Vector2 {
        float x;                // Vector x component
        float y;                // Vector y component
    } rl_Vector2;

    // rl_Vector3, 3 components
    typedef struct rl_Vector3 {
        float x;                // Vector x component
        float y;                // Vector y component
        float z;                // Vector z component
    } rl_Vector3;

    // rl_Matrix, 4x4 components, column major, OpenGL style, right-handed
    typedef struct rl_Matrix {
        float m0, m4, m8, m12;  // rl_Matrix first row (4 components)
        float m1, m5, m9, m13;  // rl_Matrix second row (4 components)
        float m2, m6, m10, m14; // rl_Matrix third row (4 components)
        float m3, m7, m11, m15; // rl_Matrix fourth row (4 components)
    } rl_Matrix;

    // rl_Camera type, defines a camera position/orientation in 3d space
    typedef struct rl_Camera3D {
        rl_Vector3 position;       // rl_Camera position
        rl_Vector3 target;         // rl_Camera target it looks-at
        rl_Vector3 up;             // rl_Camera up vector (rotation over its axis)
        float fovy;             // rl_Camera field-of-view apperture in Y (degrees) in perspective, used as near plane width in orthographic
        int projection;         // rl_Camera projection type: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
    } rl_Camera3D;

    typedef rl_Camera3D rl_Camera;    // rl_Camera type fallback, defaults to rl_Camera3D

    // rl_Camera projection
    typedef enum {
        CAMERA_PERSPECTIVE = 0, // Perspective projection
        CAMERA_ORTHOGRAPHIC     // Orthographic projection
    } rl_CameraProjection;

    // rl_Camera system modes
    typedef enum {
        CAMERA_CUSTOM = 0,      // rl_Camera custom, controlled by user (rl_UpdateCamera() does nothing)
        CAMERA_FREE,            // rl_Camera free mode
        CAMERA_ORBITAL,         // rl_Camera orbital, around target, zoom supported
        CAMERA_FIRST_PERSON,    // rl_Camera first person
        CAMERA_THIRD_PERSON     // rl_Camera third person
    } rl_CameraMode;
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

rl_RLAPI rl_Vector3 GetCameraForward(rl_Camera *camera);
rl_RLAPI rl_Vector3 GetCameraUp(rl_Camera *camera);
rl_RLAPI rl_Vector3 GetCameraRight(rl_Camera *camera);

// rl_Camera movement
rl_RLAPI void CameraMoveForward(rl_Camera *camera, float distance, bool moveInWorldPlane);
rl_RLAPI void CameraMoveUp(rl_Camera *camera, float distance);
rl_RLAPI void CameraMoveRight(rl_Camera *camera, float distance, bool moveInWorldPlane);
rl_RLAPI void CameraMoveToTarget(rl_Camera *camera, float delta);

// rl_Camera rotation
rl_RLAPI void CameraYaw(rl_Camera *camera, float angle, bool rotateAroundTarget);
rl_RLAPI void CameraPitch(rl_Camera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp);
rl_RLAPI void CameraRoll(rl_Camera *camera, float angle);

rl_RLAPI rl_Matrix GetCameraViewMatrix(rl_Camera *camera);
rl_RLAPI rl_Matrix GetCameraProjectionMatrix(rl_Camera *camera, float aspect);

#if defined(__cplusplus)
}
#endif

#endif // RCAMERA_H

/***********************************************************************************
*
*   CAMERA IMPLEMENTATION
*
************************************************************************************/

#if defined(RCAMERA_IMPLEMENTATION)

#include "raymath.h"        // Required for vector maths:
                            // Vector3Add()
                            // Vector3Subtract()
                            // Vector3Scale()
                            // Vector3Normalize()
                            // Vector3Distance()
                            // Vector3CrossProduct()
                            // Vector3RotateByAxisAngle()
                            // Vector3Angle()
                            // Vector3Negate()
                            // MatrixLookAt()
                            // MatrixPerspective()
                            // MatrixOrtho()
                            // MatrixIdentity()

// raylib required functionality:
                            // rl_GetMouseDelta()
                            // rl_GetMouseWheelMove()
                            // rl_IsKeyDown()
                            // rl_IsKeyPressed()
                            // rl_GetFrameTime()
                            
#include <math.h>           // Required for: fabsf()

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#define CAMERA_MOVE_SPEED                           5.4f       // Units per second
#define CAMERA_ROTATION_SPEED                       0.03f
#define CAMERA_PAN_SPEED                            0.2f

// rl_Camera mouse movement sensitivity
#define CAMERA_MOUSE_MOVE_SENSITIVITY               0.003f

// rl_Camera orbital speed in CAMERA_ORBITAL mode
#define CAMERA_ORBITAL_SPEED                        0.5f       // Radians per second

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Internal Functions Declaration
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
// Returns the cameras forward vector (normalized)
rl_Vector3 GetCameraForward(rl_Camera *camera)
{
    return Vector3Normalize(Vector3Subtract(camera->target, camera->position));
}

// Returns the cameras up vector (normalized)
// Note: The up vector might not be perpendicular to the forward vector
rl_Vector3 GetCameraUp(rl_Camera *camera)
{
    return Vector3Normalize(camera->up);
}

// Returns the cameras right vector (normalized)
rl_Vector3 GetCameraRight(rl_Camera *camera)
{
    rl_Vector3 forward = GetCameraForward(camera);
    rl_Vector3 up = GetCameraUp(camera);

    return Vector3Normalize(Vector3CrossProduct(forward, up));
}

// Moves the camera in its forward direction
void CameraMoveForward(rl_Camera *camera, float distance, bool moveInWorldPlane)
{
    rl_Vector3 forward = GetCameraForward(camera);

    if (moveInWorldPlane)
    {
        // Project vector onto world plane (the plane defined by the up vector)
        if (fabsf(camera->up.z) > 0.7071f) forward.z = 0;
        else if (fabsf(camera->up.x) > 0.7071f) forward.x = 0;
        else forward.y = 0;

        forward = Vector3Normalize(forward);
    }

    // Scale by distance
    forward = Vector3Scale(forward, distance);

    // Move position and target
    camera->position = Vector3Add(camera->position, forward);
    camera->target = Vector3Add(camera->target, forward);
}

// Moves the camera in its up direction
void CameraMoveUp(rl_Camera *camera, float distance)
{
    rl_Vector3 up = GetCameraUp(camera);

    // Scale by distance
    up = Vector3Scale(up, distance);

    // Move position and target
    camera->position = Vector3Add(camera->position, up);
    camera->target = Vector3Add(camera->target, up);
}

// Moves the camera target in its current right direction
void CameraMoveRight(rl_Camera *camera, float distance, bool moveInWorldPlane)
{
    rl_Vector3 right = GetCameraRight(camera);

    if (moveInWorldPlane)
    {
        // Project vector onto world plane (the plane defined by the up vector)
        if (fabsf(camera->up.z) > 0.7071f) right.z = 0;
        else if (fabsf(camera->up.x) > 0.7071f) right.x = 0;
        else right.y = 0;

        right = Vector3Normalize(right);
    }

    // Scale by distance
    right = Vector3Scale(right, distance);

    // Move position and target
    camera->position = Vector3Add(camera->position, right);
    camera->target = Vector3Add(camera->target, right);
}

// Moves the camera position closer/farther to/from the camera target
void CameraMoveToTarget(rl_Camera *camera, float delta)
{
    float distance = Vector3Distance(camera->position, camera->target);

    // Apply delta
    distance += delta;

    // Distance must be greater than 0
    if (distance <= 0) distance = 0.001f;

    // Set new distance by moving the position along the forward vector
    rl_Vector3 forward = GetCameraForward(camera);
    camera->position = Vector3Add(camera->target, Vector3Scale(forward, -distance));
}

// Rotates the camera around its up vector
// Yaw is "looking left and right"
// If rotateAroundTarget is false, the camera rotates around its position
// Note: angle must be provided in radians
void CameraYaw(rl_Camera *camera, float angle, bool rotateAroundTarget)
{
    // Rotation axis
    rl_Vector3 up = GetCameraUp(camera);

    // View vector
    rl_Vector3 targetPosition = Vector3Subtract(camera->target, camera->position);

    // Rotate view vector around up axis
    targetPosition = Vector3RotateByAxisAngle(targetPosition, up, angle);

    if (rotateAroundTarget)
    {
        // Move position relative to target
        camera->position = Vector3Subtract(camera->target, targetPosition);
    }
    else // rotate around camera.position
    {
        // Move target relative to position
        camera->target = Vector3Add(camera->position, targetPosition);
    }
}

// Rotates the camera around its right vector, pitch is "looking up and down"
//  - lockView prevents camera overrotation (aka "somersaults")
//  - rotateAroundTarget defines if rotation is around target or around its position
//  - rotateUp rotates the up direction as well (typically only usefull in CAMERA_FREE)
// NOTE: [angle] must be provided in radians
void CameraPitch(rl_Camera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp)
{
    // Up direction
    rl_Vector3 up = GetCameraUp(camera);

    // View vector
    rl_Vector3 targetPosition = Vector3Subtract(camera->target, camera->position);

    if (lockView)
    {
        // In these camera modes we clamp the Pitch angle
        // to allow only viewing straight up or down.

        // Clamp view up
        float maxAngleUp = Vector3Angle(up, targetPosition);
        maxAngleUp -= 0.001f; // avoid numerical errors
        if (angle > maxAngleUp) angle = maxAngleUp;

        // Clamp view down
        float maxAngleDown = Vector3Angle(Vector3Negate(up), targetPosition);
        maxAngleDown *= -1.0f; // downwards angle is negative
        maxAngleDown += 0.001f; // avoid numerical errors
        if (angle < maxAngleDown) angle = maxAngleDown;
    }

    // Rotation axis
    rl_Vector3 right = GetCameraRight(camera);

    // Rotate view vector around right axis
    targetPosition = Vector3RotateByAxisAngle(targetPosition, right, angle);

    if (rotateAroundTarget)
    {
        // Move position relative to target
        camera->position = Vector3Subtract(camera->target, targetPosition);
    }
    else // Rotate around camera.position
    {
        // Move target relative to position
        camera->target = Vector3Add(camera->position, targetPosition);
    }

    if (rotateUp)
    {
        // Rotate up direction around right axis
        camera->up = Vector3RotateByAxisAngle(camera->up, right, angle);
    }
}

// Rotates the camera around its forward vector
// Roll is "turning your head sideways to the left or right"
// Note: angle must be provided in radians
void CameraRoll(rl_Camera *camera, float angle)
{
    // Rotation axis
    rl_Vector3 forward = GetCameraForward(camera);

    // Rotate up direction around forward axis
    camera->up = Vector3RotateByAxisAngle(camera->up, forward, angle);
}

// Returns the camera view matrix
rl_Matrix GetCameraViewMatrix(rl_Camera *camera)
{
    return MatrixLookAt(camera->position, camera->target, camera->up);
}

// Returns the camera projection matrix
rl_Matrix GetCameraProjectionMatrix(rl_Camera *camera, float aspect)
{
    if (camera->projection == CAMERA_PERSPECTIVE)
    {
        return MatrixPerspective(camera->fovy*rl_DEG2RAD, aspect, CAMERA_CULL_DISTANCE_NEAR, CAMERA_CULL_DISTANCE_FAR);
    }
    else if (camera->projection == CAMERA_ORTHOGRAPHIC)
    {
        double top = camera->fovy/2.0;
        double right = top*aspect;

        return MatrixOrtho(-right, right, -top, top, CAMERA_CULL_DISTANCE_NEAR, CAMERA_CULL_DISTANCE_FAR);
    }

    return MatrixIdentity();
}

#if !defined(RCAMERA_STANDALONE)
// Update camera position for selected mode
// rl_Camera mode: CAMERA_FREE, CAMERA_FIRST_PERSON, CAMERA_THIRD_PERSON, CAMERA_ORBITAL or CUSTOM
void rl_UpdateCamera(rl_Camera *camera, int mode)
{
    rl_Vector2 mousePositionDelta = rl_GetMouseDelta();

    bool moveInWorldPlane = ((mode == CAMERA_FIRST_PERSON) || (mode == CAMERA_THIRD_PERSON));
    bool rotateAroundTarget = ((mode == CAMERA_THIRD_PERSON) || (mode == CAMERA_ORBITAL));
    bool lockView = ((mode == CAMERA_FREE) || (mode == CAMERA_FIRST_PERSON) || (mode == CAMERA_THIRD_PERSON) || (mode == CAMERA_ORBITAL));
    bool rotateUp = false;

    // rl_Camera speeds based on frame time
    float cameraMoveSpeed = CAMERA_MOVE_SPEED*rl_GetFrameTime();
    float cameraRotationSpeed = CAMERA_ROTATION_SPEED*rl_GetFrameTime();
    float cameraPanSpeed = CAMERA_PAN_SPEED*rl_GetFrameTime();
    float cameraOrbitalSpeed = CAMERA_ORBITAL_SPEED*rl_GetFrameTime();

    if (mode == CAMERA_CUSTOM) {}
    else if (mode == CAMERA_ORBITAL)
    {
        // Orbital can just orbit
        rl_Matrix rotation = MatrixRotate(GetCameraUp(camera), cameraOrbitalSpeed);
        rl_Vector3 view = Vector3Subtract(camera->position, camera->target);
        view = Vector3Transform(view, rotation);
        camera->position = Vector3Add(camera->target, view);
    }
    else
    {
        // rl_Camera rotation
        if (rl_IsKeyDown(KEY_DOWN)) CameraPitch(camera, -cameraRotationSpeed, lockView, rotateAroundTarget, rotateUp);
        if (rl_IsKeyDown(KEY_UP)) CameraPitch(camera, cameraRotationSpeed, lockView, rotateAroundTarget, rotateUp);
        if (rl_IsKeyDown(KEY_RIGHT)) CameraYaw(camera, -cameraRotationSpeed, rotateAroundTarget);
        if (rl_IsKeyDown(KEY_LEFT)) CameraYaw(camera, cameraRotationSpeed, rotateAroundTarget);
        if (rl_IsKeyDown(KEY_Q)) CameraRoll(camera, -cameraRotationSpeed);
        if (rl_IsKeyDown(KEY_E)) CameraRoll(camera, cameraRotationSpeed);

        // rl_Camera movement
        // rl_Camera pan (for CAMERA_FREE)
        if ((mode == CAMERA_FREE) && (rl_IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)))
        {
            const rl_Vector2 mouseDelta = rl_GetMouseDelta();
            if (mouseDelta.x > 0.0f) CameraMoveRight(camera, cameraPanSpeed, moveInWorldPlane);
            if (mouseDelta.x < 0.0f) CameraMoveRight(camera, -cameraPanSpeed, moveInWorldPlane);
            if (mouseDelta.y > 0.0f) CameraMoveUp(camera, -cameraPanSpeed);
            if (mouseDelta.y < 0.0f) CameraMoveUp(camera, cameraPanSpeed);
        }
        else
        {
            // Mouse support
            CameraYaw(camera, -mousePositionDelta.x*CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
            CameraPitch(camera, -mousePositionDelta.y*CAMERA_MOUSE_MOVE_SENSITIVITY, lockView, rotateAroundTarget, rotateUp);
        }

        // Keyboard support
        if (rl_IsKeyDown(KEY_W)) CameraMoveForward(camera, cameraMoveSpeed, moveInWorldPlane);
        if (rl_IsKeyDown(KEY_A)) CameraMoveRight(camera, -cameraMoveSpeed, moveInWorldPlane);
        if (rl_IsKeyDown(KEY_S)) CameraMoveForward(camera, -cameraMoveSpeed, moveInWorldPlane);
        if (rl_IsKeyDown(KEY_D)) CameraMoveRight(camera, cameraMoveSpeed, moveInWorldPlane);

        // Gamepad movement
        if (rl_IsGamepadAvailable(0))
        {
            // Gamepad controller support
            CameraYaw(camera, -(rl_GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X)*2)*CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
            CameraPitch(camera, -(rl_GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y)*2)*CAMERA_MOUSE_MOVE_SENSITIVITY, lockView, rotateAroundTarget, rotateUp);

            if (rl_GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) <= -0.25f) CameraMoveForward(camera, cameraMoveSpeed, moveInWorldPlane);
            if (rl_GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) <= -0.25f) CameraMoveRight(camera, -cameraMoveSpeed, moveInWorldPlane);
            if (rl_GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) >= 0.25f) CameraMoveForward(camera, -cameraMoveSpeed, moveInWorldPlane);
            if (rl_GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) >= 0.25f) CameraMoveRight(camera, cameraMoveSpeed, moveInWorldPlane);
        }

        if (mode == CAMERA_FREE)
        {
            if (rl_IsKeyDown(KEY_SPACE)) CameraMoveUp(camera, cameraMoveSpeed);
            if (rl_IsKeyDown(KEY_LEFT_CONTROL)) CameraMoveUp(camera, -cameraMoveSpeed);
        }
    }

    if ((mode == CAMERA_THIRD_PERSON) || (mode == CAMERA_ORBITAL) || (mode == CAMERA_FREE))
    {
        // Zoom target distance
        CameraMoveToTarget(camera, -rl_GetMouseWheelMove());
        if (rl_IsKeyPressed(KEY_KP_SUBTRACT)) CameraMoveToTarget(camera, 2.0f);
        if (rl_IsKeyPressed(KEY_KP_ADD)) CameraMoveToTarget(camera, -2.0f);
    }
}
#endif // !RCAMERA_STANDALONE

// Update camera movement, movement/rotation values should be provided by user
void rl_UpdateCameraPro(rl_Camera *camera, rl_Vector3 movement, rl_Vector3 rotation, float zoom)
{
    // Required values
    // movement.x - Move forward/backward
    // movement.y - Move right/left
    // movement.z - Move up/down
    // rotation.x - yaw
    // rotation.y - pitch
    // rotation.z - roll
    // zoom - Move towards target

    bool lockView = true;
    bool rotateAroundTarget = false;
    bool rotateUp = false;
    bool moveInWorldPlane = true;

    // rl_Camera rotation
    CameraPitch(camera, -rotation.y*rl_DEG2RAD, lockView, rotateAroundTarget, rotateUp);
    CameraYaw(camera, -rotation.x*rl_DEG2RAD, rotateAroundTarget);
    CameraRoll(camera, rotation.z*rl_DEG2RAD);

    // rl_Camera movement
    CameraMoveForward(camera, movement.x, moveInWorldPlane);
    CameraMoveRight(camera, movement.y, moveInWorldPlane);
    CameraMoveUp(camera, movement.z);

    // Zoom target distance
    CameraMoveToTarget(camera, zoom);
}

#endif // RCAMERA_IMPLEMENTATION
