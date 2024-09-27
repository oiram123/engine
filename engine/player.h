#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <map>
#include <algorithm>


class Player {
  public: 
    std::string name;
    int acceleration;
    int sprintSpeed;
    int aggression;
    int agility;
    int balance;
    int ballControl;
    int composure;
    int crossing;
    int curve;
    int defensiveAwareness;
    int interceptions;
    int jumping;
    int longPassing;
    int longShots;
    int penalties;
    int reactions;
    int shortPassing;
    int shotPower;
    int slidingTackle;
    int speed; 
    int stamina;
    int standingTackle;
    int strength;
    int vision;
    int volleys;
    int diving; // Goalkeeping attribute
    int handling; // Goalkeeping attribute
    int kicking; // Goalkeeping attribute
    int positioning; // Goalkeeping and field positioning
    int reflexes; // Goalkeeping attribute

        Player(std::string n, int acc, int spr, int agg, int agi, int bal, int bControl, 
           int comp, int cross, int cur, int defAw, int div, int hand, int inter, 
           int jump, int kick, int lPass, int lShots, int pens, int pos, int reac, 
           int reflex, int sPass, int sPower, int sTackle, int st, int vis, int volleys, int stam)
        : name(n), acceleration(acc), sprintSpeed(spr), aggression(agg), agility(agi), 
          balance(bal), ballControl(bControl), composure(comp), crossing(cross), 
          curve(cur), defensiveAwareness(defAw), diving(div), handling(hand), 
          interceptions(inter), jumping(jump), kicking(kick), longPassing(lPass), 
          longShots(lShots), penalties(pens), positioning(pos), reactions(reac), 
          reflexes(reflex), shortPassing(sPass), shotPower(sPower), slidingTackle(sTackle), 
          strength(st), vision(vis), volleys(volleys), stamina(stam) {}

      void updateStamina() {
        stamina -= 1;
      }

      int getEffectiveAttribute(int attribute) {
        return attribute * (stamina / 100.0);
      }

      //define the quality operation
      bool operator==(const Player& other) const {
        return name == other.name;
      }
};

#endif