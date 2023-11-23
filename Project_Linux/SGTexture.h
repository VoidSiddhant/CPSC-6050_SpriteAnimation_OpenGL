#ifndef _SG_TEXTURE
#define _SG_TEXTURE


#include"SGUtility.h"

class SGTexture
{

public:
	SGTexture(const char* filename);
	unsigned int& GetTextureID() { return textureID; }

private:
	unsigned int textureID;
};


#endif // !_SG_SGTEXTURE

