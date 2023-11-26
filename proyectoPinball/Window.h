#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }

	//Funciones para apagar las luces
	bool getLuz1() { return luz1; }
	bool getLuz2() { return luz2; }
	bool getLuz3() { return luz3; }

	//Getter para nuestros Flippers
	bool getRotacionFlipper1() { return rotacionFlipper1; }
	bool getRotacionFlipper2() { return rotacionFlipper2; }
	bool getRotacionFlipper3() { return rotacionFlipper3; }
	GLfloat getmuevey() { return muevey; }
	GLfloat getmuevez() { return muevez; }
	bool getEncenderIsometrica() { return encenderIsometrica; };
	bool getencenderPOV() { return encenderPOV; }
	GLfloat getpressU() { return pressU; }
	GLfloat getpressH() { return pressH; }
	GLfloat getpressJ() { return pressJ; }
	GLfloat getpressK() { return pressK; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	bool getAnimarResorte() { return animarResorte; }
	bool getresorte() { return resorte; }
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	

	bool luz1, luz2, luz3;

	//Booleanos para nuestros Flippers
	bool rotacionFlipper1 = false;
	bool rotacionFlipper2 = false;
	bool rotacionFlipper3 = false;

	GLfloat muevex = 0.0f;
	GLfloat muevey = 0.0f;
	GLfloat muevez = 0.0f;
	
	GLfloat pressU, pressH, pressJ, pressK;
	bool mouseFirstMoved;
	//Este booleano, nos ayudara para mantener la bandera activar cada que se desea
	bool encenderIsometrica = false;
	bool encenderPOV = false;
	bool animarResorte = false;
	bool resorte = false;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
	static void ManejaClick(GLFWwindow* window, int click, int action, int mode);
};

