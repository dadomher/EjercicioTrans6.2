//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include <cmath>
#include <SOIL.h>
#include "shaderPractica.hpp"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h";
#include "Model.h"
#include "Object.h"

using namespace std;
using namespace glm;

//ancho y alto pantalla
const GLint WIDTH = 800, HEIGHT = 600;
// ancho y alto de las texturas
int width, height;
//Variable con las que controlo la rotacion de los cubos 
float xRot = 0.0f, yRot = 0.0f;
GLfloat fov = 45.0f;
Camera c1;
Model m1;
//variables donde calculamos y almacenamos los valores de posicion, direccion de la camara
vec3 cPos = glm::vec3(0.0f, 0.0f, 3.0f);
vec3 cFront = glm::vec3(0.0f, 0.0f, -1.0f);
vec3 cUp = glm::vec3(0.0f, 1.0f, 0.0f);
// velocidad de la posicion de la camara
GLfloat cSpeed = 0.01f; 
GLfloat sensivity = 0.5f;

Object obj1, obj2;

int main();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main() {
	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	GLFWwindow* window;

	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		return NULL;
	}

	//set functions when callback
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //escondemos y impedimos que el cursor pueda abandonar la pantalla
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	glfwSetKeyCallback(window, key_callback);

	//set windows and viewport0
	int screenWithd, screenHeigtht;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeigtht);

	//origen de la camara, dimensiones de la ventana
	glViewport(0, 0, screenWithd, screenHeigtht);

	//fondo
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//cargamos los shader
	Shader sh = Shader("./src/c1Fragment.vertexshader", "./src/c1Fragment.fragmentshader");
	Shader sh2 = Shader("./src/c2Fragment.vertexshader", "./src/c2Fragment.fragmentshader");

	obj1 = Object(vec3(0.5f), vec3(0.0f), vec3(0.0f), Object::cube);
	obj2 = Object(vec3(0.1f), vec3(0.0f), vec3(0.8f), Object::cube);

	//CAMARA
	c1 = Camera(cPos, cFront, sensivity, fov);

	//Matriz de proyeccion
	mat4 proj;

	//bucle de dibujado
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		c1.DoMovement(window);

		//z Buffer
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Establecer el color de fondo
		glClearColor(0.0, 0.0, 0.0, 1.0);
		proj = perspective(radians(c1.GetFOV()), 800.0f / 600.0f, 1.0f, 100.0f);

		sh.USE();
		glUniform3f(glGetUniformLocation(sh.Program, "viewPos"),c1.cameraPos.x, c1.cameraPos.y, c1.cameraPos.z);
		glUniform3f(glGetUniformLocation(sh.Program, "lightPos"), obj2.GetPosition().x, obj2.GetPosition().y, obj2.GetPosition().z);
		glUniformMatrix4fv(glGetUniformLocation(sh.Program, "model1"), 1, GL_FALSE, glm::value_ptr(obj1.GetModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(sh.Program, "view"), 1, GL_FALSE, value_ptr(c1.LookAt()));
		glUniformMatrix4fv(glGetUniformLocation(sh.Program, "proj"), 1, GL_FALSE, value_ptr(proj));
		obj1.Draw();

		sh2.USE();
		glUniformMatrix4fv(glGetUniformLocation(sh2.Program, "model2"), 1, GL_FALSE, glm::value_ptr(obj2.GetModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(sh2.Program, "view"), 1, GL_FALSE, value_ptr(c1.LookAt()));
		glUniformMatrix4fv(glGetUniformLocation(sh2.Program, "proj"), 1, GL_FALSE, value_ptr(proj));
		obj2.Draw();

		//intercambia el framebuffer
		glfwSwapBuffers(window);
		//comprueba que algun disparador se halla activado (tales como el teclado, raton, etc)
		glfwPollEvents();
	}
	obj1.Delete();
	obj2.Delete();

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//dar un movimiento al cubo
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) obj1.Move(obj1.GetPosition() + vec3(0.1f, 0.0f, 0.0f));
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) obj1.Move(obj1.GetPosition() + vec3(-0.1f, 0.0f, 0.0f));
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) obj1.Move(obj1.GetPosition() + vec3(0.0f, 0.1f, 0.0f));
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) obj1.Move(obj1.GetPosition() + vec3(0.0f, -0.1f, 0.0f));

	//dar una rotacion al cubo
	if (key == GLFW_KEY_KP_6 && (action == GLFW_PRESS || action == GLFW_REPEAT)) obj1.Rotate(obj1.getRotate() + vec3(0.0f, 1.0f, 0.0f));
	if (key == GLFW_KEY_KP_4 && (action == GLFW_PRESS || action == GLFW_REPEAT)) obj1.Rotate(obj1.getRotate() + vec3(0.f, -1.0f, 0.0f));
	if (key == GLFW_KEY_KP_8 && (action == GLFW_PRESS || action == GLFW_REPEAT)) obj1.Rotate(obj1.getRotate() + vec3(-1.0f, 0.f, 0.0f));
	if (key == GLFW_KEY_KP_2 && (action == GLFW_PRESS || action == GLFW_REPEAT)) obj1.Rotate(obj1.getRotate() + vec3(1.0f, 0.f, 0.0f));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	c1.MouseMove(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	//utilizamos el fov para determinar cuanto podemos ver de la escena y establecemos unos limites
	c1.MouseScroll(window, yoffset);
}

