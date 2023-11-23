#include "SGSprite.h"

SGSprite::SGSprite()
{
	this->color = SGVector3(1, 1, 1);
	this->position = SGVector2(0, 0);
	this->scale = SGVector2(1, 1);
	this->rotate = SGVector3(0,0,0);
	this->init();
}

SGSprite::SGSprite(SGVector2 pos, SGVector2 scale, SGVector3 color)
{
	this->color = color;
	this->scale = scale;
	this->position = pos;
	this->rotate = SGVector3(0,0,0);
	this->init();
}

void SGSprite::init()
{
	unsigned int VBO;
	float vertices[] = {
		// pos		// tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float)*2));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SGSprite::AttachShader(const SGShader& shader)
{
	this->shader = new SGShader(shader);
}

void SGSprite::Draw()
{
	SGMat4 model = SGMat4(1.0f);
	model = glm::translate(model, SGVector3(position, 0.0f));
	model = glm::translate(model, SGVector3(0.5 * scale.x, 0.5 * scale.y, 0.0));
	model = glm::rotate(model, glm::radians(rotate.x), SGVector3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(rotate.y), SGVector3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(rotate.z), SGVector3(0.0, 0.0, 1.0));
	model = glm::translate(model, SGVector3(-0.5 * scale.x, -0.5 * scale.y, 0.0));
	model = glm::scale(model, SGVector3(scale, 1.0f));
	shader->SetMatrix4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}