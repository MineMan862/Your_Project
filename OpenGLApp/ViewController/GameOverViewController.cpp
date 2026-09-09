#include "GameOverViewController.h"
#include <iomanip>
#include <sstream>
#include "DataClasses/ScoreManager.h"

GameResult GameOverViewController::main(GLFWwindow *window,
                                        float completionPercentage) {
  bool rKeyWasPressed = false;
  
  std::vector<double> topScores = ScoreManager::GetTopScores(5);

  while (!glfwWindowShouldClose(window)) {
    const auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Input
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      return RESULT_QUIT;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
      if (!rKeyWasPressed) {
        return RESULT_RESTART;
      }
    } else {
      rKeyWasPressed = false;
    }

    // Render
    glClearColor(0.15f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    shaderText.use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f,
                                      static_cast<float>(SCR_HEIGHT));
    shaderText.setMat4("projection", projection);

    float scale = static_cast<float>(SCR_HEIGHT) / 1080.0f;
    float centerX = static_cast<float>(SCR_WIDTH) / 2.0f;
    float centerY = static_cast<float>(SCR_HEIGHT) / 2.0f;

    // Title
    RenderText(shaderText, "GAME OVER", centerX - 220.0f * scale,
               centerY + 300.0f * scale, 2.0f * scale,
               glm::vec3(0.9f, 0.2f, 0.2f));

    // Completion percentage
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << completionPercentage;
    RenderText(shaderText, "Hai percorso " + ss.str() + " m",
               centerX - 140.0f * scale, centerY + 210.0f * scale, 0.7f * scale,
               glm::vec3(0.9f, 0.9f, 0.9f));

    // Instructions
    RenderText(shaderText, "[R] Ricomincia", centerX - 100.0f * scale,
               centerY + 120.0f * scale, 0.6f * scale,
               glm::vec3(0.7f, 0.7f, 0.7f));
    RenderText(shaderText, "[ESC] Esci", centerX - 70.0f * scale,
               centerY + 70.0f * scale, 0.6f * scale,
               glm::vec3(0.7f, 0.7f, 0.7f));

    // Top Scores in the center
    float scoreTitleX = centerX - 120.0f * scale;
    float scoreTitleY = centerY - 10.0f * scale;
    RenderText(shaderText, "Top 5 Score:", scoreTitleX, scoreTitleY, 0.8f * scale, glm::vec3(0.9f, 0.8f, 0.2f));
    for (size_t i = 0; i < topScores.size(); i++) {
        std::stringstream ssScore;
        ssScore << std::fixed << std::setprecision(1) << topScores[i];
        RenderText(shaderText, std::to_string(i + 1) + ". " + ssScore.str() + " m", 
                   scoreTitleX + 40.0f * scale, scoreTitleY - (i + 1) * 50.0f * scale, 0.7f * scale, glm::vec3(1.0f));
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  return RESULT_QUIT;
}
