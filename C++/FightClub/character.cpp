#include <iostream>
#include <string>
#include "character.hpp"
#include <vector>

Character::Character(){
    health = 100;
    armor = 10;
    strength = 3;
    wins = 0;
    losses = 0;
    ab1 = nullptr;
    ab2 = nullptr;
    ab1Used = false;
    ab2Used = false;
}

void Character::setName(std::string newName){
    this->name = newName;
}

std::string Character::getName(){
    return name;
}

int Character::getWins(){
    return wins;
}

int Character::getLosses(){
    return losses;
}

void Character::setWins(int newWins){
    this->wins = newWins;
}

void Character::setLosses(int newLosses){
    this->losses = newLosses;
}

void Character::setHealth(int newHealth){
    this->health = newHealth;
}

int Character::getHealth(){
    return health;
}

void Character::setArmor(int newArmor){
    this->armor = newArmor;
}

int Character::getArmor(){
    return armor;
}

void Character::setStrength(int newStrength){
    this->strength = newStrength;
}

int Character::getStrength(){
    return strength;
}

void Character::addAbility(Ability* ability){
    if (ab1 == nullptr){
        ab1 = ability;
    }
    else if (ab2 == nullptr){
        ab2 = ability;
    }
    else {
        std::cout << "You already have two abilities. You can't have more than two." << std::endl;
        delete ability;
    }
}

int Character::diceRoll(int sides){
    return rand() % sides + 1;
}

void Character::printStats(){
    std::cout << name << " - Wins: " << wins << " Losses: " << losses << std::endl;
}

std::string Character::getString_Ability1(){
    std::string result = ab1->getName();
    return result;
}

std::string Character::getString_Ability2(){
    std::string result = ab2->getName();
    return result;
}

Ability* Character::getAbility1(){
    return ab1;
}

Ability* Character::getAbility2(){
    return ab2;
}

bool Character::getAb1Used(){
    return ab1Used;
}

bool Character::getAb2Used(){
    return ab2Used;
}

void Character::setAb1Used(bool used){
    ab1Used = used;
}

void Character::setAb2Used(bool used){
    ab2Used = used;
}

void Character::deleteAbilities(){
    if (ab1 != nullptr) {
        delete ab1;
        ab1 = nullptr;
    }
    if (ab2 != nullptr) {
        delete ab2;
        ab2 = nullptr;
    }
}

// ------ PLAYER CLASS ------
void Player::attack(Character& enemy){
    int damage = 0;
    int roll;
    int totalstrength = strength + ab1->getStrengthBoost() + ab2->getStrengthBoost();
    int totalarmor = enemy.getArmor() + enemy.getAbility1()->getArmorBoost() + enemy.getAbility2()->getArmorBoost() + diceRoll(5);

    std::cout << std::endl;
    std::cout << "1. Attack with an dice Roll" << std::endl;
    std::cout << "2. Use Ability: " << ab1->getName();
    if (ab1Used) {
        std::cout << " (already used)";
    }
    std::cout << std::endl;
    std::cout << "3. Use Ability: " << ab2->getName();
    if (ab2Used) {
        std::cout << " (already used)";
    }
    std::cout << std::endl;
    std::cout << "Enter your choice: ";
    int choice = 0;
    std::cin >> choice;
    while (choice != 1 && choice != 2 && choice != 3) {
        std::cout << "Invalid input. Please enter a valid choice: ";
        std::cin >> choice;
    }
    if(choice == 1){
        roll = diceRoll(20);
        std::cout << "You rolled a " << roll << " for attack." << std::endl;
    }
    else if(choice == 2 && !ab1Used){
        std::cout << "You used " << ab1->getName() << std::endl;
        roll = diceRoll(20) + 8;
        std::cout << "You rolled a " << roll << " for attack." << std::endl;
        damage += diceRoll(8);
        std::cout << "You rolled a " << damage << " extra damage." << std::endl;
        setAb1Used(true);
    }
    else if(choice == 3 && !ab2Used){
        std::cout << "You used " << ab2->getName() << std::endl;
        roll = diceRoll(20) + 8;
        std::cout << "You rolled a " << roll << " for attack." << std::endl;
        damage += diceRoll(8);
        std::cout << "You rolled a " << damage << " extra damage." << std::endl;
        setAb2Used(true);
    }
    else{
        std::cout << "You missed your Attack because you tried to use an used Ability" << std::endl;
        return;
    }

        if((roll + totalstrength) > totalarmor){
            int damageRoll = diceRoll(10);
            damage += damageRoll + totalstrength;
            std::cout << "Your strength increased the damage by " << totalstrength << std::endl;
            std::cout << "You rolled a " << damageRoll << " for damage." << std::endl;
            if(roll >= 20){
                damage *= 2;
                std::cout << "Critical hit!" << std::endl;
                std::cout << name <<" hit " << enemy.getName() << " for " << damage << " damage." << std::endl;
                enemy.setHealth(enemy.getHealth() - damage);
            }
            else if(roll == 1){
                damage = 0;
                int lostHealth = diceRoll(3);
                std::cout << "Critical miss! " << name << " attacked himself and lost " << lostHealth << "HP"<< std::endl;
                setHealth(getHealth() - lostHealth);

            }
            else{
                std::cout << name <<" hit " << enemy.getName() << " for " << damage << " damage." << std::endl;
                enemy.setHealth(enemy.getHealth() - damage);
            }
        }
        else{
            std::cout << "You missed!" << std::endl;
            return;
        }
}

