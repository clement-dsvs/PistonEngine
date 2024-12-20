//
// Created by cleme on 20/12/2024.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace piston {
    class Camera
    {
    public:
        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

        void setPerspectiveProjection(float fovy, float aspect, float near, float far);

        void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0, -1, 0));
        void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0, -1, 0));

        void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

        const glm::mat4& getProjection() const { return projectionMatrix; }
        const glm::mat4& getView() const { return viewMatrix; }
    private:
        glm::mat4 projectionMatrix{1};
        glm::mat4 viewMatrix{1};
    };
}

#endif //CAMERA_H
