#pragma once

#include <vector>
#include <string>

class ScoreManager {
public:
    static void AddScore(double score);
    static std::vector<double> GetTopScores(int count = 5);
private:
    static std::string GetFilePath();
};
