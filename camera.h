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
            worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
            this->up = glm::normalize(up);
            this->direction = glm::normalize(direction);

            speed = 2.5f;
            sensitivity = 0.6f;

            yaw = -90.0f;
            pitch =  0.0f;
        }

        Camera(glm::vec3 camPos, glm::vec3 up, glm::vec3 direction, float speed, float sensitivity){
            this->camPos = camPos;
            worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
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

        glm::mat4 worldToCamMatrix(){
            // 1. Normalize direction
            glm::vec3 newN = glm::normalize(direction);
            // 2. Get positive right axis vector
            glm::vec3 newU = glm::normalize(glm::cross(glm::normalize(worldUp), newN));
            // 3. Calculate camera up vector
            glm::vec3 newV = glm::cross(newN, newU);

            // Create translation and rotation matrix
            // In glm we access elements as mat[col][row] due to column-major layout
            glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
            translation[3][0] = -camPos.x; // Third column, first row
            translation[3][1] = -camPos.y;
            translation[3][2] = -camPos.z;
            glm::mat4 rotation = glm::mat4(1.0f);
            rotation[0][0] = newU.x; // First column, first row
            rotation[1][0] = newU.y;
            rotation[2][0] = newU.z;
            rotation[0][1] = newV.x; // First column, second row
            rotation[1][1] = newV.y;
            rotation[2][1] = newV.z;
            rotation[0][2] = newN.x; // First column, third row
            rotation[1][2] = newN.y;
            rotation[2][2] = newN.z; 

            // Return lookAt matrix as combination of translation and rotation matrix
            return rotation * translation; // Remember to read from right to left (first translation then rotation)
        }

        //broken rn
        glm::mat4 camToProjMatrix(float FOV, float width, float height, float nearZ, float farZ){
            FOV = glm::radians(FOV/2.0f);
            float ar = width/height;
            float A = (-nearZ - farZ)/(nearZ - farZ);
            float B = (2 * nearZ * farZ)/(nearZ - farZ);

            // Create translation and rotation matrix
            // In glm we access elements as mat[col][row] due to column-major layout
            glm::mat4 rotation = glm::mat4(1.0f);
            rotation[0][0] = ar / (tan(FOV)); // First column, first row
            rotation[1][1] = 1.0f / (tan(FOV));
            rotation[2][2] = A; 
            rotation[2][3] = 1.0f; 
            rotation[3][2] = B; 
            rotation[3][3] = 0.0f; 

            // Return lookAt matrix as combination of translation and rotation matrix
            return rotation; // Remember to read from right to left (first translation then rotation)
        }

        // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
        {
            xoffset *= sensitivity * -1;//since we want to apply counter-clockwise rotation
            yoffset *= sensitivity;

            //updated up and direction vector using updated angles
            //pitch axis: right, x
            pitch += yoffset;
            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch)
            {
                if (pitch > 89.0f){
                    pitch = 89.0f;
                    yoffset = 0;
                }
                if (pitch < -89.0f){
                    pitch = -89.0f;
                    yoffset = 0;
                }
            }

            glm::vec3 right = glm::normalize(glm::cross(up, direction));

            /* old code when simulating euler angles with Quaternions
            QuaternionRotate(right, yoffset, up);
            QuaternionRotate(right, yoffset, direction);

            yaw axis: up, y (locked yaw to worldUp axis to prevent unwanted roll)
            QuaternionRotate(worldUp, xoffset, direction);
            */

            CameraRotate(worldUp, right, xoffset, yoffset, direction);
            //by rotating direction around WorldUp we must recalculate both right and up
            right = glm::normalize(glm::cross(worldUp, direction));
            up = glm::normalize(glm::cross(direction, right));
        }

        //combined multiple quaternion
        void CameraRotate(glm::vec3 axis1, glm::vec3 axis2, float angle1, float angle2, glm::vec3& point)
        {

            //quat 1
            float angleRad1 = glm::radians(angle1);
            axis1 = glm::normalize(axis1);

            float w = glm::cos(angleRad1 / 2);
            float v = glm::sin(angleRad1 / 2);
            glm::vec3 qv =  v * axis1;

            glm::quat rot1 = glm::quat(w, qv);
            
            //quat 2
            float angleRad2 = glm::radians(angle2);
            axis2 = glm::normalize(axis2);

            w = glm::cos(angleRad2 / 2);
            v = glm::sin(angleRad2 / 2);
            qv =  v * axis2;

            glm::quat rot2 = glm::quat(w, qv);
            
            glm::quat rot = rot1 * rot2;
            
            point = glm::normalize(rot * point);
        }

        //not neccesary but cool to understand/look at
        void QuaternionRotate(glm::vec3 axis, float angle, glm::vec3& point)
        {
            float angleRad = glm::radians(angle);
            axis = glm::normalize(axis);

            float w = glm::cos(angleRad / 2);
            float v = glm::sin(angleRad / 2);
            glm::vec3 qv =  v * axis;

            glm::quat rot = glm::quat(w, qv);
            point = glm::normalize(rot * point);
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