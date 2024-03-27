#include "character.hpp"

class Team {
};

class TeamPlayer : public Team {
    public:
    void setMembers(Player* newMember1, Player* newMember2);
    Player* getMember1();
    Player* getMember2();

    private:
    Player* member1;
    Player* member2;
};

class TeamComputer : public Team {
    public:
    void setMembers(Enemy* newMember1, Enemy* newMember2);
    Enemy* getMember1();
    Enemy* getMember2();

    private:
    Enemy* member1;
    Enemy* member2;
};
