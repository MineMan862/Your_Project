#include "GameViewController.h"
#include "GameOverViewController.h"
#include "VictoryViewController.h"
#include <sstream>
#include <iomanip>

// Unit cube vertex data: position(3) + normal(3) + texcoord(2) = 8 floats per vertex, 36 vertices
static const float cubeVerticesData[] = {
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    // Left face
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
};

// Ground plane vertices (XZ plane at Y=0)
static const float groundVerticesData[] = {
    // pos                  normal           texcoord
    -1.0f, 0.0f,  1.0f,    0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
     1.0f, 0.0f,  1.0f,    0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
     1.0f, 0.0f, -1.0f,    0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
     1.0f, 0.0f, -1.0f,    0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
    -1.0f, 0.0f, -1.0f,    0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -1.0f, 0.0f,  1.0f,    0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
};

void GameViewController::setupGeometry() {
    // Cube VAO
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesData), cubeVerticesData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // Ground VAO
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVerticesData), groundVerticesData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void GameViewController::setupLighting(Shader& shader) {
    shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    shader.setVec3("dirLight.ambient", glm::vec3(0.35f, 0.35f, 0.35f));
    shader.setVec3("dirLight.diffuse", glm::vec3(0.8f, 0.8f, 0.75f));
    shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setFloat("shininess", 32.0f);
}

