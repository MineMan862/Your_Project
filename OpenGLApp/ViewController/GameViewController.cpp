#include "GameViewController.h"
#include "GameOverViewController.h"
#include <assimp/scene.h>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <wchar.h>

// Unit cube vertex data: position(3) + normal(3) + texcoord(2)
static const float cubeVerticesData[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    0.0f,  0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,
    1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  1.0f,  -0.5f,
    -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,
    0.0f,  -1.0f, 0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
    1.0f,  1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,
    1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f,  1.0f,  -0.5f,
    -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,
    0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  -0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,
    0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
    0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,
    -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,
    0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
    0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,
};

static const float groundVerticesData[] = {
    -1.0f, 0.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f,  0.0f, 1.0f,  0.0f,
    1.0f,  0.0f, 1.0f,  0.0f, 1.0f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
    1.0f,  0.0f, 1.0f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f, 1.0f,  0.0f,
    1.0f,  1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  1.0f,
    -1.0f, 0.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f,  0.0f,
};

void GameViewController::setupGeometry() {
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesData), cubeVerticesData,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);

  glGenVertexArrays(1, &groundVAO);
  glGenBuffers(1, &groundVBO);
  glBindVertexArray(groundVAO);
  glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(groundVerticesData), groundVerticesData,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);
}

void GameViewController::loadModels() {
  std::string bikePath = getResource("Models/Bici.obj");
  bikeModel.loadModel(bikePath);
  bikeModelLoaded = !bikeModel.meshes.empty();
  std::cout << "Bike model: " << (bikeModelLoaded ? "LOADED" : "FAILED") << " ("
            << bikeModel.meshes.size() << " meshes)" << std::endl;

  std::string furgoncinoPath = getResource("Models/Furgoncino.obj");
  furgoncinoModel.loadModel(furgoncinoPath);
  furgoncinoModelLoaded = !furgoncinoModel.meshes.empty();
  std::cout << "Furgoncino model: "
            << (furgoncinoModelLoaded ? "LOADED" : "FAILED") << " ("
            << furgoncinoModel.meshes.size() << " meshes)" << std::endl;

  std::string maceriePath = getResource("Models/Macerie.obj");
  macerieModel.loadModel(maceriePath);
  macerieModelLoaded = !macerieModel.meshes.empty();
  std::cout << "Macerie model: " << (macerieModelLoaded ? "LOADED" : "FAILED")
            << " (" << macerieModel.meshes.size() << " meshes)" << std::endl;

  std::string stradaPath = getResource("Models/Strada.obj");
  stradaModel.loadModel(stradaPath);
  stradaModelLoaded = !stradaModel.meshes.empty();
  std::cout << "Strada model: " << (stradaModelLoaded ? "LOADED" : "FAILED")
            << " (" << stradaModel.meshes.size() << " meshes)" << std::endl;

  std::string planePath = getResource("Models/plane.obj");
  planeModel.loadModel(planePath);
  planeModelLoaded = !planeModel.meshes.empty();
  std::cout << "Plane model: " << (planeModelLoaded ? "LOADED" : "FAILED")
            << " (" << planeModel.meshes.size() << " meshes)" << std::endl;
}

void GameViewController::setupLighting(Shader &shader) {
  shader.setVec3("dirLight.direction", glm::vec3(-1.0f, -1.0f, -1.0f));
  // Luce ambientale raddoppiata per schiarire le ombre
  shader.setVec3("dirLight.ambient", glm::vec3(0.9f, 0.9f, 0.9f));
  shader.setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
  shader.setVec3("dirLight.specular", glm::vec3(0.8f, 0.8f, 0.8f));
  shader.setFloat("shininess", 25.0f);
}

void GameViewController::renderCube(Shader &shader, glm::vec3 position,
                                    glm::vec3 scale, glm::vec3 color) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::scale(model, scale);
  shader.setMat4("model", model);
  shader.setBool("useTexture", false);
  shader.setBool("useMaterialColor", false);
  shader.setVec3("objectColor", color);
  shader.setFloat("alpha", 1.0f);
  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

