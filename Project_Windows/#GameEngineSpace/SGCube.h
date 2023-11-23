#ifndef _SG_CUBE_H
#define _SG_CUBE_H

#include<SGUtility.h>
class SGCube
{
public:

	SGCube();
	SGCube(SGVector3 pos, SGVector3 size, SGVector3 rot);
	~SGCube();
	void Draw();

private:
	void init();
	
	unsigned int VAO;
	SGVector3 position, scale, rotation;
};

#endif // !_SG_CUBE_H