#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "DataClasses/shader.h"
#include "DataClasses/Character.h"
#include "GameClasses/GameCommon.h"

#include <map>
#include <string>

using namespace std;

extern int SCR_WIDTH;
extern int SCR_HEIGHT;
extern float deltaTime;
extern float lastFrame;
extern Shader shaderText;
extern std::map<GLchar, Character> Characters;
extern unsigned int VAOText, VBOText;

extern void RenderText(const Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

class VictoryViewController {
public:
    GameResult main(GLFWwindow* window);
};
