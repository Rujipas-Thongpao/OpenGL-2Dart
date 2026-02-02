#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Shader.h"
#include "Vertex.h"
#include "Mesh.h"
#include "NoiseUtils.h"
#include <iostream>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
glm::vec3 mousePos;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("vertex.vs", "fragment.fs"); // you can name your shader files however you like


    std::vector<Vertex> circleVertices;
	// Center vertex
	circleVertices.push_back({ 0.0f, 0.0f, 0.0f, 0.5f, 0.5f });

    int segments = 20;
    float radius = 1.0;

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;

        float x = radius * cos(angle);
        float y = radius * sin(angle);

        float u = (x / radius + 1.0f) * 0.5f;
        float v = (y / radius + 1.0f) * 0.5f;

        // Edge vertex
        circleVertices.push_back({ x, y, 0.0f, u, v });
    }

	Mesh circleMesh(circleVertices);

    
    std::vector<Vertex> triangleVerticesVec = {
        {  1.0f, -1.0f, 0.0f, 1.0f, 0.0f }, // bottom right
        { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f }, // bottom left
        {  0.0f,  1.0f, 0.0f, 1.0f, 1.0f }  // top
    };

	Mesh triangleMesh(triangleVerticesVec);


    std::vector<Vertex> squareVerticesVec = {
        {  1.0f,  1.0f, 0.0f, 1.0f, 1.0f }, // top right
        {  1.0f, -1.0f, 0.0f, 1.0f, 0.0f }, // bottom right
        { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f }, // bottom left
        { -1.0f,  1.0f, 0.0f, 0.0f, 1.0f }  // top left
	};

	Mesh squareMesh(squareVerticesVec);

    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("Texture/wall.jpg", &width, &height, &nrChannels, 0);
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
    data = stbi_load("Texture/Ruri.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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

    	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

    // Variables to be changed in the ImGUI window
    //bool drawTriangle = true;
    int scale = 100;
    float color0[4] = { 0.8f, 0.3f, 0.02f, 1.0f };
    float color1[4] = { 0.8f, 0.3f, 0.02f, 1.0f };
    float color2[4] = { 0.8f, 0.3f, 0.02f, 1.0f };
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
		glEnable(GL_DEPTH_TEST);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGUI window creation
        ImGui::Begin("My name is window, ImGUI window");
        // Text that appears in the window
        ImGui::Text("Hello there adventurer!");
        ImGui::SliderInt("Scale", &scale, 10, 200);
        // Fancy color editor that appears in the window
        ImGui::ColorEdit4("Color0", color0);
        ImGui::ColorEdit4("Color1", color1);
        ImGui::ColorEdit4("Color2", color2);
        // Ends the window
        ImGui::End();


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

        // note that we're translating the scene in the reverse direction of where we want to move
        glm::mat4 projection;
         projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
		//projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -10.0f, 100.0f);

        ourShader.use();
		ourShader.setMat4("model", model);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

		//circleMesh.draw();
   //     glBindVertexArray(VAO);
        unsigned int scaleX = scale;
        unsigned int scaleY = scale;
        for (unsigned int i = 0; i < scaleX; i++)
        {
            for (unsigned int j = 0; j < scaleY; j++)
            {
				float u = (float)i / (float)scaleX;
				float v = (float)j / (float)scaleY;
                glm::mat4 model = glm::mat4(1.0f);
				glm::vec3 position = glm::vec3((i * 2.0f)/(float)scaleX - 1.0f, (j * 2.0f) / (float)scaleY - 1.0f, 0.0f);
                model = glm::translate(model, position);
				glm::vec3 lookDir = glm::normalize(mousePos - position);


                u *= 10 ;
                v *= 10 ;
                u += (float)glfwGetTime() * glm::normalize(mousePos).x;
                v += (float)glfwGetTime() * glm::normalize(mousePos).y;
                float noise = 0.0;
                noise = NoiseUtils::InterpolatedNoise(0,u,v);
                noise += 0.6;
                if (noise < 0.3f) {
					ourShader.setVec4("color", color0[0], color0[1], color0[2],color0[3]);
                }
                else if (noise < 0.6f) {
					ourShader.setVec4("color", color1[0], color1[1], color1[2],color1[3]);
                }
                else {
					ourShader.setVec4("color", color2[0], color2[1], color2[2],color2[3]);
                }

				ourShader.setFloat("noise", noise);
				model = glm::scale(model, glm::vec3(1.0f/(float)scaleX, 1.0f/(float)scaleY, 1.0) * noise);

				model = glm::rotate(model, atan2(lookDir.y, lookDir.x) + glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

                ourShader.setMat4("model", model);

				float rand = (NoiseUtils::Noise(0, i, j) + 1.0f)/2.0f;
                if (rand < 0.3f) {
					squareMesh.draw();
                }
                else if (rand < 0.6f) {
                    triangleMesh.draw();
                }
                else {
					circleMesh.draw();
                }
            }
        }


        // Renders the ImGUI elements
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deletes all ImGUI instances
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    double xpos, ypos;
    //getting cursor position
    glfwGetCursorPos(window, &xpos, &ypos);
    xpos = (xpos  / SCR_WIDTH) - 0.5f;
    ypos = (ypos / SCR_HEIGHT) - 0.5f;
	mousePos = glm::vec3(xpos, ypos,0.0);
	std::cout << "Mouse Position: (" << mousePos.x << ", " << mousePos.y << ")\n";
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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