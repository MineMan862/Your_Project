#include "MainViewController.h"
#include "GameViewController.h"

void MainViewController::loadModels() {
    // Models are loaded by GameViewController when the game starts
}

int MainViewController::main(GLFWwindow* window)
{
    // Shader Loading
    shaderText = Shader(getResource("Shaders/shaderText.vs").c_str(), getResource("Shaders/shaderText.fs").c_str());
    gameShader = Shader(getResource("Shaders/shader.vs").c_str(), getResource("Shaders/shader.fs").c_str());
    menuBGShader = Shader(getResource("Shaders/2DImage.vs").c_str(), getResource("Shaders/2DImage.fs").c_str());

    sprite2D = SpriteRenderer(menuBGShader);

    Texture2D backGroundTexture = loadTextureFromFile(getResource("Backgrounds/templateBackground.png").c_str(),false);

    Texture2D startButtonTexture = loadTextureFromFile(getResource("Buttons/StartButton/unselected.png").c_str(), true);
    Texture2D startButtonTextureSelected = loadTextureFromFile(getResource("Buttons/StartButton/selected.png").c_str(), true);

    startButton = Button(0.0f, 0.0f, 0.0f, 0.0f);

    menuMusic.playSound();

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        // render
        // ------

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST); // Menu is 2D, disable depth test

        // Background
        menuBGShader.use();
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
        menuBGShader.setMat4("projection", projection);

        sprite2D.DrawSprite(backGroundTexture, glm::vec2(0.0f, 0.0f), glm::vec2(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT)));

        //Start Button
        const float y = static_cast<float>(SCR_HEIGHT) - static_cast<float>(SCR_HEIGHT) * 0.25f;
        const float buttonWidth = static_cast<float>(SCR_WIDTH) / 5.0f;
        const float buttonHeight = buttonWidth / 4.0f;
        startButton.x = static_cast<float>(SCR_WIDTH) / 2 - buttonWidth*1.4f / 2;
        startButton.y = y - buttonHeight*0.4f/2;
        startButton.height = buttonHeight*1.4f;
        startButton.width = buttonWidth*1.4f;
        if (startButton.selected == false) {
            sprite2D.DrawSprite(startButtonTexture, glm::vec2(startButton.x, startButton.y), glm::vec2(startButton.width, startButton.height));
        }
        else {
            sprite2D.DrawSprite(startButtonTextureSelected, glm::vec2(startButton.x, startButton.y), glm::vec2(startButton.width, startButton.height));
        }

        // Handle Start button click -> Launch Game
        if (startButton.clicked) {
            startButton.clicked = false;
            menuMusic.stopSound();

            // Launch game loop
            bool keepPlaying = true;
            while (keepPlaying && !glfwWindowShouldClose(window)) {
                GameViewController gameView;
                GameResult result = gameView.main(window);
                if (result == RESULT_QUIT) {
                    keepPlaying = false;
                }
                // If RESULT_RESTART, the game already handled it internally
                // If we get here with QUIT, return to menu
            }

            // Returned from game, restart menu music if window still open
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
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------

    glfwTerminate();
	return 0;
}
