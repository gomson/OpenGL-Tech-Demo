#ifndef OMNIDIRECTION_SHADOW
#define OMNIDIRECTION_SHADOW

#include "ResourceManager.h"
#include "SceneModels.h"
#include "shader.h"
#include "model.h"
#include "Math\vector3.h"
#include "camera.h"
#include <SOIL\SOIL.h>
#include <vector>
#include <GL/glext.h>

// TODO(Darren): Remove GLM.
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class OmnidirectionalShadowDemo
{
public:
	OmnidirectionalShadowDemo();
	~OmnidirectionalShadowDemo();
	
	bool initalizeScene;
	bool moveLight, renderLight;

	void Initalize();
	void Update(Camera &camera, GLsizei screenWidth, GLsizei screenHeight);

private:
	GLuint textureID;
	glm::vec3 lightPos;
	Shader *shaderPointShadows, *shaderPointShadowsDepth, *shaderLightBox, *shaderCubeMap;
	GLuint woodTextureID, wallTextureID;
	Model *modelPlatform, *modelColumn, *modelBunny;
	GLuint depthMapFBO, depthCubemap;
	const GLuint ShadowWidth, ShadowHeight;

	void RenderScene(Shader &shader);
	void CreateCubeMap();
};

#endif