void GameViewController::renderProceduralClock(Shader &shader,
                                               glm::mat4 baseMatrix,
                                               glm::vec3 color) {
  glm::mat4 faceMat = glm::scale(baseMatrix, glm::vec3(0.8f, 0.8f, 0.1f));
  shader.setMat4("model", faceMat);
  shader.setVec3("powerupColor", color);

  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glm::mat4 hourMat = glm::translate(baseMatrix, glm::vec3(0.15f, 0.0f, 0.06f));
  hourMat = glm::scale(hourMat, glm::vec3(0.3f, 0.08f, 0.05f));
  shader.setMat4("model", hourMat);

  shader.setVec3("powerupColor", glm::vec3(1.0f, 1.0f, 1.0f));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glm::mat4 minMat = glm::translate(baseMatrix, glm::vec3(0.0f, 0.2f, 0.06f));
  minMat = glm::scale(minMat, glm::vec3(0.08f, 0.4f, 0.05f));
  shader.setMat4("model", minMat);
  shader.setVec3("powerupColor", glm::vec3(1.0f, 1.0f, 1.0f));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(0);
}

void GameViewController::renderModel(Shader &shader, Model &mdl,
                                     glm::vec3 position, glm::vec3 scale,
                                     float rotationY) {
  glm::mat4 modelMat = glm::mat4(1.0f);
  modelMat = glm::translate(modelMat, position);
  if (rotationY != 0.0f) {
    modelMat = glm::rotate(modelMat, glm::radians(rotationY),
                           glm::vec3(0.0f, 1.0f, 0.0f));
  }
  modelMat = glm::scale(modelMat, scale);
  shader.setMat4("model", modelMat);
  shader.setFloat("alpha", 1.0f);

  for (unsigned int i = 0; i < mdl.meshes.size(); i++) {
    auto &mesh = mdl.meshes[i];
    if (!mesh.textures.empty()) {
      shader.setBool("useTexture", true);
      shader.setBool("useMaterialColor", false);
    } else {
      shader.setBool("useTexture", false);
      shader.setBool("useMaterialColor", true);
      shader.setVec3("materialDiffuse", mesh.materialDiffuse);
      shader.setVec3("materialSpecular", mesh.materialSpecular);
      shader.setFloat("materialShininess", mesh.materialShininess);
    }
    mesh.Draw(shader);
  }
}