// ------ ENEMY CLASS ------
void Enemy::randomise(){
    //deleteAbilities();
    std::string names[] = {"Gorak","Grom","Gul'dan","Thrall","Jaina","Sylvanas","Arthas","Uther","Anduin","Varian","Garrosh","Cairne","Vol'jin","Rexxar","Malfurion","Tyrande","Illidan","Maiev","Kael'thas","Sylvanas","Vashj","Azshara","Kil'jaeden","Archimonde","Mannoroth","Gul'dan","Ner'zhul","Durotan","Orgrim","Blackhand","Grom","Grommash","Kargath"};
    int num_names = sizeof(names) / sizeof(std::string);
    name = names[rand() % num_names];

    int randNum1 = rand() % 4 + 1;
    if (randNum1 == 1){
        addAbility(new Master());
    }
    else if (randNum1 == 2){
        addAbility(new Defender());
    }
    else if (randNum1 == 3){
        addAbility(new Fighter());
    }
    else if (randNum1 == 4){
        addAbility(new Novice());
    }
    int randNum2 = rand() % 4 + 1;
    while (randNum2 == randNum1){
        randNum2 = rand() % 4 + 1;
    }
    if (randNum2 == 1){
        addAbility(new Master());
    }
    else if (randNum2 == 2){
        addAbility(new Defender());
    }
    else if (randNum2 == 3){
        addAbility(new Fighter());
    }
    else if (randNum2 == 4){
        addAbility(new Novice());
    }
    int randNum3 = rand() % 2 + 1;
    if(randNum3 == 1){
        setDecisionTree(new AggressiveTree());
    }
    else{
        setDecisionTree(new DefensiveTree());
    }
}

void Enemy::attack(Character &player){
    int damage = diceRoll(5);
    int roll = diceRoll(20);
    int totalstrength = strength + ab1->getStrengthBoost() + ab2->getStrengthBoost();
    int totalarmor = player.getArmor() + player.getAbility1()->getArmorBoost() + player.getAbility2()->getArmorBoost();

    if((roll + totalstrength + 2) > totalarmor){
        damage = diceRoll(10) + totalstrength;
        if(roll == 20){
            damage *= 2;
            std::cout << "Critical hit!" << std::endl;
            std::cout << name <<" hit " << player.getName() << " for " << damage << " damage." << std::endl;
            player.setHealth(player.getHealth() - damage);
        }
        else if(roll == 1){
            damage = 0;
            int randNum = rand() % 2 + 1;
            if(randNum == 1){
                int lostHealth = diceRoll(3);
                std::cout << "Critical miss! " << name << " attacked himself and lost " << lostHealth << "HP"<< std::endl;
                setHealth(getHealth() - lostHealth);
            }
            else{
                std::cout << "Critical miss! " << name << " missed!" << std::endl;
            }
        }
        else{
            std::cout << name <<" hit " << player.getName() << " for " << damage << " damage." << std::endl;
            player.setHealth(player.getHealth() - damage);
        }
    }
    else{
        std::cout << name <<" missed!" << std::endl;
    }
}

// ------ STUFE 3 Decision Tree ------

void Character::makeDecision() {
        decisionTree->makeDecision(*this);
    }

void Character::setDecisionTree(DecisionTree* tree) {
    decisionTree = tree;
}
