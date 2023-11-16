/*
Semestre 2024-1
Animaci�n:
Sesi�n 1:
Simple o b�sica:Por banderas y condicionales (m�s de 1 transformaci�n geom�trica se ve modificada
Sesi�n 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//+++++++++++++++++++++++++++++++	variables para animaci�n	+++++++++++++++++++++++++++++++
//variables para keyframes
float reproAni, habiAni, guardoFrame, reiniFrame, ciclo, ciclo2, contador = 0;	
GLfloat giroC;

Model BeppiClown;
Model BaseBeppi;
Model CuerpoBeppi;
Model IzqBeppi;
Model DerBeppi;
Model ManoIzqBeppi;
Model ManoDerBeppi;
Model CuelloBeppi;
Model CabezaBeppi;
Model Ojo1Beppi;
Model Ojo2Beppi;
float rot1;
float rot2;
float estado;
float estado2;
bool mov = false;
	

//---------------------------------------------------------------------------------------------

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
//+++++++++++++++++++++++++++++++	variables para texturas	+++++++++++++++++++++++++++++++


//---------------------------------------------------------------------------------------------

//+++++++++++++++++++++++++++++++	variables para modelos	+++++++++++++++++++++++++++++++


Model Pinball;
Model Pinballmesa;
Model Palanca;
Model bananas;
Model traga;
Model bola;
Model canica2;
Model flag;
Model tambor;
Model caliz;
Model flipper1, flipper2, flipper3;
Model pacman;

Model PacmanC;
Model PacmanBrazoD;
Model PacmanBrazoI;
Model PacmanPiernaD;
Model PacmanPiernaI;


Model resorte;
//---------------------------------------------------------------------------------------------
Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//+++++++++++++++++++++++++ Funciones para animacion +++++++++++++++++++++++++++

//funci�n para teclado de keyframes 
void inputKeyframes(bool* keys);

//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

//------------------------------------------------------------------------------

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

//++++++++++++++++++++++++++++++ Animacion Keyframes ++++++++++++++++++++++++++++++++++
bool animacion = false;

//NEW// Keyframes
float posX = 8.0, posY = 108.0, posZ = 85.0;
float	mov_x = 0.0f, mov_z = 0.0f;
float giroAvion = 0;

#define MAX_FRAMES 100
int i_max_steps = 1000;
int i_curr_steps = 21;
typedef struct _frame {
	float mov_x;
	float mov_z;
	float mov_xInc;
	float mov_zInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES]; //Arreglo de frames
int FrameIndex = 21;
bool play = false;
int playIndex = 0;

void resetElements(void){
	mov_x = KeyFrame[0].mov_x;
	mov_z = KeyFrame[0].mov_z;
}
void interpolation(void){
	//Reset con letra N
	KeyFrame[playIndex].mov_xInc = (KeyFrame[playIndex + 1].mov_x - KeyFrame[playIndex].mov_x) / i_max_steps;
	KeyFrame[playIndex].mov_zInc = (KeyFrame[playIndex + 1].mov_z - KeyFrame[playIndex].mov_z) / i_max_steps;
}
void animate(void){
	//Movimiento del objeto con M
	if (play){
		if (i_curr_steps >= i_max_steps) //Calcular un frame siguiente
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2){
				printf("Frame index= %d\n", FrameIndex);
				printf("termino la animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Interpolaci�n del pr�ximo cuadro
			{
				i_curr_steps = 0; //Resetea contador
				interpolation();
			}
		}
		else
		{
			mov_x += KeyFrame[playIndex].mov_xInc;
			mov_z += KeyFrame[playIndex].mov_zInc;
			giroC -= 10; //Giro de la canica
			i_curr_steps++;
		}
	}
}
//-------------------------------------------------------------------------------------

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(8.0f, 95.0f, 115.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	PacmanC = Model();
	PacmanC.LoadModel("Models/pacmanCuerpo.obj");
	PacmanBrazoD = Model();
	PacmanBrazoD.LoadModel("Models/pacmanBrazo.obj");
	PacmanBrazoI = Model();
	PacmanBrazoI.LoadModel("Models/pacmanBrazoI.obj");
	PacmanPiernaD = Model();
	PacmanPiernaD.LoadModel("Models/pacmanPiernaD.obj");
	PacmanPiernaI = Model();
	PacmanPiernaI.LoadModel("Models/pacmanPiernaI.obj");

	bananas = Model();
	bananas.LoadModel("Models/bananaTex.obj");
	traga = Model();
	traga.LoadModel("Models/tragamonedas.obj");
	bola = Model();
	bola.LoadModel("Models/bolaPac.obj");
	Pinball = Model();
	Pinball.LoadModel("Models/tableroPinball.obj");
	Pinballmesa = Model();
	Pinballmesa.LoadModel("Models/maquinaPinball.obj");
	Palanca = Model();
	Palanca.LoadModel("Models/palanca.obj");
	canica2 = Model();
	canica2.LoadModel("Models/canica2.obj");
	flag = Model();
	flag.LoadModel("Models/flag.obj");
	caliz = Model();
	caliz.LoadModel("Models/CalizTexturizado.obj");
	tambor = Model();
	tambor.LoadModel("Models/tambor.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/3_2.png");
	skyboxFaces.push_back("Textures/Skybox/3_4.png");
	skyboxFaces.push_back("Textures/Skybox/3_6.png");
	skyboxFaces.push_back("Textures/Skybox/3_5.png");
	skyboxFaces.push_back("Textures/Skybox/3_1.png");
	skyboxFaces.push_back("Textures/Skybox/3_3.png");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Luz de avatar
	pointLights[0] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		-30.0f, 98.0f, 40.0f,
		0.1f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;


	// Luz que ilumina todo el tablero
	spotLights[1] = SpotLight(1.0f, 0.851f, 0.729f,
		1.0f, 1.0f,
		10.0f, 115.0f, -55.0f,
		-2.5f, -1.0f, 4.0f,
		0.8f, 0.001f, 0.00009f,
		38.0f);
	spotLightCount++;


	//Luz flippers
	spotLights[2] = SpotLight(1.0f, 0.0f, 0.0f,
		2.0f, 4.0f,
		-26.0f, 100.0f, 96.0f,
		0.0f, 0.0f, -3.0f,
		0.8f, 0.2f, 0.0f,
		40.0f);
	spotLightCount++;

	//Hacemos el segundo arreglo de luces spotlight para poder apagar la luz del tablero
	spotLights2[0] = spotLights[0];		//la luz de la linterna se qudea igual
	spotLights2[1] = spotLights[2];		//la luz de flippers ahora se encuentra en la penultima posicion
	spotLights2[2] = spotLights[1];		//la luz del tablero se encuentra en el ultimo lugar del arreglo

	unsigned int spotLightCount2 = spotLightCount;	//contador para el segundo arreglo

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	//+++++++++++++++++++++++++++++++	variables para inicializar	+++++++++++++++++++++++++++++++
	pacman = Model();
	pacman.LoadModel("Models/can.obj");
	//(-18.476280,200.545273,158.405319) --- (-90.000000, -27.500000)
	//valores para la camara isometrica
	float destinoX = -20.0f;
	float destinoY = 200.0f;
	float destinoZ = 158.0f;

	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQU�
	KeyFrame[0].mov_z = -5.0f;	//1 - Movimiento por el canal principal
	KeyFrame[1].mov_z = -20.0f;	//2
	KeyFrame[2].mov_z = -40.0f;	//3
	KeyFrame[3].mov_z = -60.0f;	//4
	KeyFrame[4].mov_z = -80.0f;	//5
	KeyFrame[5].mov_z = -100.0f;//6
	KeyFrame[6].mov_x = -2.0f;	//7 - Curva
	KeyFrame[6].mov_z = -110.0f;
	KeyFrame[7].mov_x = -8.0f;	//8
	KeyFrame[7].mov_z = -118.0f;
	KeyFrame[8].mov_x = -20.0f;	//9
	KeyFrame[8].mov_z = -125.0f;
	KeyFrame[9].mov_x = -25.0f;	//10
	KeyFrame[9].mov_z = -125.0f;
	KeyFrame[10].mov_x = -35.0f;//11 - Rebote con pared
	KeyFrame[10].mov_z = -115.0f;
	KeyFrame[11].mov_x = -30.0f;//12 - Rebote con bumper
	KeyFrame[11].mov_z = -108.0f;
	KeyFrame[12].mov_x = -40.0f;//13 - Rebote con pared
	KeyFrame[12].mov_z = -90.0f;
	KeyFrame[13].mov_x = -31.0f;//14 - Rebote con bumper
	KeyFrame[13].mov_z = -70.0f;
	KeyFrame[14].mov_x = -35.0f;//15 - Rebote con posible flipper
	KeyFrame[14].mov_z = -25.0f;
	KeyFrame[15].mov_x = -13.0f;//16 - Rebote Caliz CupHead
	KeyFrame[15].mov_z = -58.0f;
	KeyFrame[16].mov_x = -47.0f;//17 - Rebote con pared
	KeyFrame[16].mov_z = -42.0f;
	KeyFrame[17].mov_x = -48.0f;//18 - Caida a carril
	KeyFrame[17].mov_z = -16.0f;
	KeyFrame[18].mov_x = -44.0f;//19
	KeyFrame[18].mov_z = -16.0f;
	KeyFrame[19].mov_x = -40.0f;//20 - Movimiento por el canal de regreso
	KeyFrame[19].mov_z = -5.0f;
	KeyFrame[20].mov_x = 0.0f;	//21 - Posici�n inicial 
	KeyFrame[20].mov_z = 0.0f;
	rot1 = 0.0f;
	rot2 = 0.0f;
	mov = true;
	estado = 0;
	estado2 = 0;

	printf("\nTeclas para uso de Keyframes:\n1.-Presionar M para reproducir animacion por KeyFrame\n2.-Presionar N para volver a habilitar la reproduccion de la animacion por KeyFrame\n");
	flipper1 = Model();
	flipper1.LoadModel("Models/flipper0.obj");
	flipper2 = Model();
	flipper2.LoadModel("Models/flipper0.obj");
	flipper3 = Model();
	flipper3.LoadModel("Models/flipper0.obj");
	//(-18.476280,200.545273,158.405319) --- (-90.000000, -27.500000)
	float rotacionFlipper1 = 0.0f;
	float rotacionFlipper2 = 0.0f;
	float rotacionFlipper3 = 0.0f;
	float velocidadRotacionFlipper = 3.0f;
	float destinoX_vista = -90.0f;
	float destinoY_vista = -27.5f;

	float posicionX; // Puedes cambiar este valor a cualquier n�mero
	float posicionY; // Puedes cambiar este valor a cualquier n�mero
	float posicionZ;  // Puedes cambiar este valor a cualquier n�mero
	
	float posicionX_vista; // Puedes cambiar este valor a cualquier n�mero
	float posicionY_vista; // Puedes cambiar este valor a cualquier n�mero
	float posicionZ_vista;  // Puedes cambiar este valor a cualquier n�mero

	float incremento = 0.01f;
	float incremento_vista = 0.01f;
	float auxiliar;
	float retardo = 0.0;
	bool segundoSkybox = true, tercerSkybox = false, cuartoSkybox = false, primerSkybox = false;
	float oscurecer = 0.0f;
	float retardo3 = 0.0f;
	BeppiClown = Model();
	BeppiClown.LoadModel("Models/BC.obj");
	BaseBeppi = Model();
	BaseBeppi.LoadModel("Models/BaseBappy.obj");
	CuerpoBeppi = Model();
	CuerpoBeppi.LoadModel("Models/CuerpoBappy.obj");
	IzqBeppi = Model();
	IzqBeppi.LoadModel("Models/BrazoIzqBappy.obj");
	ManoIzqBeppi = Model();
	ManoIzqBeppi.LoadModel("Models/ManoIzqBeppi.obj");
	DerBeppi = Model();
	DerBeppi.LoadModel("Models/BrazoDerBappy.obj");
	ManoDerBeppi = Model();
	ManoDerBeppi.LoadModel("Models/ManoDerBeppi.obj");
	CuelloBeppi = Model();
	CuelloBeppi.LoadModel("Models/CuelloBeppi.obj");
	CabezaBeppi = Model();
	CabezaBeppi.LoadModel("Models/CabezaBeppi.obj");
	Ojo1Beppi = Model();
	Ojo1Beppi.LoadModel("Models/OjoIzqBeppi.obj");
	Ojo2Beppi = Model();
	Ojo2Beppi.LoadModel("Models/OjoDerBeppi.obj");

	float incrementoAngTorsoBeppi = 0.01f, incrementoAngBrazoBeppi = 0.01f, incrementoAngManosBeppi = 0.01f, incrementoAngCabezaBeppi = 0.01f, incrementoAngOjosBeppi = 0.01f;
	float angTorsoBeppi = 0.0, angBrazoBeppi = 0.0f, angManosBeppi = 0.0f, angCabezaBeppi = 0.0f, angOjosBeppi = 0.0f, levantarse = 0.0f;
	float animacionResorte = 0.0f, incremento_animacionResorte = 0.0f, escalarResorte =0.0f;
	bool activarse = true, termino;

	resorte = Model();
	resorte.LoadModel("Models/resorte.obj");
	float comprimirResorte = 0.0f, comprimir = 1.0f, velocidadComprimir = 0.03;
	//---------------------------------------------------------------------------------------------
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//+++++++++++++ Para Keyframes +++++++++++++++++++++++
		inputKeyframes(mainWindow.getsKeys());
		animate();

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		spotLights2[0].SetFlash(lowerLight, camera.getCameraDirection());


		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelauxPacman(1.0);
		glm::mat4 modelaux2(1.0);
		glm::mat4 modelaux3(1.0);
		glm::mat4 modelaux4(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		//+++++++++++++++++++++++++++++++	PROYECTO	+++++++++++++++++++++++++++++++
		//Tragamonedas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 106.0f, -30.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		traga.RenderModel();
		
		//Tablero de pinball
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 14.0f, -9.7f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//model = glm::rotate(model, 4 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pinball.RenderModel();
		/*Flipper 01*/
		//Animaci�n
		if (mainWindow.getRotacionFlipper1()) {
			rotacionFlipper1 += (rotacionFlipper1 < 90.0f) ? velocidadRotacionFlipper * deltaTime : 0.0f;
		}
		else {
			rotacionFlipper1 -= (rotacionFlipper1 > 0.0f) ? velocidadRotacionFlipper * deltaTime : 0.0f;
		}
		//Instancia del dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 106.0f, 20.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -rotacionFlipper1 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		flipper1.RenderModel();

		//mesa de pinball
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pinballmesa.RenderModel();

		//Palanca Pinball
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.5f, 100.0f, 100.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Palanca.RenderModel();
		/*Flipper 02*/
		//Animaci�n
		if (mainWindow.getRotacionFlipper2()) {
			rotacionFlipper2 += (rotacionFlipper2 < 90.0f) ? velocidadRotacionFlipper * deltaTime : 0.0f;
		}
		else {
			rotacionFlipper2 -= (rotacionFlipper2 > 0.0f) ? velocidadRotacionFlipper * deltaTime : 0.0f;
		}
		//Instancia del dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 106.0f, 60.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -rotacionFlipper2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		flipper1.RenderModel();
		

		/*Flipper 03*/
		//Animaci�n
		if (mainWindow.getRotacionFlipper3()) {
			rotacionFlipper3 += (rotacionFlipper3 < 90.0f) ? velocidadRotacionFlipper * deltaTime : 0.0f;
		}
		else {
			rotacionFlipper3 -= (rotacionFlipper3 > 0.0f) ? velocidadRotacionFlipper * deltaTime : 0.0f;
		}
		//Instancia del dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 106.0f, 60.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -rotacionFlipper3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		flipper1.RenderModel();

		//Canica
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posX + mov_x, posY, posZ + mov_z));
		model = glm::rotate(model, giroC * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		canica2.RenderModel();

		//Obstaculo centro - "original"
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 106.0f, 15.0));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tambor.RenderModel();

		//Cuerpo pacman
		model = glm::mat4(1.0);
		if(8.0f - mainWindow.getmuevex()>-47.720284 && 8.0f - mainWindow.getmuevex()< 11.034455 && 85.0f - mainWindow.getmuevez() < 87.629875 && 85.0f - mainWindow.getmuevez() > -58.037342)//(-47.720284,107.282341,88.727539) --- (11.034455,106.301315,87.629875) ---(12.556218,106.922478,-58.037342)
			model = glm::translate(model, glm::vec3(8.0f - mainWindow.getmuevex(), 110.0f, 85.0f - mainWindow.getmuevez()));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		//Casos para girar a pacman en alguna direccion
		if (mainWindow.getpressU() == true)
		{
			model = glm::rotate(model, 1 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			
		}

		if (mainWindow.getpressH() == true)
		{
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (mainWindow.getpressJ() == true)
		{
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (mainWindow.getpressK() == true)
		{
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		modelauxPacman = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PacmanC.RenderModel();

		//Brazo Derecho
		model = modelauxPacman;
		model = glm::translate(model, glm::vec3(-1.5f, 0.5f, 0.0f));
		model = glm::rotate (model, rot1 * toRadians,glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PacmanBrazoD.RenderModel();

		//Brazo Izquierdo
		model = modelauxPacman;
		model = glm::translate(model, glm::vec3(2.0f, 0.5f, 0.0f));
		model = glm::rotate(model, rot2 * toRadians, glm::vec3(1.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PacmanBrazoI.RenderModel();

		//Pierna Izquierda
		model = modelauxPacman;
		model = glm::translate(model, glm::vec3(1.3f, -1.5f, 0.0f));
		model = glm::rotate(model, rot1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PacmanPiernaI.RenderModel();

		//Pierna Derecha
		model = modelauxPacman;
		model = glm::translate(model, glm::vec3(-0.7f, -1.5f, 0.0f));
		model = glm::rotate(model, rot2 * toRadians, glm::vec3(1.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PacmanPiernaD.RenderModel();

		if (mainWindow.getpressU() == true || mainWindow.getpressH() == true || mainWindow.getpressJ() == true || mainWindow.getpressK() == true)
		{
			// Animacion de movimiento rotativo del brazo y pierna para simular caminata
			if (rot1 < 45.0f && estado == 0)	
			{
				rot1 += 1.2 * deltaTime;		//Rota en positivo hasta 45 grados
			}
			else
			{
				estado = 1;						//Pasamos al movimiento en reversa
			}
			if (rot1 > -70.0f && estado == 1)
			{
				rot1 -= 1.2 * deltaTime;		//Rota en negativo
			}
			else
			{
				estado = 0;						//Volvemos al primer movimiento y con esto creamos un bucle
			}

			// Segundo moviento para otro par de pierna y brazo
			if (rot2 > -70.0f && estado == 0)
			{
				rot2 -= 1.2 * deltaTime;		// Ahora empezamos rotando en negativo
			}
			else
			{
				estado2 = 1;						//Pasamos al movimiento inverso
			}
			if (rot2 < 45.0f && estado == 1)
			{
				rot2 += 1.2 * deltaTime;		// Rota en positivo
			}
			else
			{
				estado2 = 0;						// Volvemos al primer movimiento y creamos el bucle
			}
		}

		//Obstaculo Arriba centro
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -35.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tambor.RenderModel();

		//Obstaculo lado derecho (de la camara)
		model = modelaux;
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tambor.RenderModel();

		//Obstaculo lado izquierdo (de la camara)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, -25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tambor.RenderModel();

		//Flag of CupHead
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 106.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		flag.RenderModel();

		//Caliz of CupHead
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 106.0f, 26.0));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caliz.RenderModel();
		//Vista Isometrica
		/*
		�Que es una vista isometrica?
		La proyecci�n isom�trica es una forma de representaci�n visual de un objeto tridimensional en un plano bidimensional. 
		En esta, los tres ejes ortogonales principales forman �ngulos de 120 grados, y las dimensiones paralelas a esos 
		ejes se miden en una misma escala.
		*/
		if (segundoSkybox)
		{
			if (retardo >= 0.30f) {
				std::vector<std::string> skyboxFaces;
				skyboxFaces.push_back("Textures/Skybox/4_2.png");
				skyboxFaces.push_back("Textures/Skybox/4_4.png");
				skyboxFaces.push_back("Textures/Skybox/4_6.png");
				skyboxFaces.push_back("Textures/Skybox/4_5.png");
				skyboxFaces.push_back("Textures/Skybox/4_1.png");
				skyboxFaces.push_back("Textures/Skybox/4_3.png");
				skybox = Skybox(skyboxFaces);
				segundoSkybox = false;
			}
			retardo += 0.00001f;
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				1.0f - retardo, 1.0f - retardo,
				0.0f, 0.0f, -1.0f);
			tercerSkybox = true;
		}
		
		if (tercerSkybox)
		{
			if (retardo >= 0.60f) {
				std::vector<std::string> skyboxFaces;
				skyboxFaces.push_back("Textures/Skybox/2_2.png");
				skyboxFaces.push_back("Textures/Skybox/2_4.png");
				skyboxFaces.push_back("Textures/Skybox/2_6.png");
				skyboxFaces.push_back("Textures/Skybox/2_5.png");
				skyboxFaces.push_back("Textures/Skybox/2_1.png");
				skyboxFaces.push_back("Textures/Skybox/2_3.png");
				skybox = Skybox(skyboxFaces);
				tercerSkybox = false;
			}
			retardo += 0.00001f;
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				1.0f - retardo, 1.0f - retardo,
				0.0f, 0.0f, -1.0f);
			cuartoSkybox = true;
		}

		if (cuartoSkybox)
		{
			if (retardo >= 0.90f) {
				std::vector<std::string> skyboxFaces;
				skyboxFaces.push_back("Textures/Skybox/1_2.png");
				skyboxFaces.push_back("Textures/Skybox/1_4.png");
				skyboxFaces.push_back("Textures/Skybox/1_6.png");
				skyboxFaces.push_back("Textures/Skybox/1_5.png");
				skyboxFaces.push_back("Textures/Skybox/1_1.png");
				skyboxFaces.push_back("Textures/Skybox/1_3.png");
				skybox = Skybox(skyboxFaces);
				cuartoSkybox = false;
				primerSkybox = true;
				retardo = 0.0f;
			}
			retardo += 0.00001f;
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				1.0f - retardo, 1.0f - retardo,
				0.0f, 0.0f, -1.0f);
			
		}

		if (primerSkybox)
		{
			if (retardo >= 0.4f) {
				std::vector<std::string> skyboxFaces;
				skyboxFaces.push_back("Textures/Skybox/3_2.png");
				skyboxFaces.push_back("Textures/Skybox/3_4.png");
				skyboxFaces.push_back("Textures/Skybox/3_6.png");
				skyboxFaces.push_back("Textures/Skybox/3_5.png");
				skyboxFaces.push_back("Textures/Skybox/3_1.png");
				skyboxFaces.push_back("Textures/Skybox/3_3.png");
				skybox = Skybox(skyboxFaces);
				segundoSkybox = true;
				primerSkybox = false;
				retardo = 0.0f;
			}
			retardo += 0.00001f;
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				1.0f - retardo, 1.0f - retardo,
				0.0f, 0.0f, -1.0f);

		}
		
		if(mainWindow.getEncenderIsometrica()){
			//Movemos a nuestra POV hasta la posici�n destino (Manipulamos el manejo del tecladdo)
			//Asignamos en variables para sustituir por las de mi formula
			posicionX = camera.getPosicionX(); // Puedes cambiar este valor a cualquier n�mero
			posicionY = camera.getPosicionY(); // Puedes cambiar este valor a cualquier n�mero
			posicionZ = camera.getPosicionZ(); // Puedes cambiar este valor a cualquier n�mero
			/*Lo que se hace basicamente en las variables auxiliar, es obtener el incremento( o decremento, segun el caso)
			para alcanzar los valores destinos que fueron planteados*/
			auxiliar = (posicionX > destinoX) ? posicionX - incremento : posicionX + incremento;
			//Asignamos la nueva posici�n de X, obtenida por nuestro auxiliar
			camera.setPosicionX(auxiliar);
			auxiliar = (posicionY > destinoY) ? posicionY - incremento : posicionY + incremento;
			//Asignamos la nueva posici�n de Y, obtenida por nuestro auxiliar
			camera.setPosicionY(auxiliar);
			//Asignamos la nueva posici�n de Z, obtenida por nuestro auxiliar
			auxiliar = (posicionZ > destinoZ) ? posicionZ - incremento : posicionZ + incremento;
			camera.setPosicionZ(auxiliar);

			////Movemos a nuestra POV hasta la vista destino (Manipulamos el manejo del mouse)
			posicionX_vista = camera.getVistaX();
			posicionY_vista = camera.getVistaY();

			auxiliar = (posicionX_vista > destinoX_vista) ? posicionX_vista - incremento_vista : posicionX_vista + incremento_vista;
			camera.setVistaX(auxiliar);
			auxiliar = (posicionY_vista > destinoY_vista) ? posicionY_vista - incremento_vista : posicionY_vista + incremento_vista;
			camera.setVistaY(auxiliar);
			//std::cout << "Eje X: " << posicionX_vista << "\tEje Y: " << posicionY_vista << "\n";
		}

		////Instancia del coche 
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 1.0f, 0.0f + mainWindow.getmuevez()));
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, 1* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//pacman.RenderModel();

		if (mainWindow.getencenderPOV()) {
			if (retardo3 <= 5.0f) {
				//Movemos a nuestra POV hasta la posici�n destino (Manipulamos el manejo del tecladdo)
			//Asignamos en variables para sustituir por las de mi formula
				posicionX = camera.getPosicionX(); // Puedes cambiar este valor a cualquier n�mero
				posicionY = camera.getPosicionY(); // Puedes cambiar este valor a cualquier n�mero
				posicionZ = camera.getPosicionZ(); // Puedes cambiar este valor a cualquier n�mero
				/*Lo que se hace basicamente en las variables auxiliar, es obtener el incremento( o decremento, segun el caso)
				para alcanzar los valores destinos que fueron planteados*/
				auxiliar = (posicionX > mainWindow.getmuevex() - 10) ? posicionX - incremento : posicionX + incremento;
				//Asignamos la nueva posici�n de X, obtenida por nuestro auxiliar
				camera.setPosicionX(auxiliar);
				auxiliar = (posicionY > 10.0f) ? posicionY - incremento : posicionY + incremento;
				//Asignamos la nueva posici�n de Y, obtenida por nuestro auxiliar
				camera.setPosicionY(auxiliar);
				//Asignamos la nueva posici�n de Z, obtenida por nuestro auxiliar
				auxiliar = (posicionZ > mainWindow.getmuevez() - 10) ? posicionZ - incremento : posicionZ + incremento;
				camera.setPosicionZ(auxiliar);

				////Movemos a nuestra POV hasta la vista destino (Manipulamos el manejo del mouse)
				posicionX_vista = camera.getVistaX();
				posicionY_vista = camera.getVistaY();

				auxiliar = (posicionX_vista > 45.0f) ? posicionX_vista - incremento_vista : posicionX_vista + incremento_vista;
				camera.setVistaX(auxiliar);
				auxiliar = (posicionY_vista > -20) ? posicionY_vista - incremento_vista : posicionY_vista + incremento_vista;
				camera.setVistaY(auxiliar);
				//std::cout << "Eje X: " << posicionX_vista << "\tEje Y: " << posicionY_vista << "\n";
				retardo3 += 0.001f;
				//printf("%f\n", retardo3);
			}else {
				//Nos da el POV	0.0f + mainWindow.getmuevex(), 106.0f, 0.0f + mainWindow.getmuevez()
				camera.setPosicionX(8.0f - mainWindow.getmuevex());
				camera.setPosicionZ(85.0f - mainWindow.getmuevez()+ 10.0f);
				camera.setPosicionY(115.0f);
				camera.setVistaY(-20.0f);
				//retardo3 = 0.0f
				//;, 110.0f, 
			}
			
		}
		//Animaci�n de Beppi
		// Incrementa los �ngulos de Beppi
		angTorsoBeppi	+= incrementoAngTorsoBeppi;
		angBrazoBeppi	+= incrementoAngBrazoBeppi;
		angManosBeppi	+= incrementoAngManosBeppi;
		angCabezaBeppi	+= incrementoAngCabezaBeppi;
		angOjosBeppi	+= incrementoAngOjosBeppi;
		
		// Esto altera el incremento y decremento para que los angulos operen en un intervalo definido, para cada modelo.
		incrementoAngTorsoBeppi	= (angTorsoBeppi	>= 45.0f	|| angTorsoBeppi <= -45.0f)	? -incrementoAngTorsoBeppi	: incrementoAngTorsoBeppi;
		incrementoAngBrazoBeppi	= (angBrazoBeppi	>= 75.0f	|| angBrazoBeppi <= -45.0f)	? -incrementoAngBrazoBeppi	: incrementoAngBrazoBeppi;
		incrementoAngCabezaBeppi= (angCabezaBeppi	>= 75.0f	|| angBrazoBeppi <= -45.0f)	? -incrementoAngCabezaBeppi : incrementoAngCabezaBeppi;
		incrementoAngOjosBeppi	= (angOjosBeppi		>= 45.0f	|| angOjosBeppi	 <= -45.0f)	? -incrementoAngOjosBeppi	: incrementoAngOjosBeppi;
		//variable = (condicion) ? 1 : 0 
		//if(condicion){
		// variable = 1
		// }else{
		// variable = 0
		// }
		// 
		//Instancia de la base de Beppi
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 106.0f, 46.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseBeppi.RenderModel();

		//Instancia del torso de Beppi
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 38.0f, 0.0f));
		model = glm::rotate(model, angTorsoBeppi * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoBeppi.RenderModel();

		//Instancia del brazo izquierdo de Beppi
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-19.376f, 45.02f, -3.148f));
		model = glm::rotate(model, angBrazoBeppi * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		IzqBeppi.RenderModel();

		//Instancia de la mano izquierda de Beppi
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-43.026f, 34.532f, 18.631f));
		model = glm::rotate(model, toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManoIzqBeppi.RenderModel();

		//Instancia del brazo derecha de Beppi
		model = modelaux2;
		model = glm::translate(model, glm::vec3(15.899f, 43.462f,-3.537f));
		model = glm::rotate(model, -angBrazoBeppi * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		DerBeppi.RenderModel();

		//Instancia de la mano derecha de Beppi
		model = modelaux3;
		model = glm::translate(model, glm::vec3(43.675f, 31.866f, 23.264f));
		model = glm::rotate(model, toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManoDerBeppi.RenderModel();

		//Instancia del cuello de Beppi
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.919f, 128.161f, 3.717f));
		model = glm::rotate(model, toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux3 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuelloBeppi.RenderModel();

		//Instancia de la cabeza
		model = modelaux3;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, angCabezaBeppi * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux4 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CabezaBeppi.RenderModel();

		//Instancia de la ojo derecho
		model = modelaux4;
		model = glm::translate(model, glm::vec3(-25.003f, 62.131f, 58.961f));
		model = glm::rotate(model, (-45 + angOjosBeppi) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ojo1Beppi.RenderModel();

		//Instancia de la ojo izquierdo
		model = modelaux4;
		model = glm::translate(model, glm::vec3(21.934f, 62.643f, 59.157f));
		model = glm::rotate(model, (-45 + angOjosBeppi) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ojo2Beppi.RenderModel();
		//-----------------------------------------------------------------------------

		//Funcion para pagar las spotlight (luz de tablero y luz de flippers) de forma independente
		if (mainWindow.getLuz2() == true)
		{
			//Mandamos al shader el primer arreglo completo, la luz del los flippers esta encendida		

			//los siguientes if nos ayudan a apagar y prender la luz del tablero cuando la luz de los flippers esta prendida
			if (mainWindow.getLuz1() == true)
			{
				shaderList[0].SetSpotLights(spotLights2, spotLightCount2);			//Prendemos la luz del tablero
			}
			else
			{
				shaderList[0].SetSpotLights(spotLights2, spotLightCount2 - 1);		//Restamos el contador y asi no mandamos la luz del tablero
			}
		}
		else
		{
			// Si el valor de getluz2(), que es la luz de los flippers es falso, apagamos la luz
			shaderList[0].SetSpotLights(spotLights, spotLightCount - 1);

			//Si queremos prender la luz del tablero cuando la luz de los flippers esta apagada, no hacemos nada
			//porque el primer arreglo spotlights, solo apago la luz de los flippers, las demas si se mandaron al shader y siguen encedidadas
			if (mainWindow.getLuz1() == true)
			{
			}
			else
			{
				//Restamos el contador y asi ya no mandamos al shader la ultimas 2 luces, apagamos ambas luces
				shaderList[0].SetSpotLights(spotLights, spotLightCount - 2);
			}
		}

		//Funcion para apagar la luz del avatar

		if (mainWindow.getLuz3() == true)
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount);			//Mandamos al shader toda la lista del pointlight
		}
		else
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);		//Restamos el contador y asi ya no mandamos al shader la ultima luz
		}
			//Bola pacman izq
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.5f, 106.0f, 43.0));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			bola.RenderModel();

			//Bola pacman derecha
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-40.0f, 106.0f, 43.0));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			bola.RenderModel();

			//Bananas como bolas de pacman
			for (float i = -42.0f; i <= -4.0f; i += 8.0f) {
				model = glm::mat4(1.0f); //inferior
				model = glm::translate(model, glm::vec3(i, 106.0f, 70.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				bananas.RenderModel();
			}
			for (float i = 70.0f; i >= 43.0f; i -= 7.5f) {
				model = glm::mat4(1.0f); //lado izq camara
				model = glm::translate(model, glm::vec3(-42.0f, 106.0f, i));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				bananas.RenderModel();
			}
			for (float i = 70.0f; i >= 26.0f; i -= 7.5f) {
				model = glm::mat4(1.0f); //lado der camara
				model = glm::translate(model, glm::vec3(-0.5f, 106.0f, i));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				bananas.RenderModel();
			}
			for (float i = -33.0f; i <= -9.0f; i += 8.0f) {
				model = glm::mat4(1.0f); //arriba flippers
				model = glm::translate(model, glm::vec3(i, 106.0f, 43.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				bananas.RenderModel();
			}
			float centerX = -21.0f;  // Centro x de la elipse
			float centerY = 106.0f;  // Centro y de la elipse
			float centerZ = -2.0f;   // Centro z de la elipse
			for (float angle = 0.0f; angle <= 2.0f * glm::pi<float>(); angle += glm::pi<float>() / 3.5f) {
				//Calcula el punto en donde estara cada objeto con una distancia total de 3.5
				model = glm::mat4(1.0f);  // Reinicializa la matriz model en cada iteraci�n
				float scaleX = 12.0f;   // Escala x
				float scaleZ = 22.0f;    // Escala z
				float x = centerX + scaleX * cos(angle);
				float y = centerY;
				float z = centerZ + scaleZ * sin(angle);
				model = glm::translate(model, glm::vec3(x, y, z));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				bananas.RenderModel();
			}
			for (float angle = 0.0f; angle <= 2.0f * glm::pi<float>(); angle += glm::pi<float>() / 8.0f) {
				//Calcula el punto en donde estara cada objeto con una distancia total de 8.0
				model = glm::mat4(1.0f);  // Reinicializa la matriz model en cada iteraci�n
				float scaleX = 20.0f;   // Aumenta este valor para hacer el centro m�s ancho
				float scaleZ = 35.0f;   // Aumenta este valor para hacer el centro m�s alto
				float x = centerX + scaleX * cos(angle);
				float y = centerY;
				float z = centerZ + scaleZ * sin(angle);
				model = glm::translate(model, glm::vec3(x, y, z));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				bananas.RenderModel();
			}

			//Instancia del resorte
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(8.0f, 107.5f, 95.0f));
			modelaux = model;
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, comprimir));
			//model = glm::rotate(model, -rotacionFlipper3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			resorte.RenderModel();
			
			
			if (mainWindow.getAnimarResorte()) {
				comprimir -= (comprimir > 0.3f) ? velocidadComprimir * deltaTime : 0.0f;
			}
			else {
				
				comprimir += (comprimir < 1.0f) ? velocidadComprimir * deltaTime : 0.0f;
			}
			//-----------------------------------------------------------------------------

			glDisable(GL_BLEND);

			glUseProgram(0);

			mainWindow.swapBuffers();
		
	}
		return 0;
	}
	
void inputKeyframes(bool* keys) {
	if (keys[GLFW_KEY_M]) {
		printf("(%f,%f,%f) --- (%f, %f)\n", camera.getPosicionX(), camera.getPosicionY(), camera.getPosicionZ(), camera.getVistaX(), camera.getVistaY());
		if (reproAni < 1) {
			if (play == false && (FrameIndex > 1)) {
				resetElements();
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproAni++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animaci�n'\n");
				habiAni = 0;
				//float animacionResorte = 0.0f, incremento_animacionResorte = 0.0f;
				//incrementoAngOjosBeppi = (angOjosBeppi >= 45.0f || angOjosBeppi <= -45.0f) ? -incrementoAngOjosBeppi : incrementoAngOjosBeppi;
				//incremento_animacionResorte += ;

			}
			else {
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_N]) {
		if (habiAni < 1 && reproAni>0) {
			printf("Ya puedes reproducir de nuevo la animaci�n con la tecla de barra espaciadora'\n");
			reproAni = 0;
		}
	}
}
