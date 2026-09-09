#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "DataClasses/Button.h"
#include "DataClasses/Character.h"
#include "DataClasses/SoundEngine.h"
#include "DataClasses/SoundManager.h"
#include "DataClasses/SpriteRenderer.h"
#include "DataClasses/model.h"
#include "DataClasses/shader.h"
#include "GameClasses/GameCommon.h"
#include "DataClasses/ScoreManager.h"
#include <map>
#include <vector>

using namespace std;

// settings
extern int SCR_WIDTH;
extern int SCR_HEIGHT;

// camera
extern float lastX;
extern float lastY;
// Pulsanti
extern Button playButton;
extern Button settingsMenuButton;
extern Button scoreMenuButton;
extern Button exitGameButton;

// timing
extern float deltaTime;
extern float lastFrame;

// Shaders
extern Shader gameShader;
extern Shader shaderText;
extern Shader powerupShader;
extern SpriteRenderer sprite2D;
extern Shader menuBGShader;

// Liste
extern string gameName;
extern SoundEngine soundEngine;
extern SoundManager menuMusic;
extern SoundManager sfxMouse;

extern std::map<GLchar, Character> Characters;
extern unsigned int VAOText, VBOText;

extern void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height);
extern void mouse_callback(GLFWwindow *window, double xPos, double yPos);
extern void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
extern void processInput(GLFWwindow *window);
extern void RenderText(const Shader &shader, std::string text, float x, float y,
                       float scale, glm::vec3 color);
extern Texture2D loadTextureFromFile(const char *file, bool alpha);
extern inline std::string getResource(const std::string &relativePath);

class MainViewController {
public:
  int main(GLFWwindow *window);

protected:
  void loadModels();
  void loadScores();
  
  bool loaded = false;
  std::vector<double> topScores;
  Model *roadModel;
  Model *obstacleModel;

  std::vector<glm::vec3> obstaclePosition;
};
