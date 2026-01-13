#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

struct Camera
{
    glm::vec3 pos{0.0f, 0.0f, 3.0f};
	float yaw = -90.0f; // y axle left, right (forward -Z)
    float pitch = 0.0f; // x axle up, down
    float fov = 60.0f; // field of view in degrees

    glm::vec3 forward() const
    {
        float cy = cos(glm::radians(yaw));
        float sy = sin(glm::radians(yaw));
        float cp = cos(glm::radians(pitch));
        float sp = sin(glm::radians(pitch));

        return glm::normalize(glm::vec3(cy * cp, sp, sy * cp));
    }

    glm::vec3 right() const
    {
        return glm::normalize(glm::cross(forward(), glm::vec3(0,1,0)));
    }

    glm::mat4 view() const
    {
        return glm::lookAt(pos, pos + forward(), glm::vec3(0,1,0));
    }

    glm::mat4 proj(float aspect) const
    {
        return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
    }

    void addPitchYaw(float dYaw, float dPitch)
    {
        yaw += dYaw;
        pitch = std::clamp(pitch + dPitch, -89.0f, 89.0f);
    }
};
