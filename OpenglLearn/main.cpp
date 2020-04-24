#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "src/Renderer.h"
#include "src/VertexBuffer.h"
#include "src/IndexBuffer.h"
#include "src/VertexArray.h"
#include "src/Shader.h"
#include "src/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include <imgui\imgui_impl_opengl3.h>


int main()
{
    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
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

    glClearColor(0, 0, 0, 0);

    float positions[] = {
         -50.0f, -50.0f, 0.0f, 0.0f,
         50.0f, -50.0f, 1.0f, 0.0f,
        50.0f, 50.0f, 1.0f, 1.0f,
        -50.0f, 50.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    {
        VertexArray* va = new VertexArray();

        VertexBufferLayout* layout = new VertexBufferLayout();
        layout->Push<float>(2);
        layout->Push<float>(2);

        VertexBuffer* vb = new VertexBuffer(positions, sizeof(positions));
        va->AddBuffer(*vb, *layout);

        IndexBuffer* ib = new IndexBuffer(indices, sizeof(indices));

        glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0, 0));
       

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        
        Texture texture("res/textures/logo.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);//the slot number the texture is at .. 
       
        Renderer renderer;
        
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();

        // Our state
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        glm::vec3 modelTranslate(100.0f, 100.0f, 0);
        float r = 0;
        float increment = 0.05f;
        // render loop
       // -----------
        while (!glfwWindowShouldClose(window))
        {
            // input
            // -----
            processInput(window);
            
            // render
            // ------
           // clear the color buffer before each drawing 
            renderer.Clear(); 

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 model = glm::translate(glm::mat4(1.0f), modelTranslate);
            glm::mat4 mvp = proj * view * model;

            shader.Bind();
            shader.SetUniform4f("u_color",r, 0.3f, 0.8f, 1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);
            

            // draw triangles starting from index 0 and 
            // using 3 indices 
            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                ImGui::Begin("Debug!");                          // Create a window called "Hello, world!" and append into it.
                ImGui::SliderFloat3("ModelTranslate", &modelTranslate.x, 0.0f, 800.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
           
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // swap the buffers and hence show the buffers  
            // content to the screen 
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        delete(ib);
        delete(vb);
        delete(va);
        delete(layout);
        shader.Unbind();
    }
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
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