#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	Mesh* mesh1();
	Mesh* mesh2();
	Mesh* mesh3();
	Audio* audioDevice();
	Texture* texture(); //load texture
	Texture* texture1(); //load texture
	Texture* skybox();
	Shader* shaderExplosion();
	Shader* shaderSkybox();
	Shader* shaderReflection();
	Shader* shaderBlinnPhong();
}

MainGame::~MainGame() 
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay();

	texture.init("..\\res\\bricks.jpg"); //load texture
	texture1.init("..\\res\\water.jpg"); //load texture1

	mesh1.loadModel("..\\res\\lamp.obj"); // loads the three diferent models
	mesh2.loadModel("..\\res\\head.obj");
	mesh3.loadModel("..\\res\\ball.obj");

	// loads all the different shaders
	shaderExplosion.initGeom("..\\res\\shaderGeoText.vert", "..\\res\\shaderGeoText.frag", "..\\res\\shaderGeoText.geom");
	shaderSkybox.init("..\\res\\shaderSkybox.vert", "..\\res\\shaderSkybox.frag");
	shaderReflection.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	shaderBlinnPhong.init("..\\res\\shaderBlinnPhong.vert", "..\\res\\shaderBlinnPhong.frag");

	//inits camaera with screen width and screen height
	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	//loads each seperate face for the skybox 
	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	cubemapTexture = skybox.loadCubemap(faces); //Load the cubemap using "faces" into cubemapTextures

	float skyboxVertices[] = {
		// positions          
		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}	
}

void MainGame::Skybox()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shaderSkybox.Bind(); // binds the shader
	shaderSkybox.setInt("skybox", 0);
	shaderSkybox.setMat4("view", myCamera.GetView()); //gets view matrix and sets it in the shader
	shaderSkybox.setMat4("projection", myCamera.GetProjection()); //gets projection matrix and sets it in the shader
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void MainGame::setReflection()
{
	shaderReflection.Bind(); // binds the shader

	// gets the model, view and projection matrices and the camera position and sets them in the shader
	shaderReflection.setMat4("model", transform.GetModel()); 
	shaderReflection.setMat4("view", myCamera.GetView());
	shaderReflection.setMat4("projection", myCamera.GetProjection());
	shaderReflection.setVec3("cameraPos", myCamera.getPos());
}

void MainGame::updateExplosionShader()
{
	shaderExplosion.Bind(); // binds the shader

	shaderExplosion.setFloat("time", 2.0 + (counter)); // sets the explosion time
	shaderExplosion.setMat4("transform", transform.GetModel()); // gets the model's position and sets it in the shader
}

void MainGame::setBlinnPhong()
{
	shaderBlinnPhong.Bind(); // binds the shader

	// gets the model and view projection matrices and the camera position and sets them in the shader
	shaderBlinnPhong.setMat4("model", transform.GetModel());
	shaderBlinnPhong.setMat4("viewProjection", myCamera.GetViewProjection());
	shaderBlinnPhong.setVec3("viewPos", myCamera.getPos());

	// sets the light position and colour and sets them in the shader
	glm::vec3 lightPos(0.0f, 2.0f, -5.0f);
	shaderBlinnPhong.setVec3("lightPos", lightPos);

	glm::vec3 lightColour(1.0, 1.0, 1.0);
	shaderBlinnPhong.setVec3("lightColour", lightColour);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f); // clears game display

	Skybox(); // draws the skybox

	// sets position, rotation and scale for the model
	transform.SetPos(glm::vec3(-sinf(counter), -1.0, -sinf(counter) * 5));
	transform.SetRot(glm::vec3(0.0, 0.0, counter));
	transform.SetScale(glm::vec3(0.03, 0.03, 0.03));

	updateExplosionShader(); // attaches the explosion shader to the model
	shaderExplosion.Update(transform, myCamera); // updates the shader based on the model's position and camera view
	texture1.Bind(0); // binds the texture to the model
	mesh1.draw(); // draws the model
	
	// sets position, rotation and scale for the model
	transform.SetPos(glm::vec3(-sinf(counter), -sinf(counter), -sinf(counter)));
	transform.SetRot(glm::vec3(1.0, counter, counter));
	transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

	setReflection(); // attaches the reflection shader to the model
	shaderReflection.Update(transform, myCamera); // updates the shader based on the model's position and camera view
	mesh2.draw();  // draws the model

	// sets position, rotation and scale for the model
	transform.SetPos(glm::vec3(sinf(counter), sinf(counter), sinf(counter)));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.8, 0.8, 0.8));

	setBlinnPhong(); // attaches the Blinn-Phong shader to the model
	shaderBlinnPhong.Update(transform, myCamera); // updates the shader based on the model's position and camera view
	texture.Bind(0);  // binds the texture to the model
	mesh3.draw(); // draws the model

	counter = counter + 0.01f; // updates the counter

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer(); // swaps the buffers
} 