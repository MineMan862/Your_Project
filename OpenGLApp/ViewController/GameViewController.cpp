#include "GameViewController.h"
#include "GameOverViewController.h"
#include "DataClasses/ScoreManager.h"
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

  overlayShader = Shader(getResource("Shaders/overlay.vs").c_str(),
                         getResource("Shaders/overlay.fs").c_str());

  float overlayVertices[] = {
      0.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f
  };

  glGenVertexArrays(1, &overlayVAO);
  glGenBuffers(1, &overlayVBO);
  glBindVertexArray(overlayVAO);
  glBindBuffer(GL_ARRAY_BUFFER, overlayVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(overlayVertices), overlayVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  sliderShader = Shader(getResource("Shaders/slider.vs").c_str(),
                        getResource("Shaders/slider.fs").c_str());

  float sliderVertices[] = {
      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f
  };

  glGenVertexArrays(1, &sliderVAO);
  glGenBuffers(1, &sliderVBO);
  glBindVertexArray(sliderVAO);
  glBindBuffer(GL_ARRAY_BUFFER, sliderVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(sliderVertices), sliderVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
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

  std::string alberoPath = getResource("Models/albero.obj");
  alberoModel.loadModel(alberoPath);
  alberoModelLoaded = !alberoModel.meshes.empty();
  std::cout << "Albero model: " << (alberoModelLoaded ? "LOADED" : "FAILED")
            << " (" << alberoModel.meshes.size() << " meshes)" << std::endl;
}

void GameViewController::setupLighting(Shader &shader) {
  // Luce che piove dall'alto, da destra e da dietro le spalle del giocatore
  shader.setVec3("dirLight.direction", glm::vec3(-0.3f, -1.0f, -0.6f));
  // Luci bilanciate per maggiore contrasto e realismo
  shader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
  shader.setVec3("dirLight.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
  shader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
  shader.setFloat("shininess", 32.0f);
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

    if (alberoModelLoaded) {
      float treeSpacing = 20.0f;
      float startTreeZ = floor((bike.posZ + 40.0f) / treeSpacing) * treeSpacing;
      float endTreeZ = bike.posZ - 300.0f;
      float treeScale = 1.0f;
      
      for (float z = startTreeZ; z > endTreeZ; z -= treeSpacing) {
        // Left tree
        renderModel(shader, alberoModel, glm::vec3(-roadHalfWidth - 3.0f, 0.0f, z), glm::vec3(treeScale), 0.0f);
        // Right tree
        renderModel(shader, alberoModel, glm::vec3(roadHalfWidth + 3.0f, 0.0f, z), glm::vec3(treeScale), 0.0f);
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
    float pedalRotation = bike.pedalRotation;

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

  float dist = -bike.posZ;
  int score = static_cast<int>(dist);
  std::stringstream ss;
  ss << score;
  RenderText(shaderText, "Distanza " + ss.str() + " m", barX, barY + 5.0f * scale,
             0.5f * scale, glm::vec3(1.0f, 1.0f, 1.0f));

  std::stringstream speedSS;
  speedSS << std::fixed << std::setprecision(1) << bike.speed;
  RenderText(shaderText, "Velocita: " + speedSS.str(), barX + 350.0f * scale,
             barY + 5.0f * scale, 0.4f * scale, glm::vec3(1.0f, 1.0f, 1.0f));

  std::string diffStr = "Facile";
  glm::vec3 diffColor(0.2f, 0.9f, 0.2f); // Verde
  if (dist >= 1000.0f) {
      diffStr = "Difficile";
      diffColor = glm::vec3(0.9f, 0.2f, 0.2f); // Rosso
  } else if (dist >= 500.0f) {
      diffStr = "Media";
      diffColor = glm::vec3(0.9f, 0.9f, 0.2f); // Giallo
  }

  // Scrivi la difficoltà sotto la distanza (spostato più in basso sull'asse Y)
  RenderText(shaderText, "Difficolta: " + diffStr, barX,
             barY - 35.0f * scale, 0.4f * scale, diffColor);

  float keyY = 180.0f * scale;
  float keyScale = 0.5f * scale;
  float centerX = static_cast<float>(SCR_WIDTH) / 2.0f;

  bool aPressed = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
  bool dPressed = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
  bool spacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
  bool cPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
  bool mPressed = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;

  glm::vec3 inactiveColor = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::vec3 activeColor = glm::vec3(1.0f, 1.0f, 1.0f);

  float leftColX = centerX - 250.0f * scale;
  float rightColX = centerX + 50.0f * scale;
  float rowSpacing = 40.0f * scale;

  // Row 1
  RenderText(shaderText, "[A] Sinistra", leftColX, keyY,
             keyScale, aPressed ? activeColor : inactiveColor);
  RenderText(shaderText, "[D] Destra", rightColX, keyY, 
             keyScale, dPressed ? activeColor : inactiveColor);
             
  // Row 2
  RenderText(shaderText, "[SPACE] Salta", leftColX, keyY - rowSpacing, 
             keyScale, spacePressed ? activeColor : inactiveColor);
  RenderText(shaderText, "[M] Pausa", rightColX, keyY - rowSpacing, 
             keyScale, mPressed ? activeColor : inactiveColor);
             
  // Row 3
  RenderText(shaderText, "[C] Camera", leftColX, keyY - rowSpacing * 2.0f, 
             keyScale, cPressed ? activeColor : inactiveColor);
  std::string cameraMode = isFirstPerson ? "1a Persona" : "3a Persona";
  RenderText(shaderText, "Camera: " + cameraMode, rightColX, keyY - rowSpacing * 2.0f, 
             keyScale, glm::vec3(0.7f, 0.7f, 0.9f));
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
  
  powerupTimer = 0.0f;
  currentSpeedMultiplier = 1.0f;
  isPaused = false;
  isResuming = false;

  // Riavvia la musica del gioco
  gameMusic.playSound();
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
  if (overlayVAO) {
    glDeleteVertexArrays(1, &overlayVAO);
    glDeleteBuffers(1, &overlayVBO);
    overlayVAO = overlayVBO = 0;
  }
  if (sliderVAO) {
    glDeleteVertexArrays(1, &sliderVAO);
    glDeleteBuffers(1, &sliderVBO);
    sliderVAO = sliderVBO = 0;
  }
  gameMusic.stopSound();
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
                         0, &soundEngine);

  gameMusic = SoundManager(getResource("Music/Y2Mate.is - VELDA - 8 bit Win A Nice Final Boss Royalty Free Music.mp3"), soundEngine.volGameMusic, 2, &soundEngine);
  gameMusic.playSound();
  
  sfxCrash = SoundManager(getResource("SFX/crash.mp3"), soundEngine.volSound,
                          0, &soundEngine);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  float btnWidth = static_cast<float>(SCR_WIDTH) * 0.25f;
  float btnHeight = static_cast<float>(SCR_HEIGHT) * 0.08f;
  float centerX = static_cast<float>(SCR_WIDTH) / 2.0f;
  float centerY = static_cast<float>(SCR_HEIGHT) / 2.0f;
  float spacing = btnHeight * 1.5f;

  continueButton = Button(centerX - btnWidth / 2.0f, centerY + spacing, btnWidth, btnHeight);
  settingsButton = Button(centerX - btnWidth / 2.0f, centerY, btnWidth, btnHeight);
  exitButton = Button(centerX - btnWidth / 2.0f, centerY - spacing, btnWidth, btnHeight);

  float sliderWidth = btnWidth * 1.5f;
  float sliderHeight = btnHeight * 0.4f;
  musicSlider = Button(centerX - sliderWidth / 2.0f, centerY + spacing, sliderWidth, sliderHeight);
  soundSlider = Button(centerX - sliderWidth / 2.0f, centerY, sliderWidth, sliderHeight);
  backButton = Button(centerX - btnWidth / 2.0f, centerY - spacing * 1.5f, btnWidth, btnHeight);

  bool shouldQuit = false;

  while (!glfwWindowShouldClose(window)) {
    const auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (deltaTime > 0.05f)
      deltaTime = 0.05f;

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
      if (!mKeyWasPressed) {
        if (!isResuming) {
          isPaused = !isPaused;
          if (isPaused) {
            wasFirstPersonBeforePause = isFirstPerson;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
          } else {
            if (wasFirstPersonBeforePause) {
              glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
              firstMouseMove = true;
            }
          }
        }
        mKeyWasPressed = true;
      }
    } else {
      mKeyWasPressed = false;
    }

    if (!isPaused) {
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
        gameMusic.stopSound();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        ScoreManager::AddScore(static_cast<double>(-bike.posZ));
        GameOverViewController gameOver;
        GameResult result = gameOver.main(window, -bike.posZ);
        if (result == RESULT_RESTART) {
          reset();
          continue;
        }
        cleanup();
        return RESULT_QUIT;
      }

      updateCamera();
    } else {
      if (isResuming) {
        resumeTimer -= deltaTime;
        if (resumeTimer <= 0.0f) {
          isResuming = false;
          isPaused = false;
          if (wasFirstPersonBeforePause) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouseMove = true;
          }
        }
      } else {
        handlePauseInput(window, shouldQuit);
        if (shouldQuit) {
          cleanup();
          return RESULT_QUIT;
        }
      }
    }

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

    if (isPaused) {
      renderPauseMenu(window);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  cleanup();
  return RESULT_QUIT;
}

void GameViewController::handlePauseInput(GLFWwindow *window, bool &shouldQuit) {
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  const auto w_conv = static_cast<float>(SCR_WIDTH);
  const auto h_conv = static_cast<float>(SCR_HEIGHT);
  float xpos = (w_conv * static_cast<float>(mouseX) / static_cast<float>(width));
  float rawYPos = (h_conv * static_cast<float>(mouseY) / static_cast<float>(height));
  float ypos = h_conv - rawYPos;

  static bool ignoreMouseUntilRelease = false;
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
    ignoreMouseUntilRelease = false;
  }

  auto checkButton = [&](Button &btn) {
    if (xpos > btn.x && xpos < btn.x + btn.width &&
        ypos > btn.y && ypos < btn.y + btn.height) {
      btn.selected = true;
      if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!btn.clicked && !ignoreMouseUntilRelease) {
          btn.clicked = true;
          return true;
        }
      } else {
        btn.clicked = false;
      }
    } else {
      btn.selected = false;
      btn.clicked = false;
    }
    return false;
  };

  if (!isSettingsMenu) {
    if (checkButton(continueButton)) {
      isResuming = true;
      resumeTimer = 1.5f;
    }

    if (checkButton(settingsButton)) {
      isSettingsMenu = true;
      ignoreMouseUntilRelease = true;
    }

    if (checkButton(exitButton)) {
      shouldQuit = true;
    }
  } else {
    // Settings Menu Input
    if (checkButton(backButton)) {
      isSettingsMenu = false;
      ignoreMouseUntilRelease = true;
    }

    auto handleSlider = [&](Button &slider, float &volParam, auto setterFunc) {
      if (xpos >= slider.x && xpos <= slider.x + slider.width &&
          ypos >= slider.y - 10.0f && ypos <= slider.y + slider.height + 10.0f) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !ignoreMouseUntilRelease) {
          float relX = xpos - slider.x;
          volParam = (relX / slider.width) * 100.0f;
          if (volParam < 0.0f) volParam = 0.0f;
          if (volParam > 100.0f) volParam = 100.0f;
          setterFunc(volParam);
        }
      }
    };

    handleSlider(musicSlider, soundEngine.volGameMusic, [&](float v) {
      soundEngine.setVolGameMusic(v);
      gameMusic.changeVolume();
    });

    handleSlider(soundSlider, soundEngine.volSound, [&](float v) {
      soundEngine.setVolSuono(v);
      sfxJump.changeVolume();
      sfxCrash.changeVolume();
    });
  }
}

