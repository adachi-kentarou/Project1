/**
* @file GLFWEW.h
*/
#ifndef GLFWEW_H_INCLUDED
#define GLFWEW_H_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GamePad.h"
#include "Texture.h"

namespace GLFWEW {

	/**
	* GLFW��GLEW�̃��b�p�[�N���X.
	*/
	class Window
	{
	public:
		static Window& Instance();
		bool Init(int w, int h, const char* title);
		bool ShouldClose() const;
		void SwapBuffers() const;

		void viewPort() const;
		
		const GamePad& GetGamePad() const;
		void UpdateGamePad();

		double GetMouseWheel() { return mouseWheel; }

		
	private:
		Window();
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool isGLFWInitialized;
		bool isInitialized;
		GLFWwindow* window;

		GamePad gamepad;

		double mouseWheel;
	};



} // namespace GLFWEW

#endif // GLFWEW_H_INCLUDED