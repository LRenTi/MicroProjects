#include <iostream>
#include <limits>
#include "game.hpp"

int Game::diceRoll(int rolls, int sides){
    int result = 0;
    for (int i = 0; i < rolls; i++){
        result += rand() % sides + 1;
    }
    return result;
}

int Game::handleInput(int maxChoice){
    int choice = 0;
    while(!(std::cin >> choice) || choice < 1 || choice > maxChoice){
        std::cin.clear(); // clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore the rest of the input
        std::cout << "Invalid choice. Please try again: ";
    }
    return choice;
}

int Game::startMenu(){
    std::cout << "###############################" << std::endl;
    std::cout << "    Welcome to Fight Club!    " << std::endl;
    std::cout << "          1. Start Game       " << std::endl;
    std::cout << "          2. Show Stats       " << std::endl;
    std::cout << "          3. Exit             " << std::endl;
    std::cout << "###############################" << std::endl;
    std::cout << "Enter your choice: ";
    int choice = handleInput(3);
    return choice;
}

int Game::GameMenu(){
    std::cout << "###############################" << std::endl;
    std::cout << "             1. PvC            " << std::endl;
    std::cout << "             2. PvP            " << std::endl;
    std::cout << "        3. TeamFight PvC       " << std::endl;
    std::cout << "        4. TeamFight PvP       " << std::endl;
    std::cout << "###############################" << std::endl;
    std::cout << "Enter your choice: ";
    int choice = handleInput(4);
    return choice;
}

int Game::chooseCharacter(){
    std::cout << "     Choose your Character " << std::endl;
    std::cout << "          1. Bjorn" << std::endl;
    std::cout << "          2. Pekeo" << std::endl;            
    std::cout << "      3. Create your own" << std::endl;
    std::cout << "###############################" << std::endl;
    std::cout << "Enter your choice: ";

    switch(handleInput(3)){
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        default:
            return 0;
    }
}

void Game::CharacterChoice(int charchoice, Player*& choosen, Player& Bjorn, Player& Pekeo, Player& ownPlayer, std::vector<Character*>& characterlist) {
    int BjornUsed = 0;
    int PekeoUsed = 0;
    for (Character* character : characterlist) {
        if (character->getName() == "Bjorn"){
            BjornUsed = 1;
        }
        if (character->getName() == "Pekeo"){
            PekeoUsed = 1;
        }
    }    

    while ((charchoice == 1 && BjornUsed == 1) || (charchoice == 2 && PekeoUsed == 1)) {

        if (charchoice == 1 && BjornUsed == 1) {
            std::cout << "Bjorn is already in use. Please choose another character." << std::endl;
        }

        if (charchoice == 2 && PekeoUsed == 1) {
            std::cout << "Pekeo is already in use. Please choose another character." << std::endl;
        }

        std::cout << "Enter your choice: ";
        charchoice = handleInput(3);
    }

    switch(charchoice)
    {
        case 1:
            choosen = &Bjorn;
            characterlist.push_back(&Bjorn);
            break;
        case 2:
            choosen = &Pekeo;
            characterlist.push_back(&Pekeo);
            break;
        case 3:
            createCharacter(ownPlayer);
            choosen = &ownPlayer;
            break;
        default:
            std::cout << "Error: Invalid Input" << std::endl;
            return;
    }
}

