#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "model.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Camera
//these vectors represent the axis of the camera, and they are represented in world coordinates
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f); //Up
glm::vec3 N = glm::vec3(0.0f, 0.0f, 1.0f); //direction (Not such a great name since it points opposite to the side the camera is facing)
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), V, N);
bool flashLightPress = false;
bool flashLightOn = true;

//Perspective
float FOV = 45.0f;

//Mouse control
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f; 

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //tell GLFW we are using core profile
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    //create an 800 by 800 size screen with the name Learn Open GL
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    //checks to see if you were unable to create a window
    if(window == NULL){
        cout << "Failed to create window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);// introduce window into current context
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    //Load GLAD to configure for OpenGL
    gladLoadGL();

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    //the z value is stored for each fragment and if the fragment wasnt to output its color, its z value must be above the current one
    glEnable(GL_DEPTH_TEST);  

    //build and compile shaders
    Shader ourShader("shaders/colors.vs", "shaders/colors.fs");

    Model ourModel("models/backpack/backpack.obj");

    //prevent program from reaching end and terminating
    while(!glfwWindowShouldClose(window)){
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //input
        processInput(window);

        //set color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//change color buffer and depth buffer from prev frame

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        //set MVP uniforms
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.worldToCamMatrix();
        glm::mat4 projection = camera.camToProjMatrix(FOV, (float) SCR_WIDTH, (float) SCR_HEIGHT, 0.1f, 100.0f);

        //set uniform in color vertex shader
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //draw model
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();//Terminate GLW before ending the program
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        if(!flashLightPress){
            flashLightOn = !flashLightOn;
            flashLightPress = true;
        }
    }else if(glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE){
        flashLightPress = false;
    }
        
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    //if mouse is all the way to the right I want to yaw 90
    //if mouse if all the way to the top I want to pitch 90

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = (xpos - lastX)/(SCR_WIDTH / 2.0f);
    float yoffset = (lastY - ypos)/(SCR_HEIGHT / 2.0f);// reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, FOV);
}