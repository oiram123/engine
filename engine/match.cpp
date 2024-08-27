#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include <mutex>
#include <thread>

class Player {
    public:
    std::string name;
    int speed, stamina, shooting, passing, defense;
    int energy;

    Player(std::string n, int spd, int stam, int sht, int pass, int def)
        : name(n), speed(spd), stamina(stam), shooting(sht), passing(pass), defense(def), energy(100){}
    
    void updateEnergy() {
        energy -= 1;
    }

    int getEffectiveAttribute(int attribute) {
        return attribute * (energy / 100.0);
    }

    //Define the equality operator

    bool operator==(const Player& other) const {
        return name == other.name;
    }
};

class Team {
public:
    std:: string name;
    std::vector<Player> players;

    Team(std::string n, std::vector<Player> p)
        : name(n), players(p){}

    void updatePlayerEnergy() {
        for (auto& player : players){
            player.updateEnergy();
        }
    }
};


class Match {
public:
    Team team1, team2;
    std::map<std::string, int> score;
    int time;

    Match(Team t1, Team t2)
        : team1(t1), team2(t2), time(0){
            score[team1.name] = 0;
            score[team2.name] = 0;
    }


    Player& randomPlayer() {
      std::random_device rd;
      std::mt19937 gen(rd());
      std:: uniform_int_distribution<> dist(0, team1.players.size());
    
      int player_index = dist(gen);

    //   std::cout << player_index << " The player INDEX" << "!\n";
     
      if(player_index < team1.players.size()) {
        return team1.players[player_index];
      } else {
        return team2.players[player_index - team1.players.size()];
      }
    }

    void simulateMatch() {
      while (time < 90) {
        simulateEvent();
        team1.updatePlayerEnergy();
        team2.updatePlayerEnergy();
        time += 1;
      }
    }

    void simulateEvent(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 2);

        int event_type = dist(gen);
        
        if(event_type == 0){
            simulatePass();
        } else if (event_type == 1){
            simulateShot();
        } else {
            simulateTackle();
        }
    }

    void simulatePass() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0.0, 1.0);
        
        Player& passer = randomPlayer();
        // std::cout << passer.name << "this is the passer" << "!\n";
    }

    void simulateTackle() {

    // std::cout << "simulateTackle" << "!\n";
  }

    void simulateShot(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0.0, 1.0);

        Player& shooter = randomPlayer();
        double shot_success_chance = shooter.getEffectiveAttribute(shooter.shooting) / 100.0;

        if(dist(gen) < shot_success_chance * 0.3){
           // std::cout << "Goal by" << shooter.name << "!\n";
            if (std::find(team1.players.begin(), team1.players.end(), shooter) != team1.players.end()) {
                score[team1.name] += 1;
            } else {
                score[team2.name] += 1;
            }
        } else {
            std::cout << shooter.name << " missed the shot.\n";
        }
    }
};


//Simulate 1000 match in one single thread

// Global mutex for thread safety when updating the shared results
std::mutex mtx;

void simulateMatchThread(Team& team1, Team& team2, std::map<std::string, int>& team_wins, std::map<std::string, int>& team_draws, std::map<std::string, int>& team_losses){
    Match match(team1, team2);
    match.simulateMatch();


    //Lock the mutex
    std::lock_guard<std::mutex> lock(mtx);

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


int main() {
    const int num_teams = 2000;
    const int num_matches = 1000;
    std::vector<Team> teams;

    for (int i = 0; i < num_teams; ++i){
        std::vector<Player> players;
        for(int j = 0; j < 11; ++j) {
            players.emplace_back("Player_" + std::to_string(i+1) + "_" + std::to_string(j+1), 75 + rand() % 10, 85 + rand() % 10, 65 + rand() % 10, 70 + rand() % 10, 55 + rand() % 10);
        }
        teams.emplace_back("Team_ " + std::to_string(i+1), players);
    }

    std::map<std::string, int> team_wins;
    std::map<std::string, int> team_draws;
    std::map<std::string, int> team_losses;

    std::vector<std::thread> threads;


    for (int i = 0; i < num_matches; ++i){
        //Randomly picking two diff teams
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

        //Creating the thread to simulate the match
        threads.emplace_back(simulateMatchThread, std::ref(team1), std::ref(team2), std::ref(team_wins), std::ref(team_draws), std::ref(team_losses));
    }

    for(auto& thread : threads){
        thread.join();
    }

    //Output
    std::cout << "Match results after " << num_matches << " matches between " << num_teams << " teams:\n";
    for(const auto& team : teams){
        std::string team_name = team.name;
        std::cout << team_name << " - Wins: " << team_wins[team_name] << ", Draws: " << team_draws[team_name] << ", Losses: " << team_losses[team_name] << "\n";
    }
}



// Simulate 1000 match one after one
// int main() {
//     const int num_teams = 2000;
//     const int num_matches = 1000;
//     std::vector<Team> teams;

//     // Create 2000 teams
//     for (int i = 0; i < num_teams; ++i) {
//         std::vector<Player> players;
//         for (int j = 0; j < 11; ++j) {
//             players.emplace_back("Player_" + std::to_string(i+1) + "_" + std::to_string(j+1), 75 + rand() % 10, 85 + rand() % 10, 65 + rand() % 10, 70 + rand() % 10, 55 + rand() % 10);
//         }
//         teams.emplace_back("Team_" + std::to_string(i+1), players);
//     }

//     std::map<std::string, int> team_wins;
//     std::map<std::string, int> team_draws;
//     std::map<std::string, int> team_losses;

//     for (int i = 0; i < num_matches; ++i) {
//         // Randomly pick two different teams
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_int_distribution<> dist(0, num_teams - 1);

//         int team1_index = dist(gen);
//         int team2_index;

//         do {
//             team2_index = dist(gen);
//         } while (team2_index == team1_index);

//         Team& team1 = teams[team1_index];
//         Team& team2 = teams[team2_index];

//         Match match(team1, team2);
//         match.simulateMatch();

//         if (match.score[team1.name] > match.score[team2.name]) {
//             team_wins[team1.name]++;
//             team_losses[team2.name]++;
//         } else if (match.score[team1.name] < match.score[team2.name]) {
//             team_wins[team2.name]++;
//             team_losses[team1.name]++;
//         } else {
//             team_draws[team1.name]++;
//             team_draws[team2.name]++;
//         }
//     }

//     // Output the results
//     std::cout << "Match results after " << num_matches << "matches between " << num_teams << " teams:\n";
//     for (const auto& team : teams) {
//         std::string team_name = team.name;
//         std::cout << team_name << " - Wins: " << team_wins[team_name] << ", Draws: " << team_draws[team_name] << ", Losses: " << team_losses[team_name] << "\n";
//     }

//     return 0;
// }


//Simulate 1 match
// Example usage
// int main() {
//   std::vector<Player> players_team1;
//    for (int i = 0; i < 11; ++i) {
//        players_team1.emplace_back("Player1_" + std::to_string(i+1), 80, 90, 70, 60, 50);
//    }
//     std::vector<Player> players_team2;
//     for (int i = 0; i < 11; ++i) {
//         players_team2.emplace_back("Player2_" + std::to_string(i+1), 75, 85, 65, 70, 55);
//     }

//     Team team1("Team1", players_team1);
//     Team team2("Team2", players_team2);

//     Match match(team1, team2);
//     match.simulateMatch();
//     std::cout << team1.name << " " << match.score[team1.name] << " - " << match.score[team2.name] << " " << team2.name << std::endl;

//     return 0;
// }


