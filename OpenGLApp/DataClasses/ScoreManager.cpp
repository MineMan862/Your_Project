#include "ScoreManager.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <filesystem>

#ifndef RESOURCE_PATH
#define RESOURCE_PATH "resources"
#endif

std::string ScoreManager::GetFilePath() {
    return std::string(RESOURCE_PATH) + "/scores.txt";
}

void ScoreManager::AddScore(double score) {
    std::ofstream file(GetFilePath(), std::ios::app);
    if (file.is_open()) {
        file << score << "\n";
        file.close();
    }
}

std::vector<double> ScoreManager::GetTopScores(int count) {
    std::vector<double> scores;
    std::ifstream file(GetFilePath());
    if (file.is_open()) {
        double score;
        while (file >> score) {
            scores.push_back(score);
        }
        file.close();
    }
    
    std::sort(scores.begin(), scores.end(), std::greater<double>());
    if (scores.size() > static_cast<size_t>(count)) {
        scores.resize(count);
    }
    return scores;
}

