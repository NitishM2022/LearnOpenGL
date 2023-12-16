#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera{
    public:
        Camera(glm::vec3 camPos, glm::vec3 up, glm::vec3 direction){
            this->camPos = camPos;
            this->up = up;
            this->direction = direction;
        }

        void setPosition(float x, float y, float z){
            camPos = glm::vec3(x, y, z);
        }
        
        void ProcessKeyboard(Camera_Movement move, float deltaTime){
            float cameraSpeed = static_cast<float>(speed * deltaTime);
            if (move == FORWARD)
                camPos -= cameraSpeed * direction; //want to decrease the direction since you are pointing in reverse direction
            if (move == BACKWARD)
                camPos += cameraSpeed * direction;
            if (move == LEFT)
                camPos += glm::normalize(glm::cross(direction, up)) * cameraSpeed;
            if (move == RIGHT)
                camPos -= glm::normalize(glm::cross(direction, up)) * cameraSpeed;
        };

        glm::mat4 GetMatrix(){
            glm::vec3 newV = glm::normalize(up);//up
            glm::vec3 newN = glm::normalize(direction);//direction
            glm::vec3 newU = glm::cross(newN, newV);
            float modelToCam[16] = {newU.x, newU.y, newU.z, -camPos.x,
                                    newV.x, newV.y, newV.z, -camPos.y,
                                    newN.x, newN.y, newN.z, -camPos.z,
                                    0.0f, 0.0f, 0.0f, 1.0f};

            //you must transpose due to column major ordering
            glm::mat4 modelToCamTransformation = glm::transpose(glm::make_mat4(modelToCam));

            return modelToCamTransformation;
        }

    private:
        glm::vec3 camPos;
        glm::vec3 direction;
        glm::vec3 up;
        float speed = 2.5;
};

#endif