void GameViewController::renderRoad(Shader &shader) {
  float roadHalfWidth = bike.laneWidth * 2.0f;
  float roadLength = -bike.posZ + 1000.0f;

  if (stradaModelLoaded) {

    float modelLength = 43.3f;
    float modelWidth = 7.13f;
    float targetWidth = roadHalfWidth * 2.0f;
    float roadScale = targetWidth / modelWidth;

    // Tile road segments along -Z
    float segmentLengthAfterScale = modelLength * roadScale;

    float startRoadZ = floor((bike.posZ + 40.0f) / segmentLengthAfterScale) *
                       segmentLengthAfterScale;
    float endRoadZ = bike.posZ - 300.0f;
    for (float z = startRoadZ; z > endRoadZ; z -= segmentLengthAfterScale) {
      glm::mat4 modelMat = glm::mat4(1.0f);
      modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, z));
      modelMat = glm::rotate(modelMat, glm::radians(-90.0f),
                             glm::vec3(0.0f, 1.0f, 0.0f));
      modelMat = glm::scale(modelMat, glm::vec3(roadScale, 1.0f, roadScale));
      modelMat = glm::translate(modelMat, glm::vec3(-20.65f, 0.0f, 0.0f));

      shader.setMat4("model", modelMat);
      shader.setFloat("alpha", 1.0f);

      // Road has texture
      for (unsigned int i = 0; i < stradaModel.meshes.size(); i++) {
        auto &mesh = stradaModel.meshes[i];
        if (!mesh.textures.empty()) {
          shader.setBool("useTexture", true);
          shader.setBool("useMaterialColor", false);
        } else {
          shader.setBool("useTexture", false);
          shader.setBool("useMaterialColor", true);
          shader.setVec3("materialDiffuse", mesh.materialDiffuse);
          shader.setVec3("materialSpecular", mesh.materialSpecular);
          shader.setFloat("materialShininess", mesh.materialShininess);
        }
        mesh.Draw(shader);
      }
    }
  } else {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model,
                           glm::vec3(0.0f, -0.01f, -roadLength / 2.0f + 10.0f));
    model =
        glm::scale(model, glm::vec3(roadHalfWidth, 1.0f, roadLength / 2.0f));
    shader.setMat4("model", model);
    shader.setBool("useTexture", false);
    shader.setBool("useMaterialColor", false);
    shader.setVec3("objectColor", glm::vec3(0.25f, 0.25f, 0.28f));
    shader.setFloat("alpha", 1.0f);
    glBindVertexArray(groundVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
  }

  if (planeModelLoaded) {
    float planeSize = 22.25f;
    float groundScale = 3.0f;
    float tileSize = planeSize * groundScale;
    float groundOffsetX = roadHalfWidth + tileSize / 2.0f;

    for (int side = -1; side <= 1; side += 2) {
      float startPlaneZ = floor((bike.posZ + 40.0f) / tileSize) * tileSize;
      float endPlaneZ = bike.posZ - 300.0f;
      for (float z = startPlaneZ; z > endPlaneZ; z -= tileSize) {
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat,
                                  glm::vec3(side * groundOffsetX, -0.02f, z));
        modelMat =
            glm::scale(modelMat, glm::vec3(groundScale, 1.0f, groundScale));
        shader.setMat4("model", modelMat);
        shader.setFloat("alpha", 1.0f);

        for (unsigned int i = 0; i < planeModel.meshes.size(); i++) {
          auto &mesh = planeModel.meshes[i];
          if (!mesh.textures.empty()) {
            shader.setBool("useTexture", true);
            shader.setBool("useMaterialColor", false);
          } else {
            shader.setBool("useTexture", false);
            shader.setBool("useMaterialColor", true);
            shader.setVec3("materialDiffuse", mesh.materialDiffuse);
            shader.setVec3("materialSpecular", mesh.materialSpecular);
            shader.setFloat("materialShininess", mesh.materialShininess);
          }
          mesh.Draw(shader);
        }
      }
    }
  } else {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-roadHalfWidth - 15.0f, -0.02f,
                                            -roadLength / 2.0f + 10.0f));
    model = glm::scale(model, glm::vec3(15.0f, 1.0f, roadLength / 2.0f));
    shader.setMat4("model", model);
    shader.setBool("useTexture", false);
    shader.setBool("useMaterialColor", false);
    shader.setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.15f));
    shader.setFloat("alpha", 1.0f);
    glBindVertexArray(groundVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(roadHalfWidth + 15.0f, -0.02f,
                                            -roadLength / 2.0f + 10.0f));
    model = glm::scale(model, glm::vec3(15.0f, 1.0f, roadLength / 2.0f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", glm::vec3(0.2f, 0.5f, 0.15f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
  }
}

