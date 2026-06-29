#include "MainViewController.h"
#include "GameViewController.h"

void MainViewController::loadModels() {}

int MainViewController::main(GLFWwindow *window) {
  shaderText = Shader(getResource("Shaders/shaderText.vs").c_str(),
                      getResource("Shaders/shaderText.fs").c_str());
  gameShader = Shader(getResource("Shaders/shader.vs").c_str(),
                      getResource("Shaders/shader.fs").c_str());
  powerupShader = Shader(getResource("Shaders/powerup.vs").c_str(),
                         getResource("Shaders/powerup.fs").c_str());
  menuBGShader = Shader(getResource("Shaders/2DImage.vs").c_str(),
                        getResource("Shaders/2DImage.fs").c_str());

  sprite2D = SpriteRenderer(menuBGShader);

  Texture2D backGroundTexture = loadTextureFromFile(
      getResource("Backgrounds/templateBackground.png").c_str(), true);

  Texture2D startButtonTexture = loadTextureFromFile(
      getResource("Buttons/StartButton/unselected-removebg-preview.png")
          .c_str(),
      true);
  Texture2D startButtonTextureSelected = loadTextureFromFile(
      getResource("Buttons/StartButton/selected-removebg-preview.png").c_str(),
      true);

  startButton = Button(0.0f, 0.0f, 0.0f, 0.0f);

  menuMusic.playSound();

  while (!glfwWindowShouldClose(window)) {
    const auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    // Background
    menuBGShader.use();
    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),
                   static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
    menuBGShader.setMat4("projection", projection);

    sprite2D.DrawSprite(backGroundTexture, glm::vec2(0.0f, 0.0f),
                        glm::vec2(static_cast<float>(SCR_WIDTH),
                                  static_cast<float>(SCR_HEIGHT)));

    // Start Button
    const float y =
        static_cast<float>(SCR_HEIGHT) - static_cast<float>(SCR_HEIGHT) * 0.25f;
    const float buttonWidth = static_cast<float>(SCR_WIDTH) / 5.0f;
    const float buttonHeight = buttonWidth / 4.0f;
    startButton.x =
        static_cast<float>(SCR_WIDTH) / 3.0f - buttonWidth * 1.4f / 2;
    startButton.y = y - buttonHeight * 0.6f;
    startButton.height = buttonHeight * 3.0f;
    startButton.width = buttonWidth * 1.4f;
    if (startButton.selected == false) {
      sprite2D.DrawSprite(startButtonTexture,
                          glm::vec2(startButton.x, startButton.y),
                          glm::vec2(startButton.width, startButton.height));
    } else {
      sprite2D.DrawSprite(startButtonTextureSelected,
                          glm::vec2(startButton.x, startButton.y),
                          glm::vec2(startButton.width, startButton.height));
    }

    if (startButton.clicked) {
      startButton.clicked = false;
      menuMusic.stopSound();
      bool keepPlaying = true;
      while (keepPlaying && !glfwWindowShouldClose(window)) {
        GameViewController gameView;
        GameResult result = gameView.main(window);
        if (result == RESULT_QUIT) {
          keepPlaying = false;
        }
      }
      if (!glfwWindowShouldClose(window)) {
        menuMusic.playSound();
        // Re-enable 2D rendering state for menu
        glDisable(GL_DEPTH_TEST);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
    }

    if (loaded == false) {
      std::cout << "Load Models" << std::endl;
      loadModels();
      loaded = true;
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
