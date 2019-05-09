#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void updateExplosionShader();
	void Skybox();
	void setReflection();
	void setBlinnPhong();


	//void setUniform(Shader currentShader, const char *name, float x, float y, float z);
	//void setUniform(Shader currentShader, const char *name, const glm::vec3 & v);
	//void setUniform(GLuint currentShader, glm::vec4 values, const GLchar *uniformName, const GLchar *uniformVarialbeName);
	//void setUniform(Shader currentShader, const char *name, const glm::mat4 & m);
	//void setUniform(Shader currentShader, const char *name, const glm::mat3 & m);
	//void setUniform(Shader currentShader, const char *name, float val);
	//void setUniform(Shader currentShader, const char *name, int val);
	//void setUniform(Shader currentShader, const char *name, bool val);

	Display _gameDisplay;
	GameState _gameState;
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Texture texture;
	Texture texture1;
	Texture skybox;
	Shader shaderExplosion;
	Shader shaderSkybox;
	Shader shaderReflection;
	Shader shaderBlinnPhong;

	glm::mat4 modelView;
	GLuint skyboxVAO, skyboxVBO, cubemapTexture;
	vector<std::string> faces;

	Camera myCamera;

	float counter;

};

