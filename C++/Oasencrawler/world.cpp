#include <iostream>
#include "character.h"
#include "world.h"

class character;

int world::get_field(int x, int y){
    return field[x][y];
}

void world::set_field(int x, int y, int value){
    field[x][y] = value;
}

void world::generation(int &relicCount, character &enemy, int level){
    bool relicFound = false;
    enemy.set_hp(level);
    enemy.set_x(5);
    enemy.set_y(5);

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(i == 0 && j == 0){
                field[i][j] = EMPTY;
                continue;
            }
            int randNum = rand() % 10 + 1;
            if(randNum <= 4){
                field[i][j] = EMPTY;
            } else if(randNum <= 8){
                int randNum2 = rand() % 3;
                if(randNum2 == 0){
                    field[i][j] = DANGER_STRENGTH;
                } else if(randNum2 == 1){
                    field[i][j] = DANGER_DEXTERITY;
                } else {
                    field[i][j] = DANGER_INTELLIGENCE;
                }
            } else if(randNum == 9){
                field[i][j] = WELL;
            } else {
                field[i][j] = RELIC;
                relicFound = true;
                relicCount++;
            }
        }
    }

    //if no relic -> place one randomly
    if(!relicFound){
        int randX = rand() % 5;
        int randY = rand() % 5;
        // place relic not on player position
        while(randX == 0 && randY == 0){
            randX = rand() % 5;
            randY = rand() % 5;
        }
        field[randX][randY] = RELIC;
        relicCount++;
    }
    // Enemy Spawn
    if(level > 1){
        while(1){
            int randX = rand() % 5;
            int randY = rand() % 5;

            if(!(field[randX][randY] == RELIC) && (randX != 0 && randY != 0)){
                enemy.set_x(randX);
                enemy.set_y(randX);
                break;
            }
        }
    }
}

void world::display(character player, character enemy){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(i == player.get_x() && j == player.get_y()){
                std::cout << "  P   ";
            }
            else if(i == enemy.get_x() && j == enemy.get_y() && enemy.get_hp() > 0){
                std::cout << "  E   ";
            }
            else if(field[i][j] == EMPTY){
                std::cout << "  .   ";
            }
            else if(field[i][j] == DANGER_DEXTERITY){
                std::cout << "  D   ";
            }
            else if(field[i][j] == DANGER_STRENGTH){
                std::cout << "  S   ";
            }
            else if(field[i][j] == DANGER_INTELLIGENCE){
                std::cout << "  I   ";
            }
            else if(field[i][j] == WELL){
                std::cout << "  O   ";
            }
            else if(field[i][j] == RELIC){
                std::cout << " (*)  ";
            }

        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
}