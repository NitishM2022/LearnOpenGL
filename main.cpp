#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
}; 

//vertex shader is a vector of 4 length x,y,z and w
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0"; 

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //tell GLFW we are using core progile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  //macos neccesary line

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
    
    gladLoadGL();//Load GLAD to configure for OpenGL

    //Specify the viewport of the OpenGL in the window
    //viewpoert goes from 0,0 to 800,800
    glViewport(0, 0, 800, 800);

    //set a color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);//clean the back buffer


    //0. Create a vertex buffer object and pass in our vertices
    unsigned int VBO;// create a buffer id
    glGenBuffers(1, &VBO);  //assign such buffer ID to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);   //assign said id to a buffer object (Any call to GL_ARRAY_BUFFER will configure the current bound buffer VBO)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copy user data into current bound buffer. Last arg specify how often buffer used

    // once again need to create shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //attach shader to shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //check to see if shader compilation is succesful
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //create another shader object for fragment shadder
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//attach source to fragment shader
    glCompileShader(fragmentShader);

    //create a shader program object to link shader to the next shader
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //check to see if linking was succesful
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //2. use the liked shader program object
    glUseProgram(shaderProgram);
    //delete shaders after you are done using them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  

    //1. Set how the vertex buffer can be used
    //specifies how the shader applies to the vertex buffer
    glVertexAttribPointer(0, //sets the location of the vertex attribute to 0
                          3, //how big is each vertex atribute
                          GL_FLOAT, //what data type is the vertex
                          GL_FALSE, //if we want data to be normalized from 0 to 1
                          3 * sizeof(float), //how far the starting position of each val is from the next
                          (void*)0);//where it starts
    glEnableVertexAttribArray(0);  
 
    //prevent program from reaching end and terminating
    while(!glfwWindowShouldClose(window)){

        //input
        processInput(window);

        //set color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);//only change color buffer bit

        glfwSwapBuffers(window);//swap the buffers
        glfwPollEvents();//takes care of all GLFW events
    }

    glfwTerminate();//Terminate GLW befoer ending the program
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

