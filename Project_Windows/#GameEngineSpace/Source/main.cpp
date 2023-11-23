#include <iostream>
#include<fstream>
#include<sstream>
#include <stdio.h>
#include<string>
#include<glew.h>
#include<glfw3.h>
#include<GL/GL.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<stb_image.h>
#include<vector>
#include<Cylinder.h>
#include<SGSprite.h>
#include<SGShader.h>
#include<SGTexture.h>
#include<SGCube.h>

using namespace std;

float colorValue = 0.0f;
bool isPressedD, isPressedA;
//Function prototype
void OnWindowResize(GLFWwindow*, int, int);
void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "SGRenderer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, OnWindowResize);
	glfwSetKeyCallback(window, ProcessInput);
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "GLEW INIT FAILED\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	//Create 7 sprite objects, each with thier own position,scale and color
	SGSprite sprite_fire(SGVector2(500 * SCALING_RATIO_X, 550 * SCALING_RATIO_Y), SGVector2(200 * SCALING_RATIO_X, 200 * SCALING_RATIO_Y), SGVector3(1, 1, 1));
	SGSprite sprite_bg1(SGVector2(0,0),SGVector2(SCREEN_W, SCREEN_H),SGVector3(1,1,1));
	SGSprite sprite_bg2(SGVector2(0,0),SGVector2(SCREEN_W, SCREEN_H),SGVector3(1,1,1));
	SGSprite sprite_bg3(SGVector2(0,0),SGVector2(SCREEN_W, SCREEN_H),SGVector3(1,1,1));
	SGSprite sprite_torch(SGVector2(500 * SCALING_RATIO_X, 700 * SCALING_RATIO_Y),SGVector2(200 * SCALING_RATIO_X,200 * SCALING_RATIO_Y),SGVector3(1,1,1));
	SGSprite sprite_character(SGVector2(100 * SCALING_RATIO_X, 690 * SCALING_RATIO_Y), SGVector2(200 * SCALING_RATIO_X, 200 * SCALING_RATIO_Y), SGVector3(1, 1, 1));
	SGSprite sprite_ground(SGVector2(0, 880 * SCALING_RATIO_X), SGVector2(SCREEN_W, 400 * SCALING_RATIO_Y), SGVector3(1, 1, 1));
	
	//Load textures
	SGTexture texFire("./firesheet5x5.png");
	SGTexture texBg1("./background1.png");
	SGTexture texBg2("./background2.png");
	SGTexture texBg3("./background3.png");
	SGTexture texTorch("./Torch.png");
	SGTexture texCharacterSheet("./Check.png");
	SGTexture texGround("./ground.png");
	//Create Shader object
	SGShader spriteShader("./Sprite.vs", "./Sprite.fs");
	SGShader spriteTiledShader("./Sprite.vs", "./SpriteTiled.fs");
	//Attach shader object to sprite object
	sprite_fire.AttachShader(spriteShader);
	sprite_bg1.AttachShader(spriteShader);
	sprite_bg2.AttachShader(spriteShader);
	sprite_bg3.AttachShader(spriteShader);
	sprite_torch.AttachShader(spriteShader);
	sprite_character.AttachShader(spriteShader);
	sprite_ground.AttachShader(spriteTiledShader);
	//Attach shader object to sprite object
	sprite_fire.GetShader().AddTexture(GL_TEXTURE0, texFire.GetTextureID(), "mainTex", 0);
	sprite_bg1.GetShader().AddTexture(GL_TEXTURE0, texBg1.GetTextureID(), "mainTex", 0);
	sprite_bg2.GetShader().AddTexture(GL_TEXTURE0, texBg2.GetTextureID(), "mainTex", 0);
	sprite_bg3.GetShader().AddTexture(GL_TEXTURE0, texBg3.GetTextureID(), "mainTex", 0);
	sprite_torch.GetShader().AddTexture(GL_TEXTURE0, texTorch.GetTextureID(), "mainTex", 0);
	sprite_character.GetShader().AddTexture(GL_TEXTURE0, texCharacterSheet.GetTextureID(), "mainTex", 0);
	sprite_ground.GetShader().AddTexture(GL_TEXTURE0, texGround.GetTextureID(), "mainTex", 0);
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float timeNow, timeOld, deltaTime, framePS;

	framePS = 25.0f;
	timeNow = timeOld = glfwGetTime();
	//Fire Animation data
	float nxFrame, nyFrame;
	nxFrame = nyFrame = 5;
	float uvX = 0.0f, uvY = 0.0f;
	float character_uvX = 0.0f, character_uvY = 0.0f;

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.2, 0.3, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		timeNow = glfwGetTime();
		deltaTime = timeNow - timeOld;
		if (deltaTime >= 1.0f / framePS)
		{
			deltaTime = 0;
			timeOld = timeNow;
			uvX +=1;
			character_uvX += 1;
			if (character_uvX >= 5)
			{
				character_uvX = 0;
				character_uvY += 1;
			}
			if (character_uvY >= 2 && (!isPressedD && !isPressedA))
			{
				character_uvY = 0;
			}
			if (isPressedD || isPressedA)
			{
				character_uvY = 2;

			}
			if (character_uvY >= 4 && (isPressedD || isPressedA))
			{
				character_uvY = 2;
			}
			if (uvX >= nxFrame)
			{
				uvX = 0.0f;
				uvY += 1.0f;
			}
			if (uvY >= nyFrame)
			{
				uvY = 0.0f;
			}
		}


		SGMat4 model = SGMat4(1.0f);
		glm::mat4 projection;
		projection = glm::ortho(0.0f, SCREEN_W, SCREEN_H, 0.0f, -1.0f, 1.0f);
		
		sprite_bg1.GetShader().ActiveTexture();
		sprite_bg1.GetShader().Use();
		
		sprite_bg1.GetShader().SetMatrix4("projection", projection);

		sprite_bg1.GetShader().SetUniform1f("uvX", 0);
		sprite_bg1.GetShader().SetUniform1f("uvY", 0);
		sprite_bg1.GetShader().SetUniform1f("nxFrame", 1);
		sprite_bg1.GetShader().SetUniform1f("nyFrame", 1);

		sprite_bg1.Draw();
		
		sprite_bg2.GetShader().ActiveTexture();
		sprite_bg2.GetShader().Use();

		sprite_bg2.GetShader().SetMatrix4("projection", projection);

		sprite_bg2.GetShader().SetUniform1f("uvX", 0);
		sprite_bg2.GetShader().SetUniform1f("uvY", 0);
		sprite_bg2.GetShader().SetUniform1f("nxFrame", 1);
		sprite_bg2.GetShader().SetUniform1f("nyFrame", 1);

		sprite_bg2.Draw();

		sprite_bg3.GetShader().ActiveTexture();
		sprite_bg3.GetShader().Use();

		sprite_bg3.GetShader().SetMatrix4("projection", projection);

		sprite_bg3.GetShader().SetUniform1f("uvX", 0);
		sprite_bg3.GetShader().SetUniform1f("uvY", 0);
		sprite_bg3.GetShader().SetUniform1f("nxFrame", 1);
		sprite_bg3.GetShader().SetUniform1f("nyFrame", 1);

		sprite_bg3.Draw();

		sprite_torch.GetShader().ActiveTexture();
		sprite_torch.GetShader().Use();

		sprite_torch.GetShader().SetUniform1f("uvX", 0);
		sprite_torch.GetShader().SetUniform1f("uvY", 0);
		sprite_torch.GetShader().SetUniform1f("nxFrame", 1);
		sprite_torch.GetShader().SetUniform1f("nyFrame", 1);

		sprite_torch.GetShader().SetMatrix4("projection", projection);
		
		sprite_torch.Position().x = 500;
		sprite_torch.Draw();
		sprite_torch.Position().x = 1000;
		sprite_torch.Draw();

		sprite_fire.GetShader().ActiveTexture();
		sprite_fire.GetShader().Use();

		sprite_fire.GetShader().SetUniform1f("uvX", uvX);
		sprite_fire.GetShader().SetUniform1f("uvY", uvY);
		sprite_fire.GetShader().SetUniform1f("nxFrame", nxFrame);
		sprite_fire.GetShader().SetUniform1f("nyFrame", nyFrame);

		sprite_fire.GetShader().SetMatrix4("projection", projection);

		sprite_fire.Position().x = 500;

		sprite_fire.Draw();

		sprite_fire.Position().x = 1000;
		sprite_fire.Draw();

		sprite_character.GetShader().ActiveTexture();
		sprite_character.GetShader().Use();

		sprite_character.GetShader().SetUniform1f("uvX", character_uvX);
		sprite_character.GetShader().SetUniform1f("uvY", character_uvY);
		sprite_character.GetShader().SetUniform1f("nxFrame", 5);
		sprite_character.GetShader().SetUniform1f("nyFrame", 4);

		sprite_character.GetShader().SetMatrix4("projection", projection);

		if (isPressedA)
		{
			sprite_character.eulerAngle().y = 180;
			sprite_character.Position().x -= 100 * deltaTime;
		}
		if (isPressedD)
		{
			sprite_character.eulerAngle().y = 0;
			sprite_character.Position().x += 100 * deltaTime;

		}
		sprite_character.Draw();

		sprite_ground.GetShader().ActiveTexture();
		sprite_ground.GetShader().Use();

		sprite_ground.GetShader().SetUniform1f("tileX", 1);
		sprite_ground.GetShader().SetUniform1f("tileY", 1);

		sprite_ground.GetShader().SetMatrix4("projection", projection);
		sprite_ground.Draw();

		/// <returns></returns>
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void OnWindowResize(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		isPressedD = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE && isPressedD)
	{
		isPressedD = false;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		isPressedA = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && isPressedA)
	{
		isPressedA = false;
	}

}