void GameViewController::renderBike(Shader &shader) {
  glm::vec3 bikePos = bike.getPosition();

  if (bikeModelLoaded) {
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, bikePos);

    float targetX = bike.getXForLane(bike.targetLane);
    float diffX = targetX - bikePos.x;

    float maxTilt = 20.0f;
    float tiltAngle = (diffX / bike.laneWidth) * maxTilt;

    modelMat = glm::rotate(modelMat, glm::radians(-tiltAngle),
                           glm::vec3(0.0f, 1.0f, 0.5f));
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.8f, 0.0f));
    float bikeScale = 0.4f;
    modelMat = glm::scale(modelMat, glm::vec3(bikeScale, bikeScale, bikeScale));
    modelMat = glm::rotate(modelMat, glm::radians(180.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setFloat("alpha", 1.0f);
    float pedalRotation = glfwGetTime() * 300.0f;

    for (unsigned int i = 0; i < bikeModel.meshes.size(); i++) {
      auto &mesh = bikeModel.meshes[i];

      glm::mat4 meshMat = modelMat;

      if (mesh.name.find("Pedal") != std::string::npos) {
        glm::vec3 pivot = glm::vec3(0.0f, 0.35f, 0.15f);

        meshMat = glm::translate(meshMat, -pivot);
        meshMat = glm::rotate(meshMat, glm::radians(pedalRotation),
                              glm::vec3(1.0f, 0.0f, 0.0f));
        meshMat = glm::translate(meshMat, pivot);
      }

      shader.setMat4("model", meshMat);

      if (!mesh.textures.empty()) {
        shader.setBool("useTexture", true);
        shader.setBool("useMaterialColor", false);
      } else {
        shader.setBool("useTexture", false);
        shader.setBool("useMaterialColor", true);
        shader.setVec3("materialDiffuse", mesh.materialDiffuse);
        shader.setVec3("materialSpecular", mesh.materialSpecular);
        shader.setFloat("materialShininess", mesh.materialShininess);
      }
      mesh.Draw(shader);
    }
  } else {
    // Fallback
    renderCube(shader, bikePos + glm::vec3(0.0f, 0.5f, 0.0f),
               glm::vec3(0.4f, 0.8f, 1.2f), glm::vec3(0.2f, 0.55f, 0.8f));
    renderCube(shader, bikePos + glm::vec3(0.0f, 1.0f, 0.15f),
               glm::vec3(0.3f, 0.15f, 0.4f), glm::vec3(0.15f, 0.15f, 0.15f));
    renderCube(shader, bikePos + glm::vec3(0.0f, 0.9f, -0.5f),
               glm::vec3(0.7f, 0.1f, 0.1f), glm::vec3(0.3f, 0.3f, 0.3f));
    renderCube(shader, bikePos + glm::vec3(0.0f, 0.2f, -0.55f),
               glm::vec3(0.08f, 0.4f, 0.4f), glm::vec3(0.1f, 0.1f, 0.1f));
    renderCube(shader, bikePos + glm::vec3(0.0f, 0.2f, 0.45f),
               glm::vec3(0.08f, 0.4f, 0.4f), glm::vec3(0.1f, 0.1f, 0.1f));
  }
}

void GameViewController::renderObstacles(Shader &shader) {
  for (const auto &obs : level.obstacles) {
    if (!obs.active)
      continue;
    if (obs.posZ > bike.posZ + 15.0f || obs.posZ < bike.posZ - 80.0f)
      continue;

    glm::vec3 pos = obs.getPosition(bike.laneWidth);

    if (obs.type == OBSTACLE_TALL && furgoncinoModelLoaded) {
      float furgScale = 0.38f;

      glm::mat4 modelMat = glm::mat4(1.0f);
      modelMat = glm::translate(modelMat, glm::vec3(pos.x, 0.0f, pos.z));
      modelMat = glm::rotate(modelMat, glm::radians(obs.rotationY),
                             glm::vec3(0.0f, 1.0f, 0.0f));
      modelMat =
          glm::scale(modelMat, glm::vec3(furgScale, furgScale, furgScale));
      modelMat = glm::translate(modelMat, glm::vec3(0.0f, 2.0f, 0.0f));

      shader.setMat4("model", modelMat);
      shader.setFloat("alpha", 1.0f);

      for (unsigned int i = 0; i < furgoncinoModel.meshes.size(); i++) {
        auto &mesh = furgoncinoModel.meshes[i];
        if (!mesh.textures.empty()) {
          shader.setBool("useTexture", true);
          shader.setBool("useMaterialColor", false);
        } else {
          shader.setBool("useTexture", false);
          shader.setBool("useMaterialColor", true);
          shader.setVec3("materialDiffuse", mesh.materialDiffuse);
          shader.setVec3("materialSpecular", mesh.materialSpecular);
          shader.setFloat("materialShininess", mesh.materialShininess);
        }
        mesh.Draw(shader);
      }
    } else if (obs.type == OBSTACLE_LOW && macerieModelLoaded) {
      float macScale = 0.2f; // Increased scale

      glm::mat4 modelMat = glm::mat4(1.0f);
      modelMat = glm::translate(modelMat, glm::vec3(pos.x, 0.0f, pos.z));
      modelMat = glm::scale(modelMat, glm::vec3(macScale, macScale, macScale));
      // Center the model: shift by -center
      modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.8f, 0.0f));

      shader.setMat4("model", modelMat);
      shader.setFloat("alpha", 1.0f);

      for (unsigned int i = 0; i < macerieModel.meshes.size(); i++) {
        auto &mesh = macerieModel.meshes[i];
        shader.setBool("useTexture", false);
        shader.setBool("useMaterialColor", true);
        shader.setVec3("materialDiffuse", mesh.materialDiffuse);
        shader.setVec3("materialSpecular", mesh.materialSpecular);
        shader.setFloat("materialShininess", mesh.materialShininess);
        mesh.Draw(shader);
      }
    } else {
      // Fallback: colored cubes
      glm::vec3 scale = obs.getScale();
      renderCube(shader, pos, scale, obs.color);
    }
  }
}

