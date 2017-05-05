#pragma once
#include <GL\glew.h>
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>
using namespace glm;
#include "shaderPractica.hpp"

class Object{
public:
	enum FigureType{
		cube = 0,
	};
	Object();
	Object(vec3 scale, vec3 rotation, vec3 position, FigureType typef);
	~Object();

	void Draw();
	void Move(vec3 translation);
	void Rotate(vec3 rota);
	vec3 getRotate();
	void Scale(vec3 scal);
	void Delete();
	mat4 GetModelMatrix();
	vec3 GetPosition();
	vec3 rotation;
private:
	GLuint VBO, VAO, EBO;
	vec3 position;
	vec3 scale;

};