void GameViewController::renderCube(Shader& shader, glm::vec3 position, glm::vec3 scale, glm::vec3 color) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    shader.setMat4("model", model);
    shader.setBool("useTexture", false);
    shader.setVec3("objectColor", color);
    shader.setFloat("alpha", 1.0f);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void GameViewController::renderRoad(Shader& shader) {
    float roadHalfWidth = bike.laneWidth * 2.0f;
    float roadLength = level.length + 20.0f;

    // Main road surface
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.01f, -roadLength / 2.0f + 10.0f));
    model = glm::scale(model, glm::vec3(roadHalfWidth, 1.0f, roadLength / 2.0f));
    shader.setMat4("model", model);
    shader.setBool("useTexture", false);
    shader.setVec3("objectColor", glm::vec3(0.25f, 0.25f, 0.28f)); // Dark asphalt
    shader.setFloat("alpha", 1.0f);
    glBindVertexArray(groundVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Grass on left side
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-roadHalfWidth - 15.0f, -0.02f, -roadLength / 2.0f + 10.0f));
    model = glm::scale(model, glm::vec3(15.0f, 1.0f, roadLength / 2.0f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.15f)); // Green grass
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Grass on right side
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(roadHalfWidth + 15.0f, -0.02f, -roadLength / 2.0f + 10.0f));
    model = glm::scale(model, glm::vec3(15.0f, 1.0f, roadLength / 2.0f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.15f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // Lane divider lines (white strips on the road)
    float lineHeight = 0.02f;
    float lineWidth = 0.1f;

    // Left lane divider (between lane 0 and lane 1)
    float dividerX1 = -bike.laneWidth / 2.0f;
    for (float z = 5.0f; z > -(level.length + 5.0f); z -= 4.0f) {
        renderCube(shader, glm::vec3(dividerX1, lineHeight, z), glm::vec3(lineWidth, 0.02f, 2.0f), glm::vec3(1.0f, 1.0f, 0.9f));
    }

    // Right lane divider (between lane 1 and lane 2)
    float dividerX2 = bike.laneWidth / 2.0f;
    for (float z = 5.0f; z > -(level.length + 5.0f); z -= 4.0f) {
        renderCube(shader, glm::vec3(dividerX2, lineHeight, z), glm::vec3(lineWidth, 0.02f, 2.0f), glm::vec3(1.0f, 1.0f, 0.9f));
    }

    // Road edge lines (solid)
    float edgeLeft = -roadHalfWidth + 0.15f;
    float edgeRight = roadHalfWidth - 0.15f;
    for (float z = 5.0f; z > -(level.length + 5.0f); z -= 6.0f) {
        renderCube(shader, glm::vec3(edgeLeft, lineHeight, z), glm::vec3(0.15f, 0.02f, 6.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        renderCube(shader, glm::vec3(edgeRight, lineHeight, z), glm::vec3(0.15f, 0.02f, 6.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    }
}

void GameViewController::renderBike(Shader& shader) {
    glm::vec3 bikePos = bike.getPosition();

    if (bikeModelLoaded) {
        // Render loaded Assimp model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, bikePos);
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        shader.setMat4("model", model);
        shader.setBool("useTexture", false);
        shader.setVec3("objectColor", glm::vec3(0.2f, 0.6f, 0.3f));
        shader.setFloat("alpha", 1.0f);
        bikeModel.Draw(shader);
    } else {
        // Fallback: render a placeholder bike from cubes
        // Frame/body
        renderCube(shader, bikePos + glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.4f, 0.8f, 1.2f), glm::vec3(0.2f, 0.55f, 0.8f));
        // Seat
        renderCube(shader, bikePos + glm::vec3(0.0f, 1.0f, 0.15f), glm::vec3(0.3f, 0.15f, 0.4f), glm::vec3(0.15f, 0.15f, 0.15f));
        // Handlebars
        renderCube(shader, bikePos + glm::vec3(0.0f, 0.9f, -0.5f), glm::vec3(0.7f, 0.1f, 0.1f), glm::vec3(0.3f, 0.3f, 0.3f));
        // Front wheel
        renderCube(shader, bikePos + glm::vec3(0.0f, 0.2f, -0.55f), glm::vec3(0.08f, 0.4f, 0.4f), glm::vec3(0.1f, 0.1f, 0.1f));
        // Rear wheel
        renderCube(shader, bikePos + glm::vec3(0.0f, 0.2f, 0.45f), glm::vec3(0.08f, 0.4f, 0.4f), glm::vec3(0.1f, 0.1f, 0.1f));
    }
}

void GameViewController::renderObstacles(Shader& shader) {
    for (const auto& obs : level.obstacles) {
        if (!obs.active) continue;
        // Only render if reasonably close to the bike
        if (obs.posZ > bike.posZ + 15.0f || obs.posZ < bike.posZ - 80.0f) continue;

        glm::vec3 pos = obs.getPosition(bike.laneWidth);
        glm::vec3 scale = obs.getScale();
        renderCube(shader, pos, scale, obs.color);
    }
}

void GameViewController::renderHUD(GLFWwindow* window) {
    // Switch to 2D orthographic projection for HUD
    shaderText.use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    shaderText.setMat4("projection", projection);

    float scale = static_cast<float>(SCR_HEIGHT) / 1080.0f;

    // -- Progress bar --
    float progress = level.getProgress(bike.posZ);
    int pct = static_cast<int>(progress * 100.0f);
    std::stringstream ss;
    ss << pct << "%";

    // Bar background
    float barX = 50.0f * scale;
    float barY = static_cast<float>(SCR_HEIGHT) - 50.0f * scale;
    float barW = 300.0f * scale;
    float barH = 20.0f * scale;

    // Render progress text
    RenderText(shaderText, "Progresso: " + ss.str(), barX, barY + 5.0f * scale, 0.5f * scale, glm::vec3(1.0f, 1.0f, 1.0f));

    // Render progress bar using cubes (switch back to game shader briefly)
    // Actually, use text-based visualization for simplicity:
    std::string barFill = "";
    int fillCount = static_cast<int>(progress * 20);
    for (int i = 0; i < 20; i++) {
        barFill += (i < fillCount) ? "|" : ".";
    }
    RenderText(shaderText, "[" + barFill + "]", barX, barY - 25.0f * scale, 0.45f * scale, glm::vec3(0.4f, 0.9f, 0.4f));

    // -- Speed display --
    std::stringstream speedSS;
    speedSS << std::fixed << std::setprecision(1) << bike.speed;
    RenderText(shaderText, "Velocita: " + speedSS.str(), barX + 350.0f * scale, barY + 5.0f * scale, 0.4f * scale, glm::vec3(0.9f, 0.9f, 0.6f));

    // -- Key layout at bottom --
    float keyY = 80.0f * scale;
    float keyScale = 0.5f * scale;
    float centerX = static_cast<float>(SCR_WIDTH) / 2.0f;

    // Check key states for feedback
    bool aPressed = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool dPressed = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool spacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    bool cPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;

    // Colors: gray transparent when not pressed, white when pressed
    glm::vec3 inactiveColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 activeColor = glm::vec3(1.0f, 1.0f, 1.0f);

    // Render key labels
    RenderText(shaderText, "[A] Sinistra", centerX - 280.0f * scale, keyY, keyScale, aPressed ? activeColor : inactiveColor);
    RenderText(shaderText, "[D] Destra", centerX + 50.0f * scale, keyY, keyScale, dPressed ? activeColor : inactiveColor);
    RenderText(shaderText, "[SPACE] Salta", centerX - 130.0f * scale, keyY - 35.0f * scale, keyScale, spacePressed ? activeColor : inactiveColor);
    RenderText(shaderText, "[C] Camera", centerX - 280.0f * scale, keyY - 70.0f * scale, 0.4f * scale, cPressed ? activeColor : inactiveColor);

    // Camera mode indicator
    std::string cameraMode = isFirstPerson ? "1a Persona" : "3a Persona";
    RenderText(shaderText, "Camera: " + cameraMode, centerX + 50.0f * scale, keyY - 70.0f * scale, 0.4f * scale, glm::vec3(0.7f, 0.7f, 0.9f));
}

void GameViewController::handleInput(GLFWwindow* window) {
    // ESC to quit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // A key - move left (trigger once)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (!aKeyWasPressed) {
            bike.moveLeft();
            aKeyWasPressed = true;
        }
    } else {
        aKeyWasPressed = false;
    }

    // D key - move right (trigger once)
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (!dKeyWasPressed) {
            bike.moveRight();
            dKeyWasPressed = true;
        }
    } else {
        dKeyWasPressed = false;
    }

    // SPACE - jump
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!spaceKeyWasPressed) {
            bike.jump();
            // Play jump sound if available
            // sfxJump.playSound();
            spaceKeyWasPressed = true;
        }
    } else {
        spaceKeyWasPressed = false;
    }

    // C - toggle camera
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        if (!cKeyWasPressed) {
            isFirstPerson = !isFirstPerson;
            if (isFirstPerson) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                firstMouseMove = true;
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                // Reset camera orientation
                gameCamera.Yaw = -90.0f;
                gameCamera.Pitch = -15.0f;
            }
            cKeyWasPressed = true;
        }
    } else {
        cKeyWasPressed = false;
    }

    // Handle mouse for first person camera
    if (isFirstPerson) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        float xpos = static_cast<float>(mouseX);
        float ypos = static_cast<float>(mouseY);

        if (firstMouseMove) {
            lastMouseX = xpos;
            lastMouseY = ypos;
            firstMouseMove = false;
        }

        float xoffset = xpos - lastMouseX;
        float yoffset = lastMouseY - ypos; // Reversed: y-coordinates bottom to top
        lastMouseX = xpos;
        lastMouseY = ypos;

        gameCamera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void GameViewController::updateCamera() {
    glm::vec3 bikePos = bike.getPosition();

    if (isFirstPerson) {
        // First person: camera at bike's handlebar height, looking forward
        gameCamera.Position = bikePos + glm::vec3(0.0f, 1.2f, -0.3f);
        // Camera Front/Up/Right are managed by ProcessMouseMovement
    } else {
        // Third person: camera behind and above the bike
        float camDistance = 8.0f;
        float camHeight = 5.0f;
        gameCamera.Position = bikePos + glm::vec3(0.0f, camHeight, camDistance);
        // Look at the bike
        glm::vec3 target = bikePos + glm::vec3(0.0f, 1.0f, -5.0f);
        gameCamera.Front = glm::normalize(target - gameCamera.Position);
        gameCamera.Right = glm::normalize(glm::cross(gameCamera.Front, glm::vec3(0.0f, 1.0f, 0.0f)));
        gameCamera.Up = glm::normalize(glm::cross(gameCamera.Right, gameCamera.Front));
    }
}

