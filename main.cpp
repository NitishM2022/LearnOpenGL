#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    
    //Load GLAD to configure for OpenGL
    gladLoadGL();

    //build and compile shaders
    Shader shaderProgram("shaders/vshader.glsl", "shaders/fshader.glsl");

    //set up vertex data and buffer
    float vertices[] = {
        // position             //color
        -0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,// top left 
        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,// top middle
        0.5f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f,// top right
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,// bottom left
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,// bottom right 
    };

    //only care about unique indices so we don't need to store duplicates
    unsigned int indices[] = {  // note that we start from 0!
        1, 3, 4,   // first triangle
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO); 

    unsigned int VBO;// create a buffer id
    glGenBuffers(1, &VBO);  //assign such buffer ID to VBO

    unsigned int EBO;//like vbo bust stores indices of what vertices to draw
    glGenBuffers(1, &EBO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);

    //0. Create a vertex buffer object and pass in our vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);   //assign said id to a buffer object (Any call to GL_ARRAY_BUFFER will configure the current bound buffer VBO)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copy user data into current bound buffer. Last arg specify how often buffer used

    //create indices buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    //1. Set how the vertex buffer can be used
    //specifies how the shader applies to the vertex buffer
    glVertexAttribPointer(0, //sets the location of the vertex attribute to 0. So where the data in the buffer can be found
                          3, //how big is each vertex atribute
                          GL_FLOAT, //what data type is the vertex
                          GL_FALSE, //if we want data to be normalized from 0 to 1
                          6 * sizeof(float), //how far the starting position of each val is from the next
                          (void*)0);//where it starts
    glEnableVertexAttribArray(0);  

    //attrivute for color
    glVertexAttribPointer(1, //sets the location of the vertex attribute to 0. So where the data in the buffer can be found
                          3, //how elements is each color atribute
                          GL_FLOAT, //what data type is the color
                          GL_FALSE, //if we want data to be normalized from 0 to 1
                          6 * sizeof(float), //how far the starting position of each set of colors to the next (stride)
                          (void*)(3 * sizeof(float)));//where it starts
    glEnableVertexAttribArray(1);  
 
    //prevent program from reaching end and terminating
    while(!glfwWindowShouldClose(window)){

        //input
        processInput(window);

        //set color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);//only change color buffer bit

        //2. use shader Object
        shaderProgram.use();
        //change the color based on time
        float timeValue = glfwGetTime();
        float timeColor = (sin(timeValue) / 2.0f) + 0.5f;
        shaderProgram.setFloat("timeValue", timeValue);

        //render triangle
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);//drawing elements instead of arrays 

        glfwSwapBuffers(window);//swap the buffers
        glfwPollEvents();//takes care of all GLFW events
    }

    glfwTerminate();//Terminate GLW before ending the program
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

