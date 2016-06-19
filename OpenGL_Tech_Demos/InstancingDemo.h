#ifndef INSTANCING_DEMO_H
#define INSTANCING_DEMO_H

#include "shader.h"
#include "model.h"
#include "camera.h"
#include "Math\matrix4.h"
#include <GL/glext.h>

class InstancingDemo
{
public:
	InstancingDemo();
	~InstancingDemo();

	void InitalizeScene();
	void Update(Camera &camera, GLsizei screenWidth, GLsizei screenHeight);

private:
	Model modelRock, modelRuins;
	Shader shaderRock, shaderGrass, shaderDirtGround;
	Matrix4 *modelMatrices;
	const GLuint amount;
	GLuint grassTexture, groundTexture;
	GLuint quadVAO, quadVBO;
	GLuint planeVAO, planeVBO;

	void SetUpBuffers(GLuint &vao, Matrix4 *matrices, GLuint vbo = NULL, int sizeOfVertices = NULL, GLfloat *vertices = NULL);
	void RenderGrassInstanceQuad();
	void RenderPlane();
	GLuint LoadTexture(GLchar *path, GLboolean alpha);
};

#endif