#include "MainViewController.h"
#include "GameViewController.h"
#include <sstream>
#include <iomanip>

void MainViewController::loadModels() {}

void MainViewController::loadScores() {
  topScores = ScoreManager::GetTopScores(5);
  std::cout << "Loaded " << topScores.size() << " top scores.\n";
}

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

  Shader sliderShader(getResource("Shaders/slider.vs").c_str(),
                      getResource("Shaders/slider.fs").c_str());

  float sliderVertices[] = {
      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f
  };

  unsigned int sliderVAO, sliderVBO;
  glGenVertexArrays(1, &sliderVAO);
  glGenBuffers(1, &sliderVBO);
  glBindVertexArray(sliderVAO);
  glBindBuffer(GL_ARRAY_BUFFER, sliderVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(sliderVertices), sliderVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  Texture2D playTex = loadTextureFromFile(getResource("Buttons/StartButton/Gioca_Deselezionato_Editato.png").c_str(), true);
  Texture2D playTexSel = loadTextureFromFile(getResource("Buttons/StartButton/Gioca_Selezionato_Editato.png").c_str(), true);
  Texture2D settingsTex = loadTextureFromFile(getResource("Buttons/StartButton/Impostazioni_Deselezionato_Editato.png").c_str(), true);
  Texture2D settingsTexSel = loadTextureFromFile(getResource("Buttons/StartButton/Impostazioni_Selezionato_Editato.png").c_str(), true);
  Texture2D scoreTex = loadTextureFromFile(getResource("Buttons/StartButton/Score_Deselezionato_Editato.png").c_str(), true);
  Texture2D scoreTexSel = loadTextureFromFile(getResource("Buttons/StartButton/Score_Selezionato_Editato.png").c_str(), true);
  Texture2D exitTex = loadTextureFromFile(getResource("Buttons/StartButton/Esci_Deselezionato_Editato.png").c_str(), true);
  Texture2D exitTexSel = loadTextureFromFile(getResource("Buttons/StartButton/Esci_Selezionato_Editato.png").c_str(), true);

  playButton = Button(0.0f, 0.0f, 0.0f, 0.0f);
  settingsMenuButton = Button(0.0f, 0.0f, 0.0f, 0.0f);
  scoreMenuButton = Button(0.0f, 0.0f, 0.0f, 0.0f);
  exitGameButton = Button(0.0f, 0.0f, 0.0f, 0.0f);

  bool isScoreMenuVisible = false;
  bool isSettingsMenuVisible = false;
  Button backScoreButton(0.0f, 0.0f, 0.0f, 0.0f);
  Button backSettingsButton(0.0f, 0.0f, 0.0f, 0.0f);
  Button musicSlider(0.0f, 0.0f, 0.0f, 0.0f);
  Button soundSlider(0.0f, 0.0f, 0.0f, 0.0f);
  static bool ignoreMouseUntilRelease = false;

  loadScores();
  menuMusic.playSound();

  while (!glfwWindowShouldClose(window)) {
    const auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        ignoreMouseUntilRelease = false;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    // Background
    menuBGShader.use();
    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),
                   static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
    menuBGShader.setMat4("projection", projection);

    glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    shaderText.use();
    shaderText.setMat4("projection", textProjection);

    sprite2D.DrawSprite(backGroundTexture, glm::vec2(0.0f, 0.0f),
                        glm::vec2(static_cast<float>(SCR_WIDTH),
                                  static_cast<float>(SCR_HEIGHT)));

    // Layout parameters
    const float buttonWidth = static_cast<float>(SCR_WIDTH) * 0.35f; 
    const float buttonHeight = buttonWidth * 0.30f; // Stretch verticale bilanciato
    const float leftXOffset = static_cast<float>(SCR_WIDTH) * 0.1f;
    const float startY = static_cast<float>(SCR_HEIGHT) * 0.15f;
    const float spacing = buttonHeight * 1.05f; // Riduciamo lo spazio per non uscire dallo schermo

    auto updateButton = [&](Button& btn, float yOffset) {
        btn.x = leftXOffset;
        btn.y = startY + yOffset;
        btn.width = buttonWidth;
        btn.height = buttonHeight;
    };

    // Sposta i bottoni fuori dallo schermo per evitare "ghost clicks" quando nascosti
    playButton.x = -10000.0f;
    settingsMenuButton.x = -10000.0f;
    scoreMenuButton.x = -10000.0f;
    exitGameButton.x = -10000.0f;

    auto drawButton = [&](Button& btn, Texture2D& normalTex, Texture2D& selTex) {
        if (!btn.selected) {
            sprite2D.DrawSprite(normalTex, glm::vec2(btn.x, btn.y), glm::vec2(btn.width, btn.height));
        } else {
            sprite2D.DrawSprite(selTex, glm::vec2(btn.x, btn.y), glm::vec2(btn.width, btn.height));
        }
    };

    if (!isScoreMenuVisible && !isSettingsMenuVisible) {
        updateButton(playButton, 0.0f);
        updateButton(settingsMenuButton, spacing);
        updateButton(scoreMenuButton, spacing * 2.0f);
        updateButton(exitGameButton, spacing * 3.0f);

        drawButton(playButton, playTex, playTexSel);
        drawButton(settingsMenuButton, settingsTex, settingsTexSel);
        drawButton(scoreMenuButton, scoreTex, scoreTexSel);
        drawButton(exitGameButton, exitTex, exitTexSel);
        
        if (playButton.clicked) {
          playButton.clicked = false;
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
            glDisable(GL_DEPTH_TEST);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            loadScores();
          }
        }

        if (settingsMenuButton.clicked) {
            settingsMenuButton.clicked = false;
            isSettingsMenuVisible = true;
            ignoreMouseUntilRelease = true;
        }

        if (scoreMenuButton.clicked) {
            scoreMenuButton.clicked = false;
            isScoreMenuVisible = true;
        }

        if (exitGameButton.clicked) {
            exitGameButton.clicked = false;
            glfwSetWindowShouldClose(window, true);
        }
    } else if (isScoreMenuVisible) {
        // --- Draw Scoreboard ---
        float scale = static_cast<float>(SCR_HEIGHT) / 1080.0f;
        float textScale = 0.8f * scale;
        float titleScale = 1.2f * scale;
        
        float centerX = static_cast<float>(SCR_WIDTH) * 0.5f - 200.0f * scale;
        float textTopY = static_cast<float>(SCR_HEIGHT) * 0.75f;
        RenderText(shaderText, "TOP 5 SCORE:", centerX, textTopY, titleScale, glm::vec3(0.9f, 0.8f, 0.2f));
        for (size_t i = 0; i < topScores.size(); i++) {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(1) << topScores[i];
            RenderText(shaderText, std::to_string(i + 1) + ". " + ss.str() + " m", 
                       centerX, textTopY - (i + 1) * 60.0f * scale, textScale, glm::vec3(1.0f));
        }

        // Setup back button
        float backY = textTopY - 400.0f * scale;
        backScoreButton.x = centerX + 50.0f * scale;
        backScoreButton.y = backY - 20.0f * scale;
        backScoreButton.width = 200.0f * scale;
        backScoreButton.height = 60.0f * scale;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        int w, h; glfwGetWindowSize(window, &w, &h);
        float curX = static_cast<float>(SCR_WIDTH) * static_cast<float>(mouseX) / static_cast<float>(w);
        float curY = static_cast<float>(SCR_HEIGHT) * static_cast<float>(mouseY) / static_cast<float>(h);
        
        glm::vec3 backColor = glm::vec3(1.0f);
        // backScoreButton.y è in coordinate Y=dal basso (RenderText), ma curY è in coordinate Y=dall'alto (mouse)
        float hitTestY = static_cast<float>(SCR_HEIGHT) - backScoreButton.y - backScoreButton.height;

        if (curX >= backScoreButton.x && curX <= backScoreButton.x + backScoreButton.width &&
            curY >= hitTestY && curY <= hitTestY + backScoreButton.height) {
            backColor = glm::vec3(0.8f, 0.2f, 0.2f);
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                if (!backScoreButton.clicked) {
                    sfxMouse.playSound();
                    backScoreButton.clicked = true;
                }
            } else {
                if (backScoreButton.clicked) {
                    isScoreMenuVisible = false;
                    backScoreButton.clicked = false;
                }
            }
        } else {
            backScoreButton.clicked = false;
        }

        RenderText(shaderText, "Indietro", backScoreButton.x, backY, textScale, backColor);
    } else if (isSettingsMenuVisible) {
        float scale = static_cast<float>(SCR_HEIGHT) / 1080.0f;
        float textScale = 0.8f * scale;
        float titleScale = 1.2f * scale;
        
        float centerX = static_cast<float>(SCR_WIDTH) * 0.5f;
        float centerY = static_cast<float>(SCR_HEIGHT) * 0.5f;
        float textTopY = static_cast<float>(SCR_HEIGHT) * 0.75f;
        
        RenderText(shaderText, "IMPOSTAZIONI", centerX - 120.0f * scale, textTopY, titleScale, glm::vec3(0.9f, 0.8f, 0.2f));

        float sliderWidth = static_cast<float>(SCR_WIDTH) * 0.35f;
        float sliderHeight = static_cast<float>(SCR_HEIGHT) * 0.035f;
        float spacing = sliderHeight * 4.0f;

        musicSlider.width = sliderWidth;
        musicSlider.height = sliderHeight;
        musicSlider.x = centerX - sliderWidth / 2.0f;
        musicSlider.y = centerY + spacing;

        soundSlider.width = sliderWidth;
        soundSlider.height = sliderHeight;
        soundSlider.x = centerX - sliderWidth / 2.0f;
        soundSlider.y = centerY;

        backSettingsButton.width = 200.0f * scale;
        backSettingsButton.height = 60.0f * scale;
        backSettingsButton.x = centerX - backSettingsButton.width / 2.0f;
        backSettingsButton.y = centerY - spacing * 2.5f;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        int w, h; glfwGetWindowSize(window, &w, &h);
        float curX = static_cast<float>(SCR_WIDTH) * static_cast<float>(mouseX) / static_cast<float>(w);
        float curY = static_cast<float>(SCR_HEIGHT) * static_cast<float>(mouseY) / static_cast<float>(h);
        float hitTestY = static_cast<float>(SCR_HEIGHT) - curY;

        // Back button
        glm::vec3 backColor = glm::vec3(1.0f);
        if (curX >= backSettingsButton.x && curX <= backSettingsButton.x + backSettingsButton.width &&
            hitTestY >= backSettingsButton.y && hitTestY <= backSettingsButton.y + backSettingsButton.height) {
            backColor = glm::vec3(0.8f, 0.2f, 0.2f);
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                if (!backSettingsButton.clicked && !ignoreMouseUntilRelease) {
                    sfxMouse.playSound();
                    backSettingsButton.clicked = true;
                }
            } else {
                if (backSettingsButton.clicked) {
                    isSettingsMenuVisible = false;
                    backSettingsButton.clicked = false;
                    ignoreMouseUntilRelease = true;
                }
            }
        } else {
            backSettingsButton.clicked = false;
        }

        RenderText(shaderText, "Indietro", backSettingsButton.x + 40.0f*scale, backSettingsButton.y, textScale, backColor);

        // Slider handler
        auto handleSlider = [&](Button &slider, float &volParam, auto setterFunc) {
            if (curX >= slider.x && curX <= slider.x + slider.width &&
                hitTestY >= slider.y - 10.0f && hitTestY <= slider.y + slider.height + 10.0f) {
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !ignoreMouseUntilRelease) {
                    float relX = curX - slider.x;
                    volParam = (relX / slider.width) * 100.0f;
                    if (volParam < 0.0f) volParam = 0.0f;
                    if (volParam > 100.0f) volParam = 100.0f;
                    setterFunc(volParam);
                }
            }
        };

        handleSlider(musicSlider, soundEngine.volMenuMusic, [&](float v) {
            soundEngine.setVolMenuMusic(v);
            menuMusic.changeVolume();
        });

        handleSlider(soundSlider, soundEngine.volSound, [&](float v) {
            soundEngine.setVolSuono(v);
            sfxMouse.changeVolume();
        });

        auto drawSlider = [&](Button &slider, float volParam, const std::string &label) {
            RenderText(shaderText, label, slider.x, slider.y + slider.height + 10.0f * scale, textScale, glm::vec3(1.0f));

            sliderShader.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(slider.x, slider.y, 0.0f));
            model = glm::scale(model, glm::vec3(slider.width, slider.height, 1.0f));
            
            sliderShader.setMat4("model", model);
            sliderShader.setMat4("projection", textProjection);
            sliderShader.setVec3("spriteColor", glm::vec3(0.2f, 0.8f, 0.2f));
            sliderShader.setFloat("fillPercentage", volParam / 100.0f);

            glDisable(GL_CULL_FACE);
            glBindVertexArray(sliderVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
            glEnable(GL_CULL_FACE);
        };

        drawSlider(musicSlider, soundEngine.volMenuMusic, "Volume Musica");
        drawSlider(soundSlider, soundEngine.volSound, "Volume Effetti");
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
