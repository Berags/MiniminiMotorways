//
// Created by Jacopo Beragnoli on 02/04/22.
//

#include "../../include/engine/MainLoop.h"
#include <string>
#include <iostream>

namespace Engine {
    MainLoop::MainLoop(Engine::Window *window) : window(window) {}

    void MainLoop::start() {
        update();
        stop();
    }

    void MainLoop::stop() {
        delete window;
    }

    void MainLoop::update() const {
        while (!glfwWindowShouldClose(window->getGlfwWindow())) {
            glClear(GL_COLOR_BUFFER_BIT);

            // Calling the main render function
            render();

            // Swap front and back buffers
            glfwSwapBuffers(window->getGlfwWindow());

            // Poll for and process events
            glfwPollEvents();
        }
    }

    void MainLoop::render() {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnd();
    }
}