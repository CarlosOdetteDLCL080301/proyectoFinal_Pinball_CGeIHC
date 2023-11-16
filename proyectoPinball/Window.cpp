#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	luz1 = false;
	luz2 = false;
	luz3 = false;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto Final: Pinball", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	
	if (key == GLFW_KEY_H)
	{
		theWindow->muevex += 1.0;
	}

	// Detecatamos si se esta presionando la tecla y cambiamos el valor booleano 
	
	//Para apagar luz del tablero
	if (key == GLFW_KEY_1)
	{
		theWindow->luz1 = true;
	}
	if (key == GLFW_KEY_2)
	{
		theWindow->luz1 = false;
	}

	//Para apagar la luz de los flippers
	if (key == GLFW_KEY_3)
	{
		theWindow->luz2 = true;
	}
	if (key == GLFW_KEY_4)
	{
		theWindow->luz2 = false;
	}
	
	//Para apagar la luz del avatar
	if (key == GLFW_KEY_5)
	{
		theWindow->luz3 = true;
	}
	if (key == GLFW_KEY_6)
	{
		theWindow->luz3 = false;
	}
	//Este boton es asignado para el Flipper 1
	if (key == GLFW_KEY_V and action == GLFW_PRESS) { theWindow->rotacionFlipper1 = true; } else { theWindow->rotacionFlipper1 = false; }
	//Este boton es asignado para el Flipper 2
	if (key == GLFW_KEY_C and action == GLFW_PRESS) { theWindow->rotacionFlipper2 = true; } else { theWindow->rotacionFlipper2 = false; }
	//Este boton es asignado para el Flipper 3
	if (key == GLFW_KEY_X and action == GLFW_PRESS) { theWindow->rotacionFlipper3 = true; } else { theWindow->rotacionFlipper3 = false; }
	//Te movera a al posici�n destino, hasta que se desea
	if (key == GLFW_KEY_K)
	{
		theWindow->muevex -= 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->muevez += 1.0;
	}
	if (key == GLFW_KEY_J)
	{
		theWindow->muevez -= 1.0;
	}

	//Te movera a al posici�n destino, hasta que se desea
	theWindow->encenderIsometrica = (key == GLFW_KEY_I and action == GLFW_PRESS) ? !(theWindow->encenderIsometrica) : theWindow->encenderIsometrica;
	theWindow->encenderPOV = (key == GLFW_KEY_0 and action == GLFW_PRESS) ? !(theWindow->encenderPOV) : theWindow->encenderPOV;
	//Sirve para saber si se estan presionando las teclas wasd
	if (key == GLFW_KEY_U)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->pressU = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->pressU = false;
		}
	}

	if (key == GLFW_KEY_H)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->pressH = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->pressH = false;
		}
	}

	if (key == GLFW_KEY_J)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->pressJ = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->pressJ = false;
		}
	}

	if (key == GLFW_KEY_K)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->pressK = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->pressK = false;
		}
	}

	if (key == GLFW_KEY_M and action == GLFW_PRESS) { theWindow->animarResorte = true; } else { theWindow->animarResorte = false; }
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