void GameViewController::renderPowerUps() {
  powerupShader.use();
  glm::mat4 projection = glm::perspective(
      glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
  powerupShader.setMat4("projection", projection);
  powerupShader.setMat4("view", gameCamera.GetViewMatrix());
  powerupShader.setFloat("time", glfwGetTime());

  for (const auto &p : level.powerUps) {
    if (!p.active)
      continue;

    glm::vec3 pos = p.getPosition(bike.laneWidth);
    glm::vec3 pColor = (p.type == 0) ? glm::vec3(0.0f, 1.0f, 0.0f)
                                     : glm::vec3(1.0f, 0.0f, 0.0f);

    glm::mat4 baseMat = glm::mat4(1.0f);
    baseMat = glm::translate(baseMat, pos);
    baseMat = glm::rotate(baseMat, (float)glfwGetTime() * 2.0f,
                          glm::vec3(0.0f, 1.0f, 0.0f));
    renderProceduralClock(powerupShader, baseMat, pColor);
  }
}

void GameViewController::renderHUD(GLFWwindow *window) {
  shaderText.use();
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f,
                                    static_cast<float>(SCR_HEIGHT));
  shaderText.setMat4("projection", projection);

  float scale = static_cast<float>(SCR_HEIGHT) / 1080.0f;

  float barX = 50.0f * scale;
  float barY = static_cast<float>(SCR_HEIGHT) - 50.0f * scale;

  int score = static_cast<int>(-bike.posZ);
  std::stringstream ss;
  ss << score << " m";
  RenderText(shaderText, "Distanza " + ss.str(), barX, barY + 5.0f * scale,
             0.5f * scale, glm::vec3(1.0f, 1.0f, 1.0f));

  std::stringstream speedSS;
  speedSS << std::fixed << std::setprecision(1) << bike.speed;
  RenderText(shaderText, "Velocita: " + speedSS.str(), barX + 350.0f * scale,
             barY + 5.0f * scale, 0.4f * scale, glm::vec3(0.9f, 0.9f, 0.6f));

  float keyY = 80.0f * scale;
  float keyScale = 0.5f * scale;
  float centerX = static_cast<float>(SCR_WIDTH) / 2.0f;

  bool aPressed = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
  bool dPressed = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
  bool spacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
  bool cPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;

  glm::vec3 inactiveColor = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::vec3 activeColor = glm::vec3(1.0f, 1.0f, 1.0f);

  RenderText(shaderText, "[A] Sinistra", centerX - 280.0f * scale, keyY,
             keyScale, aPressed ? activeColor : inactiveColor);
  RenderText(shaderText, "[D] Destra", centerX + 50.0f * scale, keyY, keyScale,
             dPressed ? activeColor : inactiveColor);
  RenderText(shaderText, "[SPACE] Salta", centerX - 130.0f * scale,
             keyY - 35.0f * scale, keyScale,
             spacePressed ? activeColor : inactiveColor);
  RenderText(shaderText, "[C] Camera", centerX - 280.0f * scale,
             keyY - 70.0f * scale, 0.4f * scale,
             cPressed ? activeColor : inactiveColor);

  std::string cameraMode = isFirstPerson ? "1a Persona" : "3a Persona";
  RenderText(shaderText, "Camera: " + cameraMode, centerX + 50.0f * scale,
             keyY - 70.0f * scale, 0.4f * scale, glm::vec3(0.7f, 0.7f, 0.9f));
}

