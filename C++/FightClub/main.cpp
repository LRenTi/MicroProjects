#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "game.hpp"

int main() {
    Game fightClub;
    Player ownPlayer;
    Player ownPlayer1;
    Player ownPlayer2;
    Player ownPlayer3;
    Player ownPlayer4;
    Player Bjorn;
        Bjorn.setName("Bjorn");
        Bjorn.addAbility(new Master);
        Bjorn.addAbility(new Defender);
        Bjorn.setDecisionTree(new AggressiveTree());
    Player Pekeo;
        Pekeo.setName("Pekeo");
        Pekeo.addAbility(new Fighter);
        Pekeo.addAbility(new Novice);
        Pekeo.setDecisionTree(new DefensiveTree());
    Enemy enemy;  // Solo Fight Enemy
    Enemy enemy1; // Team Fight Enemy
    Enemy enemy2; // Team Fight Enemy
    Player* choosen = nullptr;
    Player* choosen1 = nullptr;
    Player* choosen2 = nullptr;
    Player* choosen3 = nullptr;
    Player* choosen4 = nullptr;
    int choice = -1;

    while(choice != 0){
        choice = fightClub.startMenu();
        // Start Game
        if(choice == 1){
            int mode = fightClub.GameMenu();
            // Stufe 1: PvC
            if(mode == 1){
                // Choose Character
                int charchoice = fightClub.chooseCharacter();
                if (charchoice == 0){
                    break;
                    //return 0;
                }
                else if (charchoice == 1){
                    choosen = &Bjorn;
                }
                else if (charchoice == 2){
                    choosen = &Pekeo;
                }
                else if (charchoice == 3){
                    fightClub.createCharacter(ownPlayer);
                    choosen = &ownPlayer;
                }
                else {
                    std::cout << "Error: Invalid input" << std::endl;
                    break;
                }
                fightClub.chooseOpponent(enemy,Bjorn,Pekeo);
                fightClub.fightPvC(*choosen, enemy);
                fightClub.displayWinner1v1(*choosen, enemy);
            }
            // Stufe 2: PvP
            else if(mode == 2){
                std::vector<Character*> usedCharacters;

                std::cout << "Choose Player1's character" << std::endl;
                int charchoice1 = fightClub.chooseCharacter();
                fightClub.CharacterChoice(charchoice1, choosen1, Bjorn, Pekeo, ownPlayer1, usedCharacters);

                std::cout << "Choose Player2's character" << std::endl;
                int charchoice2 = fightClub.chooseCharacter();
                fightClub.CharacterChoice(charchoice2, choosen2, Bjorn, Pekeo, ownPlayer2, usedCharacters);
                fightClub.fightPvP(*choosen1, *choosen2);
                fightClub.displayWinner1v1(*choosen1, *choosen2);
            }
            // Stufe 2: TeamPvC
            else if(mode == 3){
                std::vector<Character*> usedCharacters;

                TeamPlayer* team1 = new TeamPlayer();
                TeamComputer* team2 = new TeamComputer();

                std::cout << "Choose Player1's character" << std::endl;
                int charchoice1 = fightClub.chooseCharacter();
                fightClub.CharacterChoice(charchoice1, choosen1, Bjorn, Pekeo, ownPlayer1, usedCharacters);

                std::cout << "Choose Player2's character" << std::endl;
                int charchoice2 = fightClub.chooseCharacter();
                fightClub.CharacterChoice(charchoice2, choosen2, Bjorn, Pekeo, ownPlayer2, usedCharacters);

                enemy1.randomise();
                enemy2.randomise();
                
                team1->setMembers(choosen1, choosen2);
                team2->setMembers(&enemy1, &enemy2);
                fightClub.fightTeamPvC(*team1, *team2);
                fightClub.displayWinnerTeam(team1, team2);

                delete team1;
                delete team2; 
            }
            // Stufe 2 TeamPvP
            else if(mode == 4){
              
                std::vector<Character*> usedCharacters;

                TeamPlayer* team1 = new TeamPlayer();
                TeamPlayer* team2 = new TeamPlayer();

                std::cout << "Choose Player1's character" << std::endl;
                int charchoice1 = fightClub.chooseCharacter();
                fightClub.CharacterChoice(charchoice1, choosen1, Bjorn, Pekeo, ownPlayer1, usedCharacters);

                std::cout << "Choose Player2's character" << std::endl;
                int charchoice2 = fightClub.chooseCharacter();
                fightClub.CharacterChoice(charchoice2, choosen2, Bjorn, Pekeo, ownPlayer2, usedCharacters);

                std::cout << "Choose Player3's character" << std::endl;
                int charchoice3 = fightClub.chooseCharacter();
                fightClub.CharacterChoice(charchoice3, choosen3, Bjorn, Pekeo, ownPlayer3, usedCharacters);

                std::cout << "Choose Player4's character" << std::endl;
                int charchoice4 = fightClub.chooseCharacter();
                fightClub.CharacterChoice(charchoice4, choosen4, Bjorn, Pekeo, ownPlayer4, usedCharacters);

                team1->setMembers(choosen1, choosen2);
                team2->setMembers(choosen3, choosen4);
                fightClub.fightTeamPvP(*team1, *team2);
                fightClub.displayWinnerTeamPvP(team1, team2);

                delete team1;
                delete team2;
            }
        }
        // Show Stats
        else if(choice == 2){
            Bjorn.printStats();
            Pekeo.printStats();
            if(!(ownPlayer.getName().empty())){
                ownPlayer.printStats();
            }
            if(!(ownPlayer1.getName().empty())){
                ownPlayer1.printStats();
            }
            if(!(ownPlayer2.getName().empty())){
                ownPlayer2.printStats();
            }
            if(!(ownPlayer3.getName().empty())){
                ownPlayer3.printStats();
            }
            if(!(ownPlayer4.getName().empty())){
                ownPlayer4.printStats();
            }
            if(!(enemy.getName().empty())){
                std::cout << "Enemy Single - Wins: " << enemy.getWins() << " Losses: " << enemy.getLosses() + enemy1.getLosses() + enemy2.getLosses()  << std::endl;
            }
            if (!(enemy1.getName().empty()) && !(enemy2.getName().empty())) {
                std::cout << "Enemy Team - Wins: " << enemy1.getWins() + enemy2.getWins() << " Losses: " << enemy1.getLosses() + enemy2.getLosses() << std::endl;
            }

        }
        // Exit
        else if(choice == 3){
            std::cout << "Exiting game..." << std::endl;
            break;
        }
    }

    std::cout << "Goodbye!" << std::endl;
    return 0;
}
