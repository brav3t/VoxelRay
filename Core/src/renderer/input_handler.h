#pragma once

#include "camera.h"

#include <glm/fwd.hpp>
#include <GLFW/glfw3.h>

class InputHandlerGLFW
{
public:
    InputHandlerGLFW(GLFWwindow* pWnd, float wndAspectRatio, Camera& camera);

    glm::mat4 calcInvViewProj();

private:
    GLFWwindow* mWnd;
    float mWndAspectRatio;

    Camera& mCamera;

    double mLastTime;
    double mLastX, mLastY;
    bool mFirstMouse;
};
