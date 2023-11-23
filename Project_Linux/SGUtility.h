#ifndef _SG_UTILITY_H
#define _SG_UTILITY_H

#include <iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include <GL/glew.h>       // Include GLEW (or new version of GL if on Windows).
#include <GLFW/glfw3.h>    // GLFW helper library.
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"stb_image.h"

#define SCREEN_W 1280.0f
#define SCREEN_H 720.0f
#define TARGET_W 1920.0f
#define TARGET_H 1080.0f
#define SCALING_RATIO_X (SCREEN_W / TARGET_W)
#define SCALING_RATIO_Y (SCREEN_H / TARGET_H)
typedef glm::vec3 SGVector3;
typedef glm::vec2 SGVector2;
typedef glm::mat4 SGMat4;


#endif // !_SG_UTILITY_H
