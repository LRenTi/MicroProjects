#include <iostream>
#include <time.h>
#include "character.h"
#include "world.h"

// function to detect OS
void detectOS(){ //check OS and change clear command
    #ifdef _WIN32
        std::cout << "OS: Windows" << std::endl << std::endl;
        #define CLEAR "cls"
    #elif __linux__
        #define CLEAR "clear"
        std::cout << "OS: Linux" << std::endl << std::endl;
    #elif __APPLE__
        #define CLEAR "clear"
        std::cout << "OS: MacOS" << std::endl << std::endl;
    #else
        std::cout << "Unknown OS" << std::endl << std::endl;
    #endif
}

// function for player input control
void inputControl(character &player, bool &quit){
    char input;
    while(1){
    std::cin >> input;

    while(input != 'w' && input != 's' && input != 'a' && input != 'd' && input != 'q'){
        std::cout << "Invalid input! Try again: ";
        std::cin >> input;
    }

    // Player Movement
    switch(input){
        case 'w':
            if (player.get_x() == 0) {
                std::cout << "You can't move up! Try again: ";
                continue;
            }
            player.moveUp();
            break;
        case 's':
            if(player.get_x() == 4){
                std::cout << "You can't move down! Try again: ";
                continue;;
            }
            player.moveDown();
            break;
        case 'a':
            if(player.get_y() == 0){
                std::cout << "You can't move left! Try again: ";
                continue;
            }
            player.moveLeft();
            break;
        case 'd':
            if(player.get_y() == 4){
                std::cout << "You can't move right! Try again: ";
                continue;
            }
            player.moveRight();
            break;
        case 'q':
            quit = true;
            break;
        default:
            std::cout << "Invalid input! Try again: ";
            continue;
    }
    break;
    }
}

// function for printing legend
void printLegend(character player, int relicCount){
    std::cout << "HP: " << player.get_hp() << " Relics: " << player.get_rp() << "/" << relicCount << std::endl;
    std::cout << "Level: S" << player.get_strength() << " I" << player.get_intelligence() << " D" << player.get_dexterity() << std::endl;
    std::cout << "Items: "; 
    
    if(player.get_totem_of_undying()){
        std::cout << "Totem of Undying";
        if(player.get_strength_potion() || player.get_intelligence_potion() || player.get_dexterity_potion() || player.get_dagger()){
            std::cout << ", ";
        }
        else {
            std::cout << " ";
        }
    }
    if(player.get_strength_potion()){
        std::cout << "Strength Potion";
        if(player.get_intelligence_potion() || player.get_dexterity_potion() || player.get_dagger()){
            std::cout << ", ";
        }
        else {
            std::cout << " ";
        }
    }
    if(player.get_intelligence_potion()){
        std::cout << "Intelligence Potion";
        if(player.get_dexterity_potion() || player.get_dagger()){
            std::cout << ", ";
        }
        else {
            std::cout << " ";
        }
    }
    if(player.get_dexterity_potion()){
        std::cout << "Dexterity Potion";
        if(player.get_dagger()){
            std::cout << ", ";
        }
        else {
            std::cout << " ";
        }
    }
    if(player.get_dagger()){
        std::cout << "Dagger";
    }
    std::cout << std::endl;
    std::cout << "Move: UP - w | DOWN - s | LEFT - a | RIGHT - d | QUIT - q" << std::endl;
    std::cout << "Legend: " << std::endl;
    std::cout << "P - Player" << std::endl;
    std::cout << "E - Enemy" << std::endl;
    std::cout << ". - Empty" << std::endl;
    std::cout << "S - Danger of Strength" << std::endl;
    std::cout << "I - Danger of Intelligence" << std::endl;
    std::cout << "D - Danger of Dexterity" << std::endl;
    std::cout << "O - Well" << std::endl;
    std::cout << "(*) - Relic" << std::endl;
    std::cout << std::endl;
}

int main(){
    character player;
        int lost_hp = 0;
        int total_rp = 0;
    character enemy;

    system(CLEAR);

    world gameworld;
        int next_level = 2;
        int curr_level = 1;
        int relicCount = 0;

    std::srand( (unsigned int) time(NULL)); //initialize random seed (utime using long int -> cast to unsigned (srand) int to avoid warning)

    bool quit = false;
    player.set_totem_of_undying(true);

    while(1){
        
        gameworld.generation(relicCount, enemy, curr_level);

        while(1){
            detectOS();
            std::cout << "Level: " << curr_level << std::endl;
            gameworld.display(player, enemy);
            printLegend(player, relicCount);

            inputControl(player, quit);
            system(CLEAR);
            if(quit){
                break;
            }

            player.fieldInteraction(gameworld, curr_level);
            player.enemyInteraction(enemy, curr_level, lost_hp);
            
            // save by totem of undying
            if(player.get_hp() <= 0 && player.get_totem_of_undying()){
                player.set_totem_of_undying(false);
                player.set_hp(1);
                std::cout << "You took fatal damage, but the totem of undying saved you!" << std::endl;
                if(enemy.get_hp() != curr_level ){
                    enemy.set_hp(0);
                }
                continue;
            }
            
            // for game over when enemy kills player
            if(player.get_hp() <= 0 && enemy.get_hp() >= 0 && (player.get_x() == enemy.get_x()) && (player.get_y() == enemy.get_y())){
                std::cout << "The Enemy killed you! Game Over!" << std::endl;
                break;
            }
            
            // game over by no hp
            if(player.get_hp() <= 0){
                std::cout << "You died! Game Over!" << std::endl;
                break;
            } 
            
            // player wins when all relics are collected
            else if(player.get_rp() == relicCount){
                std::cout << "You won!" << std::endl;
                curr_level++;
                break;
            }
        }
        
        if(quit){
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        
        // for game over
        if(player.get_hp() <= 0){
            //print stats
            std::cout << "Last World: " << std::endl;
            gameworld.display(player, enemy);
            std::cout << "Your Game Statistics: " << std::endl;
            std::cout << "Level reached: " << curr_level << std::endl;
            std::cout << "Total Relic Points: " << total_rp << std::endl;
            std::cout << "Goodbye! Thanks for playing!" << std::endl;
            break;
        }

        // next level
        if(curr_level == next_level){
            total_rp += player.get_rp();
            next_level++;
            relicCount = 0;
            player.set_rp(0);
            player.set_x(0);
            player.set_y(0);
            system(CLEAR);
            continue;
        }
        
    }
}
