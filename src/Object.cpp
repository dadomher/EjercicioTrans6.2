#include "Object.h"

Object::Object() {

}

Object::Object(vec3 scale, vec3 rotation, vec3 position, FigureType typef){
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;

	GLfloat VertexBufferObject[]= {
		//front
		1.0f ,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
		1.0f , -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
		1.0f ,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
		//back
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		1.0f , -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		1.0f ,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 

		1.0f ,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		//left
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		//right
		1.0f , -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		1.0f ,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		1.0f ,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 

		1.0f ,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f , -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f , -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		//down
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 
		1.0f , -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		1.0f , -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		1.0f , -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		//up
		1.0f ,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		1.0f ,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		1.0f ,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f
	};

	//reservar memoria para el VAO, VBO y EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Establecer el objeto
	glBindVertexArray(VAO);
	//Declarar el VBO y el EBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Enlazar el buffer con openGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_STATIC_DRAW);

	//Establecer las propiedades de los vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0); //POSICION ARRAY, Nº VALORES, LARGO TOTAL VERTICES, VALOR QUE EMPIEZA EN EL VECTOR
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//liberar el buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//liberar el buffer de vertices
	glBindVertexArray(0);
}


Object::~Object(){}

void Object::Draw() {
	//pitar el VAO
	glBindVertexArray(VAO);

	//pintar con triangulos
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

void Object::Move(vec3 translation) {
	this->position = translation;
}

void Object::Rotate(vec3 rota) {
	this->rotation = rota;
}

vec3 Object::getRotate() {
	return rotation;
}

void Object::Scale(vec3 scal){
	this->scale = scal;
}

mat4 Object::GetModelMatrix() {
	mat4 funcTransModel, funcRotModel, funcScalModel, funcFinalModel;

	funcTransModel = translate(funcTransModel, position);

	funcRotModel = glm::rotate(funcRotModel, radians(rotation[0]), vec3(1.0f, 0.0f, 0.0f));
	funcRotModel = glm::rotate(funcRotModel, radians(rotation[1]), vec3(0.0f, 1.0f, 0.0f));
	funcRotModel = glm::rotate(funcRotModel, radians(rotation[2]), vec3(0.0f, 0.0f, 1.0f));

	funcScalModel = glm::scale(funcScalModel, scale);
	funcFinalModel = funcTransModel * funcRotModel * funcScalModel;

	return funcFinalModel;
}

vec3 Object::GetPosition() {
	return position;
}

void Object::Delete() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}