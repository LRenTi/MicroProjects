#include "ability.hpp"

std::string Ability::getName() {
    return (*this).name;
}

int Ability::getHealthBoost() {
    return healthboost;
}
int Ability::getArmorBoost() {
    return armorboost;
}
int Ability::getStrengthBoost() {
    return strengthboost;
}

Master::Master() {
    name = "Master";
    strengthboost = 3;
    healthboost = 0;
    armorboost = 0;
}

Defender::Defender() {
    name = "Defender";
    armorboost = 3;
    healthboost = 0;
    strengthboost = 0;
}

Fighter::Fighter() {
    name = "Fighter";
    strengthboost = 2;
    armorboost = 1;
    healthboost = 0;
}

Novice::Novice() {
    name = "Novice";
    healthboost = 20;
    armorboost = 1;
    strengthboost = 1;
}  
