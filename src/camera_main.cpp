#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <../include/shader.h>
#include <../include/camera.h>
#include <../include/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int width = 800;
const int height = 600;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

void framebuffer_resize(GLFWwindow* window, int new_width, int new_height) {
  glViewport(0, 0, new_width, new_height);
}

void processInput(GLFWwindow *window) {

  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

int main()
{
    // initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window and make context current
    GLFWwindow* window = glfwCreateWindow(width, height, "Intro to OpenGL", NULL, NULL);
    if (window == NULL) {
      // throw exception
      std::cout << "GLFW Error" << std::endl;
      glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_resize);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load address of OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "GLAD Error" << std::endl;
    }
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("../shaders/vertex.vs", "../shaders/fragment.fs"); 

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( -10.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3 (2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../textures/paint.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("../textures/awesomeface.png", &width, &height, &nrChannels, 0);
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

    ourShader.use(); 
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    while(!glfwWindowShouldClose(window)) {
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;  
      
      // input
      processInput(window);
      
      // render
      // ------
      glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // bind textures on corresponding texture units
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);

      // render the triangle
      ourShader.use();

      // pass projection matrix to shader (note that in this case it could change every frame)
      glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
      ourShader.setMat4("projection", projection);

      // camera/view transformation
      glm::mat4 view = camera.GetViewMatrix();
      ourShader.setMat4("view", view);

      // render boxes
      glBindVertexArray(VAO);
      for (unsigned int i = 0; i < 10; i++) {
          // calculate the model matrix for each object and pass it to shader before drawing
          glm::mat4 model = glm::mat4(1.0f);
          model = glm::translate(model, cubePositions[i]);
          float angle = 35.0f * i;
          model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
          ourShader.setMat4("model", model);

          glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
      // -------------------------------------------------------------------------------
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}