void GameViewController::renderPauseMenu(GLFWwindow *window) {
  // Draw semi-transparent background
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);

  overlayShader.use();
  glm::mat4 projOverlay = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f);
  overlayShader.setMat4("projection", projOverlay);
  overlayShader.setVec4("overlayColor", glm::vec4(0.25f, 0.25f, 0.25f, 0.85f));

  glBindVertexArray(overlayVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  shaderText.use();
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f,
                                    static_cast<float>(SCR_HEIGHT));
  shaderText.setMat4("projection", projection);

  float scale = static_cast<float>(SCR_HEIGHT) / 1080.0f;
  
  auto getTextWidth = [&](const std::string &text, float textScale) {
    float width = 0.0f;
    for (char c : text) {
      width += (Characters[c].Advance >> 6) * textScale;
    }
    return width;
  };

  auto getTextWidthLarge = [&](const std::string &text, float textScale) {
    float width = 0.0f;
    for (char c : text) {
      width += (CharactersLarge[c].Advance >> 6) * (textScale / 3.0f);
    }
    return width;
  };

  float centerX = static_cast<float>(SCR_WIDTH) / 2.0f;

  if (isResuming) {
    std::string countStr;
    if (resumeTimer > 1.0f) countStr = "3";
    else if (resumeTimer > 0.5f) countStr = "2";
    else countStr = "1";

    float textScale = 6.0f * scale;
    float tWidth = getTextWidthLarge(countStr, textScale);
    RenderTextLarge(shaderText, countStr, centerX - tWidth / 2.0f, 
               static_cast<float>(SCR_HEIGHT) / 2.0f - textScale * 10.0f, textScale, 
               glm::vec3(1.0f, 0.9f, 0.2f));
    return;
  }

  float titleScale = 2.0f * scale;
  float titleWidth = getTextWidth("PAUSA", titleScale);
  RenderText(shaderText, "PAUSA", centerX - titleWidth / 2.0f, 
             static_cast<float>(SCR_HEIGHT) * 0.8f, titleScale, 
             glm::vec3(0.9f, 0.8f, 0.2f));

  auto drawBtnText = [&](Button &btn, const std::string &text) {
     glm::vec3 color = btn.selected ? glm::vec3(1.0f, 1.0f, 0.2f) : glm::vec3(1.0f, 1.0f, 1.0f);
     
     float textScale = 0.8f * scale;
     float tWidth = getTextWidth(text, textScale);
     float textXOffset = (btn.width - tWidth) / 2.0f;
     
     RenderText(shaderText, text, btn.x + textXOffset, btn.y + btn.height * 0.25f, textScale, color);
     
     if (btn.selected) {
         float indicatorWidth = getTextWidth("> ", textScale);
         RenderText(shaderText, ">", btn.x + textXOffset - indicatorWidth, btn.y + btn.height * 0.25f, textScale, color);
     }
  };

  if (!isSettingsMenu) {
    drawBtnText(continueButton, "Continua");
    drawBtnText(settingsButton, "Impostazioni");
    drawBtnText(exitButton, "Torna al menu");
  } else {
    auto drawSlider = [&](Button &slider, float volParam, const std::string &label) {
        float textScale = 0.6f * scale;
        RenderText(shaderText, label, slider.x, slider.y + slider.height + 10.0f * scale, textScale, glm::vec3(1.0f));

        sliderShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(slider.x, slider.y, 0.0f));
        model = glm::scale(model, glm::vec3(slider.width, slider.height, 1.0f));
        
        sliderShader.setMat4("model", model);
        sliderShader.setMat4("projection", projection);
        sliderShader.setVec3("spriteColor", glm::vec3(0.2f, 0.8f, 0.2f));
        sliderShader.setFloat("fillPercentage", volParam / 100.0f);

        glBindVertexArray(sliderVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    };

    drawSlider(musicSlider, soundEngine.volGameMusic, "Volume Musica");
    drawSlider(soundSlider, soundEngine.volSound, "Volume Effetti");
    drawBtnText(backButton, "Indietro");
  }
}
