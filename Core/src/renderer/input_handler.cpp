#include "input_handler.h"

#include "camera.h"

#include <GLFW/glfw3.h>

InputHandlerGLFW::InputHandlerGLFW(GLFWwindow* pWnd, float wndAspectRatio, Camera& camera)
    : mWnd(pWnd)
    , mWndAspectRatio(wndAspectRatio)
    , mCamera(camera)
    , mLastTime(glfwGetTime())
    , mLastX(0.0), mLastY(0.0)
    , mFirstMouse(true)
{
}

glm::mat4 InputHandlerGLFW::calcInvViewProj()
{
    // Mouse look with right click
    if (glfwGetMouseButton(mWnd, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(mWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        double x, y;
        glfwGetCursorPos(mWnd, &x, &y);
        if (mFirstMouse) { mLastX = x; mLastY = y; mFirstMouse = false; }

        float dx = float(x - mLastX);
        float dy = float(mLastY - y);
        mLastX = x; mLastY = y;

        const float sens = 0.08f;
        mCamera.addPitchYaw(dx * sens, dy * sens);
    }
    else
    {
        glfwSetInputMode(mWnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mFirstMouse = true;
    }

    // WASD move
    float speed = 3.0f;
    if (glfwGetKey(mWnd, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) speed = 8.0f;

    // Get time delta
    double now = glfwGetTime();
    float dt = float(now - mLastTime);
    mLastTime = now;

    if (glfwGetKey(mWnd, GLFW_KEY_W) == GLFW_PRESS) mCamera.pos += mCamera.forward() * speed * dt;
    if (glfwGetKey(mWnd, GLFW_KEY_S) == GLFW_PRESS) mCamera.pos -= mCamera.forward() * speed * dt;
    if (glfwGetKey(mWnd, GLFW_KEY_A) == GLFW_PRESS) mCamera.pos -= mCamera.right() * speed * dt;
    if (glfwGetKey(mWnd, GLFW_KEY_D) == GLFW_PRESS) mCamera.pos += mCamera.right() * speed * dt;

    // update invViewProj
    glm::mat4 view = mCamera.view();
    glm::mat4 proj = mCamera.proj(mWndAspectRatio);
    glm::mat4 invViewProj = glm::inverse(proj * view);

    return invViewProj;
}
