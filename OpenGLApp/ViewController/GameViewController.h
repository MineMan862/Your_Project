#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "DataClasses/shader.h"
#include "DataClasses/model.h"
#include "DataClasses/camera.h"
#include "DataClasses/Character.h"
#include "DataClasses/SpriteRenderer.h"
#include "DataClasses/SoundManager.h"
#include "DataClasses/SoundEngine.h"
#include "GameClasses/GameCommon.h"
#include "GameClasses/Bike.h"
#include "GameClasses/Level.h"

#include <map>
#include <string>

using namespace std;

// Externs from Main.cpp
extern int SCR_WIDTH;
extern int SCR_HEIGHT;
extern float deltaTime;
extern float lastFrame;
extern Shader gameShader;
extern Shader shaderText;
extern Shader menuBGShader;
extern SpriteRenderer sprite2D;
extern SoundEngine soundEngine;
extern std::map<GLchar, Character> Characters;
extern unsigned int VAOText, VBOText;

extern void RenderText(const Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
extern inline std::string getResource(const std::string& relativePath);
extern Texture2D loadTextureFromFile(const char* file, bool alpha);
extern void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class GameViewController {
public:
    GameResult main(GLFWwindow* window);

private:
    Bike bike;
    Level level;
    Camera gameCamera;
    bool isFirstPerson = false;
    bool cKeyWasPressed = false;
    bool aKeyWasPressed = false;
    bool dKeyWasPressed = false;
    bool spaceKeyWasPressed = false;
    bool rKeyWasPressed = false;
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;
    bool firstMouseMove = true;

    // Rendering - procedural geometry
    unsigned int cubeVAO = 0, cubeVBO = 0;
    unsigned int groundVAO = 0, groundVBO = 0;

    // Assimp Models
    Model bikeModel;
    bool bikeModelLoaded = false;
    Model furgoncinoModel;   // Obstacle TALL
    bool furgoncinoModelLoaded = false;
    Model macerieModel;      // Obstacle LOW
    bool macerieModelLoaded = false;
    Model stradaModel;       // Road segment
    bool stradaModelLoaded = false;
    Model planeModel;        // Scenery plane
    bool planeModelLoaded = false;

    // Sound
    SoundManager gameMusic;
    SoundManager sfxJump;
    SoundManager sfxCrash;

    void setupGeometry();
    void loadModels();
    void setupLighting(Shader& shader);
    void renderRoad(Shader& shader);
    void renderBike(Shader& shader);
    void renderObstacles(Shader& shader);
    void renderHUD(GLFWwindow* window);
    void renderCube(Shader& shader, glm::vec3 position, glm::vec3 scale, glm::vec3 color);
    void renderModel(Shader& shader, Model& mdl, glm::vec3 position, glm::vec3 scale, float rotationY = 0.0f);
    void handleInput(GLFWwindow* window);
    void updateCamera();
    void reset();
    void cleanup();
};
