#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../IWindow.hpp"
#include <Utilities/IO.hpp>

namespace Labyrinth::lvk {

    class Window : public IWindow {

        friend class Renderer;

        GLFWwindow*        mWindow  = nullptr;
        GLFWmonitor*       mMonitor = nullptr;
		const GLFWvidmode* mMode    = nullptr;

    public:

         Window(const std::string& title, uint32_t width, uint32_t height, bool Fullscreen);
         Window();
        ~Window();

        virtual int  Open(const std::string& Title, uint32_t Width, uint32_t Height) override;
        virtual int  Open()      override;
        virtual void Update()    override;
		virtual void Close()     override;
		virtual void Terminate() override;

    };

    Window::Window(const std::string& Title, uint32_t Width, uint32_t Height, bool Fullscreen) : IWindow(Title, Width, Height, Fullscreen, GAPI::Vulkan) {}
    Window::Window() : IWindow(GAPI::Vulkan) {}

    Window::~Window() {  }

    int Window::Open(const std::string& Title, uint32_t Width, uint32_t Height) {
        mTitle = Title; mWidth = Width; mHeight = Height;
        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        mMonitor = glfwGetPrimaryMonitor();
        mMode    = glfwGetVideoMode(mMonitor);

        mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), mFullscreen ? mMonitor : nullptr, nullptr);
        if(mWindow == nullptr) {
            errorln("Failed to create glfw window!");
            return -1;
        }

        glfwMakeContextCurrent  (mWindow);
		glfwSetWindowUserPointer(mWindow, this);

        mValid = true;

        return 0;
    }

    int  Window::Open() {
        return Open(mTitle, mWidth, mHeight);
    }
    void Window::Update()    {
        if(!glfwWindowShouldClose(mWindow)) {
            glfwPollEvents();
        }
        else {
            mValid = false;
        }
    }
    void Window::Close()     {
        if(mValid) { glfwWindowShouldClose(mWindow); }
        //else       { warnln("Attempted to close an already closed window!"); }
    }
    void Window::Terminate() {
        if(mValid) {
            mValid = false;
            glfwDestroyWindow(mWindow);
        }
        //else { warnln("Attempted to terminate an already closed window!"); }
    }

}