void Game::createCharacter(Player& player){
    player.deleteAbilities();
    int firstability = 0;
    std::string name;
    std::cout << "###############################" << std::endl;
    std::cout << "   Create your character" << std::endl;
    if(player.getName().empty()){
        std::cout << "Enter your character's name: ";
        std::cin >> name;
        player.setName(name);
    }
    else{
        std::cout << "Your character's name: " << player.getName() << std::endl;
    }
    std::cout << "Choose your Abilities" << std::endl;
    std::cout << "1. Master" << std::endl;
    std::cout << "2. Defender" << std::endl;
    std::cout << "3. Fighter" << std::endl;
    std::cout << "4. Novice" << std::endl;
    std::cout << "Enter your first choice: ";
    switch(handleInput(4)){
        case 1:
            player.addAbility(new Master());
            firstability = 1;
            break;
        case 2:
            player.addAbility(new Defender());
            firstability = 2;
            break;
        case 3:
            player.addAbility(new Fighter());
            firstability = 3;
            break;
        case 4:
            player.addAbility(new Novice());
            firstability = 4;
            break;
        default:
            break;
    }
    std::cout << "Enter your second choice: ";
    int secondChoice = handleInput(4);
    while (secondChoice == firstability) {
        std::cout << "You already chose that ability. Please choose another." << std::endl;
        std::cout << "Enter your second choice: ";
        secondChoice = handleInput(4);
    }
    switch (secondChoice) {
        case 1:
            player.addAbility(new Master());
            break;
        case 2:
            player.addAbility(new Defender());
            break;
        case 3:
            player.addAbility(new Fighter());
            break;
        case 4:
            player.addAbility(new Novice());
            break;
        default:
            break;
    }
    // STUFE 3
    std::cout << " Whats your Decision Tree?" << std::endl;
    std::cout << "1. Aggressive" << std::endl;
    std::cout << "2. Defensive" << std::endl;
    std::cout << "Enter your choice: ";
    switch(handleInput(2)){
        case 1:
            player.setDecisionTree(new AggressiveTree());
            break;
        case 2:
            player.setDecisionTree(new DefensiveTree());
            break;
        default:
            break;
    }
}

void Game::resetCharacter(Character& character){
    int starthealth = 100;
    if(character.getName() == "Bjorn" || character.getName() == "Pekeo"){
        character.setHealth(starthealth);
        character.setAb1Used(false);
        character.setAb2Used(false);
        return;
    }
    character.setHealth(starthealth);
    character.deleteAbilities();
    character.setAb1Used(false);
    character.setAb2Used(false);
}

void Game::transferPtE(Enemy& enemy, Player& player){ //Transfer Player attributes to Enemy
    enemy.setHealth(player.getHealth());
    enemy.setArmor(player.getArmor());
    enemy.setStrength(player.getStrength());
    enemy.setName(player.getName());
    enemy.addAbility(player.getAbility1());
    enemy.addAbility(player.getAbility2());
    enemy.setWins(player.getWins());
    enemy.setLosses(player.getLosses());
}

void Game::chooseOpponent(Enemy& enemy, Player firstOPP, Player secondOPP){
    std::cout << "###############################" << std::endl;
    std::cout << "     Choose your Opponent " << std::endl;
    std::cout << "          1. " << firstOPP.getName() << std::endl;
    std::cout << "          2. " << secondOPP.getName() << std::endl;            
    std::cout << "          3. Random" << std::endl;
    std::cout << "###############################" << std::endl;
    std::cout << "Enter your choice: ";
    switch(handleInput(3)){
        case 1:
            transferPtE(enemy, firstOPP);
            break;
        case 2:
            transferPtE(enemy, secondOPP);
            break;
        case 3:
            enemy.randomise();
            break;
        default:
            break;
    }
}

void Game::displayStatsSmall(Character& player, Character& enemy){
    std::cout << " " << player.getName() << "  Health: " << player.getHealth() << std::endl;
    std::cout << " " << enemy.getName() << "  Health: " << enemy.getHealth() << std::endl;
}

void Game::displayWinner1v1(Character& player1, Character& player2){
    if (player1.getHealth() <= 0 && player2.getHealth() > 0){
        std::cout << "  The winner is: " << player2.getName() << std::endl;

        player2.setWins(player2.getWins() + 1);
        player1.setLosses(player1.getLosses() + 1);

        resetCharacter(player1);
        resetCharacter(player2);

        return;
    }
    else if (player2.getHealth() <= 0 && player1.getHealth() > 0){
        std::cout << "  The winner is: " << player1.getName() << std::endl;

        player1.setWins(player1.getWins() + 1);
        player2.setLosses(player2.getLosses() + 1);
        
        resetCharacter(player1);
        resetCharacter(player2);

        return;
    }
    else {
        std::cout << "         It's a draw!         " << std::endl;

        resetCharacter(player1);
        resetCharacter(player2);

        return;
    }
}

