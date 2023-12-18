#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"

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
            glm::vec3 worldUp = glm::vec3(0.0,1.0,0.0);
            this->up = glm::normalize(up);
            this->direction = glm::normalize(direction);

            speed = 2.5;
            sensitivity = 0.7f;

            yaw = -90.0f;
            pitch =  0.0f;
        }

        Camera(glm::vec3 camPos, glm::vec3 up, glm::vec3 direction, float speed, float sensitivity){
            this->camPos = camPos;
            glm::vec3 worldUp = glm::vec3(0.0,1.0,0.0);
            this->up = glm::normalize(up);
            this->direction = glm::normalize(direction);

            this->speed = speed;
            this->sensitivity = sensitivity;

            yaw = -90.0f;
            pitch =  0.0f;
        }

        void setPosition(float x, float y, float z){
            camPos = glm::vec3(x, y, z);
        }
        
        void ProcessKeyboard(Camera_Movement move, float deltaTime){
            //operations are opposite since direction points opposite to where the camera is facing
            float cameraSpeed = static_cast<float>(speed * deltaTime);
            if (move == FORWARD)
                camPos -= cameraSpeed * direction; //want to decrease the direction since you are pointing in reverse direction
            if (move == BACKWARD)
                camPos += cameraSpeed * direction;
            if (move == LEFT)
                camPos -= glm::normalize(glm::cross(up, direction)) * cameraSpeed;
            if (move == RIGHT)
                camPos += glm::normalize(glm::cross(up, direction)) * cameraSpeed;
        };

        glm::mat4 GetMatrix(){
            glm::vec3 newV = glm::normalize(up);//up
            glm::vec3 newN = glm::normalize(direction);//direction
            glm::vec3 newU = glm::cross(newV, newN);
            float modelToCam[16] = {newU.x, newU.y, newU.z, -camPos.x,
                                    newV.x, newV.y, newV.z, -camPos.y,
                                    newN.x, newN.y, newN.z, -camPos.z,
                                    0.0f, 0.0f, 0.0f, 1.0f};

            //you must transpose due to column major ordering
            glm::mat4 modelToCamTransformation = glm::transpose(glm::make_mat4(modelToCam));

            return modelToCamTransformation;
        }

        // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
        {
            xoffset *= sensitivity * -1;//since we want to apply counter-clockwise rotation
            yoffset *= sensitivity;

            //updated up and direction vector using updated angles
            //yaw axis: up, y
            QuaternionRotate(up, xoffset, direction);

            //pitch axis: right, x
            glm::vec3 right = glm::cross(up, direction);
            QuaternionRotate(right, yoffset, up);
            
            //recalculate direction
            right = glm::cross(up, direction);
            direction = glm::cross(right, up);
        }

        void QuaternionRotate(glm::vec3& axis, float angle, glm::vec3& direction)
        {
            float angleRad = glm::radians(angle);
            axis = glm::normalize(axis);

            float w = glm::cos(angleRad / 2);
            float v = glm::sin(angleRad / 2);
            glm::vec3 qv =  v * axis;

            glm::quat rot = glm::quat(w, qv);
            direction = rot * direction;
        }


    private:
        glm::vec3 camPos;
        glm::vec3 direction;
        glm::vec3 worldUp;
        glm::vec3 up;

        float speed;
        float sensitivity;//range 0 - 1 exculsive
        float yaw;
        float pitch;
};

#endif