void GameViewController::reset() {
    bike.reset();
    level.reset();
    isFirstPerson = false;
    firstMouseMove = true;
    gameCamera.Yaw = -90.0f;
    gameCamera.Pitch = -15.0f;
}

void GameViewController::cleanup() {
    if (cubeVAO) {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteBuffers(1, &cubeVBO);
        cubeVAO = cubeVBO = 0;
    }
    if (groundVAO) {
        glDeleteVertexArrays(1, &groundVAO);
        glDeleteBuffers(1, &groundVBO);
        groundVAO = groundVBO = 0;
    }
}

GameResult GameViewController::main(GLFWwindow* window) {
    setupGeometry();
    level.generate();
    bike.reset();
    gameCamera = Camera(glm::vec3(0.0f, 5.0f, 8.0f));
    gameCamera.Yaw = -90.0f;
    gameCamera.Pitch = -15.0f;

    // Try to load bike model via Assimp
    std::string modelPath = getResource("Models/placeholder/placeholder.obj");
    bikeModel.loadModel(modelPath);
    bikeModelLoaded = !bikeModel.meshes.empty();
    if (bikeModelLoaded) {
        std::cout << "Bike model loaded successfully via Assimp" << std::endl;
    } else {
        std::cout << "Bike model not found, using procedural placeholder" << std::endl;
    }

    // Initialize sounds (graceful if files missing)
    // gameMusic = SoundManager(getResource("Music/game.mp3"), soundEngine.volMusic, true, &soundEngine);
    // sfxJump = SoundManager(getResource("SFX/jump.wav"), soundEngine.volSound, false, &soundEngine);
    // sfxCrash = SoundManager(getResource("SFX/crash.wav"), soundEngine.volSound, false, &soundEngine);
    // gameMusic.playSound();

    // Ensure cursor starts normal (third person default)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    while (!glfwWindowShouldClose(window)) {
        // Timing
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Clamp delta time to avoid huge jumps
        if (deltaTime > 0.05f) deltaTime = 0.05f;

        // Input
        handleInput(window);

        // Update
        bike.update(deltaTime);

        // Check collision
        if (level.checkCollisions(bike)) {
            // Game Over
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            // sfxCrash.playSound();
            // gameMusic.stopSound();

            float completionPct = level.getProgress(bike.posZ) * 100.0f;
            GameOverViewController gameOver;
            GameResult result = gameOver.main(window, completionPct);
            if (result == RESULT_RESTART) {
                reset();
                continue;
            }
            cleanup();
            return RESULT_QUIT;
        }

        // Check level completion
        if (level.isCompleted(bike.posZ)) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            // gameMusic.stopSound();

            VictoryViewController victory;
            GameResult result = victory.main(window);
            if (result == RESULT_RESTART) {
                reset();
                continue;
            }
            cleanup();
            return RESULT_QUIT;
        }

        // Update camera
        updateCamera();

        // === RENDER ===
        glClearColor(0.45f, 0.7f, 0.95f, 1.0f); // Sky blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        // Setup 3D shader
        gameShader.use();
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
            0.1f, 500.0f
        );
        glm::mat4 view = gameCamera.GetViewMatrix();
        gameShader.setMat4("projection", projection);
        gameShader.setMat4("view", view);
        gameShader.setVec3("viewPos", gameCamera.Position);
        setupLighting(gameShader);

        // Render 3D scene
        renderRoad(gameShader);
        renderObstacles(gameShader);
        renderBike(gameShader);

        // Render HUD (2D overlay)
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        renderHUD(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanup();
    return RESULT_QUIT;
}