void Game::displayWinnerTeam(TeamPlayer* team1, TeamComputer* team2){ // Stufe 2
    if ((*team1->getMember1()).getHealth() <= 0 && (*team1->getMember2()).getHealth() <= 0 && ((*team2->getMember1()).getHealth() > 0 || (*team2->getMember2()).getHealth() > 0)){
        std::cout << "  The winner is: Team2" << " " << (*team2->getMember1()).getName() << " and " << (*team2->getMember2()).getName() << std::endl;

        (*team2).getMember1()->setWins((*team2).getMember1()->getWins() + 1);
        (*team2).getMember2()->setWins((*team2).getMember2()->getWins() + 1);
        (*team1).getMember1()->setLosses((*team1).getMember1()->getLosses() + 1);
        (*team1).getMember2()->setLosses((*team1).getMember2()->getLosses() + 1);

        resetCharacter(*team1->getMember1());
        resetCharacter(*team1->getMember2());
        resetCharacter(*team2->getMember1());
        resetCharacter(*team2->getMember2());

        return;
    }
    else if ((*team2->getMember1()).getHealth() <= 0 && (*team2->getMember2()).getHealth() <= 0 && ((*team1->getMember1()).getHealth() > 0 || (*team1->getMember2()).getHealth() > 0)){
        std::cout << "  The winner is: Team1" << " " << (*team1->getMember1()).getName() << " and " << (*team1->getMember2()).getName() << std::endl;
        
        (*team1->getMember1()).setWins((*team1->getMember1()).getWins() + 1);
        (*team1->getMember2()).setWins((*team1->getMember2()).getWins() + 1);
        (*team2->getMember1()).setLosses((*team2->getMember1()).getLosses() + 1);
        (*team2->getMember2()).setLosses((*team2->getMember2()).getLosses() + 1);

        resetCharacter(*team1->getMember1());
        resetCharacter(*team1->getMember2());
        resetCharacter(*team2->getMember1());
        resetCharacter(*team2->getMember2());

        return;
    }
    else {
        std::cout << "         It's a draw!         " << std::endl;

        resetCharacter(*team1->getMember1());
        resetCharacter(*team1->getMember2());
        resetCharacter(*team2->getMember1());
        resetCharacter(*team2->getMember2());

        return;
    }
}

void Game::displayWinnerTeamPvP(TeamPlayer* team1, TeamPlayer* team2){ // Stufe 2
    if ((team1->getMember1()->getHealth() <= 0 && team1->getMember2()->getHealth() <= 0) && (team2->getMember1()->getHealth() > 0 || team2->getMember2()->getHealth() > 0)){
        std::cout << "  The winner is: Team2" << " " << team2->getMember1()->getName() << " and " << team2->getMember2()->getName() << std::endl;

        team2->getMember1()->setWins(team2->getMember1()->getWins() + 1);
        team2->getMember2()->setWins(team2->getMember2()->getWins() + 1);
        team1->getMember1()->setLosses(team1->getMember1()->getLosses() + 1);
        team1->getMember2()->setLosses(team1->getMember2()->getLosses() + 1);

        resetCharacter(*team1->getMember1());
        resetCharacter(*team1->getMember2());
        resetCharacter(*team2->getMember1());
        resetCharacter(*team2->getMember2());

        return;
    }
    else if ((team2->getMember1()->getHealth() <= 0 && team2->getMember2()->getHealth() <= 0) && (team1->getMember1()->getHealth() > 0 || team1->getMember2()->getHealth() > 0)){
        std::cout << "  The winner is: Team1" << " " << team1->getMember1()->getName() << " and " << team1->getMember2()->getName() << std::endl;
        
        team1->getMember1()->setWins(team1->getMember1()->getWins() + 1);
        team1->getMember2()->setWins(team1->getMember2()->getWins() + 1);
        team2->getMember1()->setLosses(team2->getMember1()->getLosses() + 1);
        team2->getMember2()->setLosses(team2->getMember2()->getLosses() + 1);

        resetCharacter(*team1->getMember1());
        resetCharacter(*team1->getMember2());
        resetCharacter(*team2->getMember1());
        resetCharacter(*team2->getMember2());

        return;
    }
    else {
        std::cout << "         It's a draw!         " << std::endl;

        resetCharacter(*team1->getMember1());
        resetCharacter(*team1->getMember2());
        resetCharacter(*team2->getMember1());
        resetCharacter(*team2->getMember2());

        return;
    }
}

