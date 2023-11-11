/*
Semestre 2024-1
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
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

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//+++++++++++++++++++++++++++++++	variables para animación	+++++++++++++++++++++++++++++++
//variables para keyframes
float reproAni, habiAni, guardoFrame, reiniFrame, ciclo, ciclo2, contador = 0;	
GLfloat giroC;
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
Model canica2;
Model flag;
Model tambor;
Model caliz;

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

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//+++++++++++++++++++++++++ Funciones para animacion +++++++++++++++++++++++++++

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

//cálculo del promedio de las normales para sombreado de Phong
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
int i_max_steps = 90;
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
			else //Interpolación del próximo cuadro
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

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	Pinball = Model();
	Pinball.LoadModel("Models/tableroPinball.obj");
	Pinballmesa = Model();
	Pinballmesa.LoadModel("Models/maquinaPinball.obj");
	Palanca = Model();
	Palanca.LoadModel("Models/palanca.obj");
	canica2 = Model();
	canica2.LoadModel("Models/canica2.obj");
	/*flag = Model();
	flag.LoadModel("Models/flag.obj");
	caliz = Model();
	caliz.LoadModel("Models/CalizTexturizado.obj");
	tambor = Model();
	tambor.LoadModel("Models/tambor.obj");*/

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.5f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
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

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	//+++++++++++++++++++++++++++++++	variables para inicializar	+++++++++++++++++++++++++++++++

	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ
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
	KeyFrame[20].mov_x = 0.0f;	//21 - Posición inicial 
	KeyFrame[20].mov_z = 0.0f;
	//Se agregan nuevos frames 
	printf("\nTeclas para uso de Keyframes:\n1.-Presionar M para reproducir animacion por KeyFrame\n2.-Presionar N para volver a habilitar la reproduccion de la animacion por KeyFrame\n");

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

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
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
		//Tablero de pinball
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 14.0f, -9.7f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//model = glm::rotate(model, 4 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pinball.RenderModel();

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

		//Canica
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posX + mov_x, posY, posZ + mov_z));
		model = glm::rotate(model, giroC * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		canica2.RenderModel();

		/*//Obstaculo centro - "original"
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 106.0f, 15.0));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tambor.RenderModel();

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
		caliz.RenderModel();*/

		//-----------------------------------------------------------------------------

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys) {
	if (keys[GLFW_KEY_M]) {
		if (reproAni < 1) {
			if (play == false && (FrameIndex > 1)) {
				resetElements();
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproAni++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habiAni = 0;
			}
			else {
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_N]) {
		if (habiAni < 1 && reproAni>0) {
			printf("Ya puedes reproducir de nuevo la animación con la tecla de barra espaciadora'\n");
			reproAni = 0;
		}
	}
/*
	if (keys[GLFW_KEY_L]) {
		if (guardoFrame < 1) {
			saveFrame();
			printf("movAvion_x es: %f\n", movAvion_x);
			printf("movAvion_y es: %f\n", movAvion_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reiniFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P]) {
		if (reiniFrame < 1 && guardoFrame>0) {
			guardoFrame = 0;
			printf("Ya puedes guardar otro frame presionando la tecla L'\n");
		}
	}
	if (keys[GLFW_KEY_1]) {
		if (ciclo < 1) {
			movAvion_x += 1.0f;
			printf("\n mov_x es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}
	}
	if (keys[GLFW_KEY_2]) {
		if (ciclo2 < 1 && ciclo>0) {
			ciclo = 0;
			printf("\n Ya puedes modificar tu variable presionando la tecla 1\n");
		}
	}
*/
}