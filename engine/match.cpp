#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include "player.h"

class Team {
public:
    std::string name;
    std::vector<Player> players;

    Team(std::string n, std::vector<Player> p)
        : name(n), players(p) {}

    void updatePlayerStamina() {
        for (auto& player : players){
            player.updateStamina();
        }
    }
};

class Match {
public:
    Team team1, team2;
    std::map<std::string, int> score;
    int time;

    Match(Team t1, Team t2)
        : team1(t1), team2(t2), time(0) {
            score[team1.name] = 0;
            score[team2.name] = 0;
    }

    Player& randomPlayer() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, team1.players.size() + team2.players.size() - 1);

        int player_index = dist(gen);
        
        if (player_index < team1.players.size()) {
            return team1.players[player_index];
        } else {
            return team2.players[player_index - team1.players.size()];
        }
    }

    void simulateMatch() {
        while (time < 90) {
            simulateEvent();
            team1.updatePlayerStamina();
            team2.updatePlayerStamina();
            time += 1;
        }
    }

    void simulateEvent() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 2);

        int event_type = dist(gen);
        
        if (event_type == 0) {
            simulatePass();
        } else if (event_type == 1) {
            simulateShot();
        } else {
            simulateTackle();
        }
    }

    void simulatePass() {
        Player& passer = randomPlayer();
        // Implement pass logic here
    }

    void simulateTackle() {
        Player& tackler = randomPlayer();
        // Implement tackle logic here
    }

    void simulateShot() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0.0, 1.0);

        Player& shooter = randomPlayer();
        double shot_success_chance = shooter.getEffectiveAttribute(shooter.shotPower) / 100.0;

        if (dist(gen) < shot_success_chance * 0.3) {
            if (std::find(team1.players.begin(), team1.players.end(), shooter) != team1.players.end()) {
                score[team1.name] += 1;
            } else {
                score[team2.name] += 1;
            }
        } else {
            // Implement missed shot logic here
        }
    }
};

// Simulate 10000 matches one after the other on a single thread
int main() {
    const int num_teams = 20000;
    const int num_matches = 10000;
    std::vector<Team> teams;

    // Create 20000 teams
    for (int i = 0; i < num_teams; ++i) {
        std::vector<Player> players;
        for (int j = 0; j < 11; ++j) {
            players.emplace_back("Player_" + std::to_string(i+1) + "_" + std::to_string(j+1),
                                75 + rand() % 10, 85 + rand() % 10, 65 + rand() % 10, 
                                70 + rand() % 10, 55 + rand() % 10,
                                75 + rand() % 10, 85 + rand() % 10, 65 + rand() % 10, 
                                70 + rand() % 10, 55 + rand() % 10,
                                75 + rand() % 10, 85 + rand() % 10, 65 + rand() % 10, 
                                70 + rand() % 10, 55 + rand() % 10,
                                75 + rand() % 10, 85 + rand() % 10, 65 + rand() % 10, 
                                70 + rand() % 10, 55 + rand() % 10,
                                75 + rand() % 10, 85 + rand() % 10, 65 + rand() % 10, 
                                70 + rand() % 10, 55 + rand() % 10, 65 + rand() % 10, 
                                70 + rand() % 10, 55 + rand() % 10);

        }
        teams.emplace_back("Team_" + std::to_string(i+1), players);
    }

    std::map<std::string, int> team_wins;
    std::map<std::string, int> team_draws;
    std::map<std::string, int> team_losses;

    for (int i = 0; i < num_matches; ++i) {
        // Randomly pick two different teams
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, num_teams - 1);

        int team1_index = dist(gen);
        int team2_index;

        do {
            team2_index = dist(gen);
        } while (team2_index == team1_index);

        Team& team1 = teams[team1_index];
        Team& team2 = teams[team2_index];

        Match match(team1, team2);
        match.simulateMatch();

        if (match.score[team1.name] > match.score[team2.name]) {
            team_wins[team1.name]++;
            team_losses[team2.name]++;
        } else if (match.score[team1.name] < match.score[team2.name]) {
            team_wins[team2.name]++;
            team_losses[team1.name]++;
        } else {
            team_draws[team1.name]++;
            team_draws[team2.name]++;
        }
    }

    // Output the results
    std::cout << "Match results after " << num_matches << " matches between " << num_teams << " teams:\n";
    for (const auto& team : teams) {
        std::string team_name = team.name;
        std::cout << team_name << " - Wins: " << team_wins[team_name] << ", Draws: " << team_draws[team_name] << ", Losses: " << team_losses[team_name] << "\n";
    }

    return 0;
}
