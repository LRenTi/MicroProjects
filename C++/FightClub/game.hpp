#include <iostream>
#include <string>
#include "team.hpp"

class Game {
    public:

    int startMenu();
    int GameMenu();
    int handleInput(int maxChoice);
    int chooseCharacter();
    void CharacterChoice(int charchoice, Player*& choosen, Player& Bjorn, Player& Pekeo, Player& ownPlayer, std::vector<Character*>& characterlist);
    int diceRoll(int rolls, int sides);
    void createCharacter(Player& player);
    void transferPtE(Enemy& enemy, Player& player);
    void transferEtP(Player& player, Enemy& enemy);
    void chooseOpponent(Enemy& enemy, Player firstOPP, Player secondOPP);
    void resetCharacter(Character& character);
    void checkNovice(Character* character);
    
    void fightPvC(Player& player, Enemy& enemy);
    void fightPvP(Player& player1, Player& player2); // Stufe 2
    int chooseTarget(Character& target1, Character& target2); // Stufe 2
    void attackTarget(Player& player, Character& target1, Character& target2); // Stufe 2
    void randomattackTarget(Enemy& player, Character& target1, Character& target2); // Stufe 2
    void fightTeamPvC(TeamPlayer& team1, TeamComputer& team2); // Stufe 2
    void fightTeamPvP(TeamPlayer& team1, TeamPlayer& team2); // Stufe 2

    void displayStatsSmall(Character& player, Character& enemy);
    void displayWinner1v1(Character& player, Character& enemy);
    void displayWinnerTeam(TeamPlayer* team1, TeamComputer* team2); // Stufe 2
    void displayWinnerTeamPvP(TeamPlayer* team1, TeamPlayer* team2); // Stufe 2

};
