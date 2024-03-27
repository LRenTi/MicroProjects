#include <iostream>
#include <string>
#include <vector>
#include "ability.hpp"

class Enemy;
class DecisionTree; // Stufe 3

class Character {
    public:
    Character();

    void setName(std::string name);
    std::string getName();
    void setWins(int wins);
    int getWins();
    void setLosses(int losses);
    int getLosses();
    void setHealth(int health);
    int getHealth();
    void setArmor(int armor);
    int getArmor();
    void setStrength(int strength);
    int getStrength();
    void addAbility(Ability* ability);
    Ability* getAbility1();
    Ability* getAbility2();
    int diceRoll(int sides);
    void printStats();

    std::string getString_Ability1();
    std::string getString_Ability2();
    void deleteAbilities();
    bool getAb1Used();
    bool getAb2Used();
    void setAb1Used(bool used);
    void setAb2Used(bool used);

    // Stufe 3
    void makeDecision();
    void setDecisionTree(DecisionTree* tree);

    protected:
    Ability* ab1;
    Ability* ab2;
    DecisionTree* decisionTree;
    bool ab1Used;
    bool ab2Used;

    std::string name;

    int health;
    int armor;
    int strength;
    int wins;
    int losses;
    
};

class Player : public Character {
    public:
    void createCharacter();
    void attack(Character& enemy);

};

class Enemy : public Character {
    public:
    void randomise();
    void attack(Character& player);
    
};


// STUFE 3 - Entscheidungsbäume
class DecisionTree {
public:
    virtual void makeDecision(Character& character) = 0; 
};


class AggressiveTree : public DecisionTree {
public:
    virtual ~AggressiveTree() {}
    void makeDecision(Character& character) override {
        character.setStrength(character.getStrength() + 5);
    }
};

class DefensiveTree : public DecisionTree {
public:
    virtual ~DefensiveTree() {}
    void makeDecision(Character& character) override {
        character.setArmor(character.getArmor() + 5);
    }
};

