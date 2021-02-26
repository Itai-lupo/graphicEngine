#include "linuxWindow.h"
#include "logger.h"
#include "events.h"

#include <string>

namespace raftelGraphicEngine {
	
	static bool s_GLFWInitialized = false;

	void WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowResizeData *eventData = new WindowResizeData(width, height, window);
		eventManger::trigerEvent(events::WindowResize, eventData, window);
		std::string msg = "WindowSizeCallback " + std::to_string(width) + ", " + std::to_string(height);
		logger::LogInfo(msg);
	}

	void WindowCloseCallback(GLFWwindow* window)
	{
		std::string msg = "WindowCloseCallback";
		logger::LogInfo(msg);

	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		KeyData *eventData = new KeyData(key, scancode, mods, window);

		eventData->eventType = (events)(
			(action == GLFW_PRESS) * events::KeyPressed + 
			(action == GLFW_RELEASE) * events::KeyReleased + 
			(action == GLFW_REPEAT) * events::KeyRepeat);

		eventManger::trigerEvent(eventData->eventType, eventData, window);

		std::string msg = "KeyCallback " + std::to_string(eventData->eventType) + ", " + (char)key;
		logger::LogInfo(msg);

	}

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		mouseClickData *eventData = new mouseClickData(button, mods, window);

		eventData->eventType = (events)(
			(action == GLFW_PRESS) * events::MouseButtonPressed + 
			(action == GLFW_RELEASE) * events::MouseButtonReleased
		);

		eventManger::trigerEvent(eventData->eventType, eventData, window);
		std::string msg = "MouseButtonCallback";
		logger::LogInfo(msg);
	}

	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		mouseScrollData *eventData = new mouseScrollData(xOffset, yOffset, window);

		eventManger::trigerEvent(events::MouseScrolled, eventData, window);

		std::string msg = "ScrollCallback";
		logger::LogInfo(msg);

	}

	void CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
	{
		mouseMoveData *eventData = new mouseMoveData(xPos, yPos, window);
		eventManger::trigerEvent(events::MouseMoved, eventData, window);

		std::string msg = "CursorPosCallback";
		logger::LogInfo(msg);
	
	}

	GLFWwindow* linuxWindow::Init(linuxWindow data)
	{

		std::string msg = "Creating window " +   data.Title + ", (" +   std::to_string(data.Width) + ", " +   std::to_string(data.Height) + ") ";
		logger::LogInfo(msg);
		

		if (!s_GLFWInitialized)
		{
			glfwSetErrorCallback([](int error, const char* description){ 
				std::string msg =  "glfw error: error code: " + std::to_string(error) + "\t error message" + description;
				logger::LogError(msg);
			});

			int success = glfwInit();

            std::string msg =  "Could not intialize GLFW!" + std::to_string(success);
			logger::condtionLogFatal(msg, success != GLFW_TRUE);

			s_GLFWInitialized = true;
		}

		GLFWwindow* Window = glfwCreateWindow((int)data.Width, (int)data.Height, data.Title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(Window);
		glfwSetWindowUserPointer(Window, &data);

		glfwSetWindowSizeCallback(Window, WindowSizeCallback);
		glfwSetWindowCloseCallback(Window, WindowCloseCallback);
		glfwSetKeyCallback(Window, KeyCallback);
		glfwSetMouseButtonCallback(Window, MouseButtonCallback);
		glfwSetScrollCallback(Window, ScrollCallback);
		glfwSetCursorPosCallback(Window, CursorPosCallback);

		return Window;
	}


	void linuxWindow::Shutdown(GLFWwindow* Window)
	{
		glfwDestroyWindow(Window);
	}

	void linuxWindow::onUpdate(linuxWindow data)
	{
		glfwMakeContextCurrent(data.Window);
		
		glfwSwapBuffers(data.Window);
        glfwPollEvents();
	}

	bool linuxWindow::isOpen(GLFWwindow* window)
	{
		return !glfwWindowShouldClose(window);
	}
}