void Game::checkNovice(Character* character){
    if (character->getAbility1()->getName() == "Novice" || character->getAbility2()->getName() == "Novice"){
            int boost = character->getAbility1()->getHealthBoost() + character->getAbility2()->getHealthBoost();
            character->setHealth(character->getHealth() + boost);
    }
}

void Game::fightPvC(Player& player, Enemy& enemy){
        int round = 1;
        checkNovice(&player);
        checkNovice(&enemy);

        std::cout << "Your character: " << player.getName() << std::endl;
        std::cout << "Your abilities: " << player.getString_Ability1() << " and " << player.getString_Ability2() << std::endl;
        std::cout << "Your enemy: " << enemy.getName() << std::endl;
        std::cout << "Your enemy's abilities: " << enemy.getString_Ability1() << " and " << enemy.getString_Ability2() << std::endl;
        std::cout << "###############################" << std::endl;
        std::cout << " Press 1 to start the Fight!" << std::endl;
        int choice = handleInput(1);
        if (choice == 1){
        }

        while((player.getHealth() > 0) && (enemy.getHealth() > 0)){
            std::cout << "###############################" << std::endl;
            std::cout << "          Round " << round << std::endl;
            int randNum = diceRoll(1, 2);
            if (randNum == 1){
                player.attack(enemy);
                enemy.attack(player);
            }
            else {
                enemy.attack(player);
                player.attack(enemy);
            }
            round++;
            displayStatsSmall(player, enemy);
        }
}

void Game::fightPvP(Player& player1, Player& player2){ // Stufe 2
    int round = 1;
    checkNovice(&player1);
    checkNovice(&player2);
    std::cout << "Player1: " << player1.getName() << std::endl;
    std::cout << "Abilities: " << player1.getString_Ability1() << " and " << player1.getString_Ability2() << std::endl;
    std::cout << "Player2: " << player2.getName() << std::endl;
    std::cout << "Abilities: " << player2.getString_Ability1() << " and " << player2.getString_Ability2() << std::endl;
    std::cout << "###############################" << std::endl;
    std::cout << " Press 1 to start the Fight!" << std::endl;
    int choice = handleInput(1);
    if (choice == 1){
    }

    while((player1.getHealth() > 0) && (player2.getHealth() > 0)){
        std::cout << "###############################" << std::endl;
        std::cout << "          Round " << round << std::endl;
        std::cout << std::endl;
        int randNum = diceRoll(1, 2);
        if (randNum == 1){
            std::cout << "# " << player1.getName() << " attacks first" << std::endl;
            player1.attack(player2);
            std::cout << "# " << player2.getName() << " attacks second" << std::endl;
            player2.attack(player1);
        }
        else {
            std::cout << "# " << player2.getName() << " attacks first" << std::endl;
            player2.attack(player1);
            std::cout << "# " << player1.getName() << " attacks second" << std::endl;
            player1.attack(player2);
        }
        round++;
        displayStatsSmall(player1, player2);
    }
}

