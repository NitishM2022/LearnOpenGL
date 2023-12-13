#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "stb_image.h"
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
        //position          //color             //texture coords
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   2.0f, 2.0f,//top right
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   2.0f, 0.0f,//bottom right 
        0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,//top middle
        0.0f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,   0.0f, 0.0f,//bottom middle
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,   0.0f, 2.0f,//top left 
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,//bottom left
    };

    //only care about unique indices so we don't need to store duplicates
    unsigned int indices[] = {  // note that we start from 0!
        4, 5, 1,   // first triangle
        4, 0, 1,   // second triangle
    };

    //texture coordinates
    // float texCoords[] = {
    //     0.0f, 0.0f,  // lower-left corner  
    //     1.0f, 0.0f,  // lower-right corner
    //     0.5f, 1.0f   // top-center corner
    // };

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
                          8 * sizeof(float), //how far the starting position of each val is from the next
                          (void*)0);//where it starts
    glEnableVertexAttribArray(0);  

    //attribute for color
    glVertexAttribPointer(1, //sets the location of the vertex attribute to 0. So where the data in the buffer can be found
                          3, //how elements is each color atribute
                          GL_FLOAT, //what data type is the color
                          GL_FALSE, //if we want data to be normalized from 0 to 1
                          8 * sizeof(float), //how far the starting position of each set of colors to the next (stride)
                          (void*)(3 * sizeof(float)));//where it starts
    glEnableVertexAttribArray(1);  

    //attribute for texture
    glVertexAttribPointer(2, //sets the location of the vertex attribute to 0. So where the data in the buffer can be found
                          2, //how elements is each color atribute
                          GL_FLOAT, //what data type is the color
                          GL_FALSE, //if we want data to be normalized from 0 to 1
                          8 * sizeof(float), //how far the starting position of each set of colors to the next (stride)
                          (void*)(6 * sizeof(float)));//where it starts
    glEnableVertexAttribArray(2);  

    //load textures
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1); //link texture to unsigned int
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, texture1);  //bind texture so all command apply to binded texture
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //image loader
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  
    unsigned char *data = stbi_load("textures/wood_container.jpg", &width, &height, &nrChannels, 0); 

    if(data){
         glTexImage2D(GL_TEXTURE_2D, //only bound 2d textures will be affected
                 0, //sets what mipmap level you want to creat texture if you want manual control (would have to call function again for every level)
                 GL_RGB, // we want to store texture in rgb
                 width, //width of texture
                 height, //height of texture
                 0, //some legacy stuff (should always be 0)
                 GL_RGB, //image rbg
                 GL_UNSIGNED_BYTE, //stored image as unsigned char array
                 data //actual image data
                 );
        glGenerateMipmap(GL_TEXTURE_2D);//generates bunch of different size versions of a texture
    }else{
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);

    // texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("textures/awesome_face.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
 
    shaderProgram.use();//must activate shader before setting uniforms
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);

    //scale and rotate
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));  

    //set uniform in vertex shader
    unsigned int transformLoc = glGetUniformLocation(shaderProgram.shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, //uniform location
                         1, //number of matrices
                         GL_FALSE, // do you want to transpose (swap row and columns)
                         glm::value_ptr(trans) //actual matrix
                         );

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
        //float timeColor = (sin(timeValue) / 2.0f) + 0.5f;
        shaderProgram.setFloat("timeValue", timeValue);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //render triangle
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//drawing elements instead of arrays 

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

