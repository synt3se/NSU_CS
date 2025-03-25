#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

#include "ftxui/dom/elements.hpp"

struct Result {
    std::string nickname;
    int score;

    std::string toCSV() const;

    static Result fromCSV(const std::string& csvLine);
};

class Leaderboard {
private:
    std::vector<Result> players;
    std::string filename;

public:
    Leaderboard(const std::string& file) : filename(file) {
        loadFromFile();
    }

    void addPlayer(const std::string& nickname, int score);

    void loadFromFile();

    void saveToFile() const;

    void sortPlayers();

    ftxui::Elements display() const;

    const std::vector<Result>& getPlayers() const;
};

