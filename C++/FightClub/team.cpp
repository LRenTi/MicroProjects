#include "team.hpp"

void TeamPlayer::setMembers(Player* newMember1, Player* newMember2) {
    this->member1 = newMember1;
    this->member2 = newMember2;
}

Player* TeamPlayer::getMember1() {
    return member1;
}

Player* TeamPlayer::getMember2() {
    return member2;
}

void TeamComputer::setMembers(Enemy* newMember1, Enemy* newMember2) {
    this->member1 = newMember1;
    this->member2 = newMember2;
}

Enemy* TeamComputer::getMember1() {
    return member1;
}

Enemy* TeamComputer::getMember2() {
    return member2;
}
