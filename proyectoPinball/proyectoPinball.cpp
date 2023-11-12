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

	bananas = Model();
	bananas.LoadModel("Models/bananaTex.obj");
	traga = Model();
	traga.LoadModel("Models/tragamonedas.obj");
	bola = Model();
	bola.LoadModel("Models/bolaPac.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	Pinball = Model();
	Pinball.LoadModel("Models/tableroPinball.obj");
	Pinballmesa = Model();
	Pinballmesa.LoadModel("Models/maquinaPinball.obj");
	Palanca = Model();
	Palanca.LoadModel("Models/palanca.obj");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.4f, 0.4f,
		-1.0f, 0.0f, -1.0f);
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
		//model = glm::rotate(model,4 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pinball.RenderModel();

		//mesa de pinball
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pinballmesa.RenderModel();

		//Palanca
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.5f, 100.0f, 100.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Palanca.RenderModel();

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
			//-----------------------------------------------------------------------------

			glDisable(GL_BLEND);

			glUseProgram(0);

			mainWindow.swapBuffers();
		
	}
		return 0;
	}


