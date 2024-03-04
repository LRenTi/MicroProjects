#include <iostream>
#include "character.h"
#include "world.h"


// Getter
int character::get_x(){
    return x;
}

int character::get_y(){
    return y;
}

int character::get_hp(){
    return hp;
}

int character::get_rp(){
    return rp;
}

int character::get_strength(){
    return strength;
}

int character::get_intelligence(){
    return intelligence;
}

int character::get_dexterity(){
    return dexterity;
}

bool character::get_totem_of_undying(){
    return totem_of_undying;
}

bool character::get_strength_potion(){
    return strength_potion;
}

bool character::get_intelligence_potion(){
    return intelligence_potion;
}

bool character::get_dexterity_potion(){
    return dexterity_potion;
}

bool character::get_dagger(){
    return dagger;
}

// Setter
void character::set_x(int newX){
    this->x = newX;
}

void character::set_y(int newY){
    this->y = newY;
}

void character::set_hp(int newHP){
    this->hp = newHP;
}

void character::set_rp(int newRP){
    this->rp = newRP;
}

void character::set_totem_of_undying(bool newTotem){
    this->totem_of_undying = newTotem;
}

//Methoden

// Konstruktor
character::character(){
    x = 0;
    y = 0;
    hp = 5;
    rp = 0;
    strength = 1;
    intelligence = 1;
    dexterity = 1;
    totem_of_undying = false;
    strength_potion = false;
    intelligence_potion = false;
    dexterity_potion = false;
    dagger = false;
}

void character::moveUp(){
    x--;

}

void character::moveDown(){
    x++;
}

void character::moveLeft(){
    y--;
}

void character::moveRight(){
    y++;
}

void character::onDanger(int attribute, int level){
    int randNum = rand() % 6 + 1;
    if(randNum == 1){
        if(attribute == 1){ // STRENGTH
            int dice_str = (rand() % 20) + level;

            if(dice_str < strength + (rand() % 20)){ // Saving throw success
                std::cout << "You took damage, but your strength saved you!" << std::endl;
                return;
                }
            else if(strength_potion){ // Saving throw fail but potion
                std::cout << "You took damage, but the strength potion saved you!" << std::endl;
                strength_potion = false;
                return;
            }
            else { // Saving throw fail
                hp--;
                std::cout << "You took damage!" << std::endl;
            }
        }
        else if(attribute == 2){ // INTELLIGENCE
            int dice_int = (rand() % 20) + level;

            if(dice_int < intelligence + (rand() % 20)){ // Saving throw success
                std::cout << "You took damage, but your intelligence saved you!" << std::endl;
                return;
            }
            else if(intelligence_potion){ // Saving throw fail but potion
                std::cout << "You took damage, but the intelligence potion saved you!" << std::endl;
                intelligence_potion = false;
                return;
            } 
            else { // Saving throw fail
                hp--;
                std::cout << "You took damage!" << std::endl;
            }
        }
        else if(attribute == 3){ // DEXTERITY
            int dice_dext = (rand() % 20) + level;

            if(dice_dext < dexterity + (rand() % 20)){ // Saving throw success
                std::cout << "You took damage, but your dexterity saved you!" << std::endl;
                return;
            }
            else if(dexterity_potion){ // Saving throw fail but potion
                std::cout << "You took damage, but the dexterity potion saved you!" << std::endl;
                dexterity_potion = false;
                return;
            }
            else { // Saving throw fail
                hp--;
                std::cout << "You took damage!" << std::endl;
            }
        }
    }
    else if(attribute == 1){
        strength++;
    }
    else if(attribute == 2){
        intelligence++;
    }
    else if(attribute == 3){
        dexterity++;
    }
}

void character::addRelic(){
    rp++;
}

void character::addHealth(){
    hp++;
}

void character::useDagger(character &enemy){
    enemy.hp = 0;
    dagger = false;
}

void character::enemyInteraction(character &enemy, int curr_level, int &lost_hp){
    if(curr_level > 1 && enemy.hp > 0){
        // Player Fight enemy
        if(x == enemy.x && y == enemy.y && enemy.hp > 0){
            // Dagger kill
            if(dagger){
                useDagger(enemy);
                std::cout << "You killed the enemy with the dagger!"<< std::endl;
            }
            // normal fight
            else {
                lost_hp = enemy.hp;
                int dmg = hp;
                hp -= enemy.hp;
                enemy.hp -= dmg;
                if(hp > 0){
                    std::cout << "You killed the enemy! You lost " << lost_hp << " HP" << std::endl;
                }
            }

        }
        else {
            // Enemy move
            enemy.chase(*this);
            }
        if(x == enemy.x && y == enemy.y && enemy.hp > 0){
            if(dagger){
                useDagger(enemy);
                std::cout << "You killed the enemy with the dagger!"<< std::endl;
            }
            else {
                lost_hp = enemy.hp;
                int dmg = hp;
                hp -= enemy.hp;
                enemy.hp -= dmg;
                if(hp > 0){
                    std::cout << "You killed the enemy! You lost " << lost_hp << " HP" << std::endl;
                }
            }
        }
    }
}

void character::findItem(){
    int randNum = rand() % 25 + 1;
    if(randNum == 1 && !totem_of_undying){
        totem_of_undying = true;
        std::cout << "You found a Totem of Undying!" << std::endl;
    } else if(randNum == 2 && !strength_potion){
        strength_potion = true;
        std::cout << "You found a Strength Potion!" << std::endl;
    } else if(randNum == 3 && !intelligence_potion){
        intelligence_potion = true;
        std::cout << "You found an Intelligence Potion!" << std::endl;
    } else if(randNum == 4 && !dexterity_potion){
        dexterity_potion = true;
        std::cout << "You found a Dexterity Potion!" << std::endl;
    } else if(randNum == 5 && !dagger){
        dagger = true;
        std::cout << "You found a Dagger!" << std::endl;
    }

}

void character::fieldInteraction(world &gameworld, int level){
    if(gameworld.get_field(x,y) == DANGER_DEXTERITY){
        onDanger(DEXTERITY, level); 
        removeField(gameworld);
    } else if(gameworld.get_field(x,y) == DANGER_STRENGTH){
        onDanger(STRENGTH, level); 
        removeField(gameworld);
    } else if(gameworld.get_field(x,y) == DANGER_INTELLIGENCE){
        onDanger(INTELLIGENCE, level); 
        removeField(gameworld);
    } else if(gameworld.get_field(x,y) == WELL){
        addHealth(); 
        findItem();
        removeField(gameworld);
    } else if(gameworld.get_field(x,y) == RELIC){
        addRelic();
        findItem(); 
        removeField(gameworld);
    }
}

void character::removeField(world &gameworld){
    gameworld.set_field(x,y, EMPTY);
}

// Enemy move
void character::chase(character &player){
    int xgap = std::abs(player.get_x() - get_x());
    int ygap = std::abs(player.get_y() - get_y());

    if (xgap > ygap) {
        if (player.get_x() > get_x()) {
            set_x(get_x() + 1);
        } else {
            set_x(get_x() - 1);
        }
    } else {
        if (player.get_y() > get_y()) {
            set_y(get_y() + 1);
        } else {
            set_y(get_y() - 1);
        }
    }
}