int Game::chooseTarget(Character& target1, Character& target2){ // Stufe 2
    std::cout << "Choose your target" << std::endl;
    std::cout << "1. " << target1.getName() << std::endl;
    std::cout << "2. " << target2.getName() << std::endl;
    std::cout << "Enter your choice: ";
    int choice = handleInput(2);
    if (choice == 1){
        return 1;
    }
    else {
        return 2;
    }
}

void Game::attackTarget(Player& player, Character& target1, Character& target2){ // Stufe 2
    std::cout << std::endl;
    std::cout << "# " << player.getName() << " attacks" << std::endl;
    int choice = chooseTarget(target1, target2);
    if (choice == 1){
        player.attack(target1);
    }
    else {
        player.attack(target2);
    }

}

void Game::randomattackTarget(Enemy& player, Character& target1, Character& target2){ // Stufe 2
    std::cout << std::endl;
    std::cout << "# " << player.getName() << " attacks" << std::endl;
    int choice = diceRoll(1, 2);
    if (choice == 1){
        player.attack(target1);
    }
    else {
        player.attack(target2);
    }
}

void Game::fightTeamPvC(TeamPlayer& team1, TeamComputer& team2){ // Stufe 2
    int round = 1;
    checkNovice(team1.getMember1());
    checkNovice(team2.getMember1());
    checkNovice(team1.getMember2());
    checkNovice(team2.getMember2());
    std::cout << "###############################" << std::endl;
    std::cout << "          TeamFight PvC" << std::endl;
    std::cout << "###############################" << std::endl;
    std::cout << " Team1: " << team1.getMember1()->getName() << " and " << team1.getMember2()->getName() << std::endl;
    std::cout << " Team2: " << team2.getMember1()->getName() << " and " << team2.getMember2()->getName() << std::endl;
    std::cout << "###############################" << std::endl;
    std::cout << " Press 1 to start the Fight!" << std::endl;
    int choice = handleInput(1);
    if (choice == 1){
    }

    while((team1.getMember1()->getHealth() > 0 || team1.getMember2()->getHealth() > 0) && (team2.getMember1()->getHealth() > 0 || team2.getMember2()->getHealth() > 0)){
        std::cout << "###############################" << std::endl;
        std::cout << "          Round " << round << std::endl;
        int randNum = diceRoll(1, 2);
        if (randNum == 1){
            // Team 1 attacks first
            if(team1.getMember1()->getHealth() > 0){
                attackTarget(*team1.getMember1(), *team2.getMember1(), *team2.getMember2());
            }
            if(team1.getMember2()->getHealth() > 0){
                attackTarget(*team1.getMember2(), *team2.getMember1(), *team2.getMember2());
            }
            // Team 2 attacks second
            if(team2.getMember1()->getHealth() > 0){
                randomattackTarget(*team2.getMember1(), *team1.getMember1(), *team1.getMember2());
            }
            if(team2.getMember2()->getHealth() > 0){
                randomattackTarget(*team2.getMember2(), *team1.getMember1(), *team1.getMember2());
            }
        }
        // Team 2 attacks first
        else {
            if(team2.getMember1()->getHealth() > 0){
                randomattackTarget(*team2.getMember1(), *team1.getMember1(), *team1.getMember2());
            }
            if(team2.getMember2()->getHealth() > 0){
                randomattackTarget(*team2.getMember2(), *team1.getMember1(), *team1.getMember2());
            }
            // Team 1 attacks second
            if(team1.getMember1()->getHealth() > 0){
                attackTarget(*team1.getMember1(), *team2.getMember1(), *team2.getMember2());
            }
            if(team1.getMember2()->getHealth() > 0){
                attackTarget(*team1.getMember2(), *team2.getMember1(), *team2.getMember2());
            }
        }
        round++;
        std::cout << "###############################" << std::endl;
        std::cout << "Team1: " << std::endl;
        displayStatsSmall(*team1.getMember1(), *team1.getMember2());
        std::cout << "Team2: " << std::endl;
        displayStatsSmall(*team2.getMember1(), *team2.getMember2());

    }
}

