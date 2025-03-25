#include "Leaderboard.hpp"

std::string Result::toCSV() const {
    return nickname + "," + std::to_string(score);
}

Result Result::fromCSV(const std::string& csvLine) {
    Result player;
    std::stringstream ss(csvLine);
    std::getline(ss, player.nickname, ',');
    std::string scoreStr;
    std::getline(ss, scoreStr, ',');
    player.score = std::stoi(scoreStr);
    return player;
}

void Leaderboard::addPlayer(const std::string& nickname, int score) {
    Result newPlayer{ nickname, score };
    players.push_back(newPlayer);
    sortPlayers();
    saveToFile();
}

void Leaderboard::loadFromFile() {
    players.clear();
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::ofstream infile(filename); // create file
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (!line.empty()) {
            try {
                Result player = Result::fromCSV(line);
                players.push_back(player);
            }
            catch (const std::exception& e) {
                std::cerr << "Error parsing CSV: " << line << "\n";
            }
        }
    }

    infile.close();
    sortPlayers();
}

void Leaderboard::saveToFile() const {
    std::ofstream outfile(filename, std::ios::trunc);
    if (!outfile.is_open()) {
        std::cerr << "Error opening file " << filename << "\n";
        return;
    }

    for (const auto& player : players) {
        outfile << player.toCSV() << "\n";
    }

    outfile.close();
}

void Leaderboard::sortPlayers() {
    std::sort(players.begin(), players.end(), [](const Result& a, const Result& b) {
        return a.score > b.score;
    });
}

ftxui::Elements Leaderboard::display() const {
    using namespace ftxui;
    Elements leaderboard;
    leaderboard.push_back(text("----- Leaderboard -----"));
    leaderboard.push_back(text("Num    Nickname        Result"));
    leaderboard.push_back(text("-------------------------------------\n"));
    int place = 1;
    for (const auto& player : players) {
        leaderboard.push_back(text(std::to_string(place++) + "    " + player.nickname + "        "
            + std::to_string(player.score) + "\n"));
    }
    leaderboard.push_back(text("-----------------------\n"));
    return leaderboard;
}

const std::vector<Result>& Leaderboard::getPlayers() const {
    return players;
}