void GameViewController::handleInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    if (!aKeyWasPressed) {
      bike.moveLeft();
      aKeyWasPressed = true;
    }
  } else {
    aKeyWasPressed = false;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    if (!dKeyWasPressed) {
      bike.moveRight();
      dKeyWasPressed = true;
    }
  } else {
    dKeyWasPressed = false;
  }

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (!spaceKeyWasPressed) {
      if (!bike.isJumping) {
        bike.jump();
        sfxJump.playSound();
      }
      spaceKeyWasPressed = true;
    }
  } else {
    spaceKeyWasPressed = false;
  }

  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
    if (!cKeyWasPressed) {
      isFirstPerson = !isFirstPerson;
      if (isFirstPerson) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouseMove = true;
      } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        gameCamera.Yaw = -90.0f;
        gameCamera.Pitch = -15.0f;
      }
      cKeyWasPressed = true;
    }
  } else {
    cKeyWasPressed = false;
  }

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
    float yoffset = lastMouseY - ypos;
    lastMouseX = xpos;
    lastMouseY = ypos;

    gameCamera.ProcessMouseMovement(xoffset, yoffset);
  }
}

void GameViewController::updateCamera() {
  glm::vec3 bikePos = bike.getPosition();

  if (isFirstPerson) {
    gameCamera.Position = bikePos + glm::vec3(0.0f, 1.5f, 0.0f);
  } else {
    float camDistance = 8.0f;
    float camHeight = 5.0f;
    gameCamera.Position = bikePos + glm::vec3(0.0f, camHeight, camDistance);
    glm::vec3 target = bikePos + glm::vec3(0.0f, 1.0f, -5.0f);
    gameCamera.Front = glm::normalize(target - gameCamera.Position);
    gameCamera.Right = glm::normalize(
        glm::cross(gameCamera.Front, glm::vec3(0.0f, 1.0f, 0.0f)));
    gameCamera.Up =
        glm::normalize(glm::cross(gameCamera.Right, gameCamera.Front));
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

GameResult GameViewController::main(GLFWwindow *window) {
  setupGeometry();
  loadModels();
  level.generate();
  bike.reset();
  gameCamera = Camera(glm::vec3(0.0f, 5.0f, 8.0f));
  gameCamera.Yaw = -90.0f;
  gameCamera.Pitch = -15.0f;

  sfxJump = SoundManager(getResource("SFX/jump.mp3"), soundEngine.volSound,
                         false, &soundEngine);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  while (!glfwWindowShouldClose(window)) {
    const auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (deltaTime > 0.05f)
      deltaTime = 0.05f;

    handleInput(window);
    if (powerupTimer > 0.0f) {
      powerupTimer -= deltaTime;
      if (powerupTimer <= 0.0f) {
        currentSpeedMultiplier = 1.0f;
      }
    }
    int collected = level.checkPowerUp(bike);
    if (collected == 0) {
      currentSpeedMultiplier = 0.6f;
      powerupTimer = 7.0f;
    } else if (collected == 1) {
      currentSpeedMultiplier = 1.5f;
      powerupTimer = 8.0f;
    }
    float gameDeltaTime = deltaTime * currentSpeedMultiplier;
    bike.update(gameDeltaTime);
    level.update(bike.posZ);

    if (level.checkCollisions(bike)) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      GameOverViewController gameOver;
      GameResult result = gameOver.main(window, -bike.posZ);
      if (result == RESULT_RESTART) {
        reset();
        continue;
      }
      cleanup();
      return RESULT_QUIT;
    }

    /*if (level.isCompleted(bike.posZ)) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      VictoryViewController victory;
      GameResult result = victory.main(window);
      if (result == RESULT_RESTART) {
        reset();
        continue;
      }
      cleanup();
      return RESULT_QUIT;
    }*/

    updateCamera();

    glClearColor(0.45f, 0.7f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    gameShader.use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            static_cast<float>(SCR_WIDTH) /
                                                static_cast<float>(SCR_HEIGHT),
                                            0.1f, 500.0f);
    glm::mat4 view = gameCamera.GetViewMatrix();
    gameShader.setMat4("projection", projection);
    gameShader.setMat4("view", view);
    gameShader.setVec3("viewPos", gameCamera.Position);
    setupLighting(gameShader);

    renderRoad(gameShader);
    renderObstacles(gameShader);
    renderBike(gameShader);
    renderPowerUps();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    renderHUD(window);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cleanup();
  return RESULT_QUIT;
}