void Game::fightTeamPvP(TeamPlayer& team1, TeamPlayer& team2){ // Stufe 2
    int round = 1;
    checkNovice(team1.getMember1());
    checkNovice(team2.getMember1());
    checkNovice(team1.getMember2());
    checkNovice(team2.getMember2());
    std::cout << "###############################" << std::endl;
    std::cout << "          TeamFight PvP" << std::endl;
    std::cout << "###############################" << std::endl;
    std::cout << " Team1: " << team1.getMember1()->getName() << " and " << team1.getMember2()->getName() << std::endl;
    std::cout << " Team2: " << team2.getMember1()->getName() << " and " << team2.getMember2()->getName() << std::endl;
    std::cout << "###############################" << std::endl;
    std::cout << " Press 1 to start the Fight!" << std::endl;
    int choice = handleInput(1);
    if (choice == 1){
    }

    while((team1.getMember1()->getHealth() > 0 || team1.getMember2()->getHealth() > 0) && (team2.getMember1()->getHealth() > 0 || team2.getMember2()->getHealth() > 0)){
        std::cout << "###############################" << std::endl;
        std::cout << "          Round " << round << std::endl;
        int randNum = diceRoll(1, 2);
        if (randNum == 1){
            // Team 1 attacks first
            if(team1.getMember1()->getHealth() > 0){
                Player* tempTarget1 = team2.getMember1();
                Player* tempTarget2 = team2.getMember2();
                attackTarget(*team1.getMember1(), *tempTarget1, *tempTarget2);
            }
            if(team1.getMember2()->getHealth() > 0){
                Player* tempTarget1 = team2.getMember1();
                Player* tempTarget2 = team2.getMember2();
                attackTarget(*team1.getMember2(), *tempTarget1, *tempTarget2);
            }
            // Team 2 attacks second
            if(team2.getMember1()->getHealth() > 0){
                Player* tempTarget1 = team1.getMember1();
                Player* tempTarget2 = team1.getMember2();
                attackTarget(*team2.getMember1(), *tempTarget1, *tempTarget2);
            }
            if(team2.getMember2()->getHealth() > 0){
                Player* tempTarget1 = team1.getMember1();
                Player* tempTarget2 = team1.getMember2();
                attackTarget(*team2.getMember2(), *tempTarget1, *tempTarget2);
            }
        }
        else {
            // Team 2 attacks first
            if(team2.getMember1()->getHealth() > 0){
                Player* tempTarget1 = team1.getMember1();
                Player* tempTarget2 = team1.getMember2();
                attackTarget(*team2.getMember1(), *tempTarget1, *tempTarget2);
            }
            if(team2.getMember2()->getHealth() > 0){
                Player* tempTarget1 = team1.getMember1();
                Player* tempTarget2 = team1.getMember2();
                attackTarget(*team2.getMember2(), *tempTarget1, *tempTarget2);
            }
            // Team 1 attacks second
            if(team1.getMember1()->getHealth() > 0){
                Player* tempTarget1 = team2.getMember1();
                Player* tempTarget2 = team2.getMember2();
                attackTarget(*team1.getMember1(), *tempTarget1, *tempTarget2);
            }
            if(team1.getMember2()->getHealth() > 0){
                Player* tempTarget1 = team2.getMember1();
                Player* tempTarget2 = team2.getMember2();
                attackTarget(*team1.getMember2(), *tempTarget1, *tempTarget2);
            }
        }
        round++;
        std::cout << "###############################" << std::endl;
        std::cout << "Team1: " << std::endl;
        displayStatsSmall(*team1.getMember1(), *team1.getMember2());
        std::cout << "Team2: " << std::endl;
        displayStatsSmall(*team2.getMember1(), *team2.getMember2());
    }
}
  