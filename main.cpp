/**
 * @file main.cpp 
 * @brief Multiphase SPH Based Shallow Water Simulation
 * @author Yuki Ogiwara
 * @date 2022-05-07
 */

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "constant.hpp"
#include "scene.hpp"

int window_width = 800;
int window_height = 600;

std::unique_ptr<Scene> scene;

// callback functions

/**
 * @brief output error
 * @param[in] error error code
 * @param[in] description description
 */
void glfwErrorCallBack(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

/**
 * @brief cursor movement callback
 * @param[in] window window handler
 * @param[in] x position
 * @param[in] y position
 */
void glfwCursorPosCallBack(GLFWwindow* window, double x, double y) {

}

/**
 * @brief mouse button callback
 * @param[in] window window handler
 * @param[in] button button code
 * @param[in] action key action
 * @param[in] mods modifier key
 */
void glfwMouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {

}

/**
 * @brief key callback
 * @param[in] window window handler
 * @param[in] key key code
 * @param[in] scancode scancode
 * @param[in] action key action
 * @param[in] mods modifier key
 */
void glfwKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_ESCAPE:
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            default:
                break;
        }
    }
}

/**
 * @brief resize callback
 * @param[in] window window handler
 * @param[in] width window width
 * @param[in] height window height
 */
void glfwFramebufferSizeCallBack(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;

    float aspcet_ratio = (float)window_width / (float)window_height;
    scene->SetAspectRatio (aspcet_ratio);

    glViewport(0, 0, window_width, window_height);
}

int main(int argc, char* argv[]) {
    // initialize GLFW
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(1);
    }

    // set function to be executed on exit
    atexit(glfwTerminate);

    // configurations of GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kOpenGLVersionMejor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kOpenGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_TRUE);
#endif

    // create a window
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, kTitle.c_str(), NULL, NULL);
    if(window == NULL) {
        std::cerr << "Falied to create GLFW window" << std::endl;
        exit(1);
    }

    // make context current
    glfwMakeContextCurrent(window);

    // register callback functions
    glfwSetErrorCallback(glfwErrorCallBack);
    glfwSetCursorPosCallback(window, glfwCursorPosCallBack);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallBack);
    glfwSetKeyCallback(window, glfwKeyCallBack);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallBack);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(1);
    }

    // settings
    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(kBackgroundColor.r, kBackgroundColor.g, kBackgroundColor.b, 1.0f);
    glPointSize(kPointSize);

    // create a scene
    scene = std::make_unique<Scene>(window_width, window_height);

    // set timer
    float current_time = 0.0f;
    float last_time    = 0.0f;
    float elapsed_time = 0.0f;
    glfwSetTime(0.0);

    // ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // main loop
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->Draw();

        // timer
        current_time = glfwGetTime();
        elapsed_time = current_time - last_time;
        if(elapsed_time >= scene->GetDeltaTime()) {
            scene->Update();
            last_time = current_time;
        }

        // GUI
        ImGui_ImplOpenGL3_NewFrame(); 
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("ImGui Window");
        ImGui::Text("Framerate: %.3f ms/frame (%.1f fps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        scene->ImGui(window);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // terminate program
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    exit(0);
}
