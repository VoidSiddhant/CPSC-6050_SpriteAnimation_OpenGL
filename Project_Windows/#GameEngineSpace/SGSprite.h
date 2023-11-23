#ifndef _SPRITE_H
#define _SPRITE_H

#include<SGUtility.h>
#include<SGShader.h>
class SGSprite
{
public:
	SGSprite();
	SGSprite(SGVector2 pos, SGVector2 scale, SGVector3 color);
	void AttachShader(const SGShader& shader);
	SGShader& GetShader() { return *shader; }
	void Draw();
	SGVector2& Position() { return position; }
	SGVector2& Scale() { return scale; }
	SGVector3& eulerAngle() { return rotate; }
private:
	//Member Functions
	void init();


	//Member Variables
	unsigned int VAO;
	SGVector3 rotate;
	SGVector2 position, scale;
	SGVector3 color;
	SGShader* shader;
};

#endif // !_SPRITE_H


