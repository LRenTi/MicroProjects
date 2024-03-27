#include <string>

class Ability {
    public:

    std::string getName();
    int getHealthBoost();
    int getArmorBoost();
    int getStrengthBoost();

    protected:
    std::string name;
    int healthboost;
    int armorboost;
    int strengthboost;
};

class Master : public Ability {
    public:
    Master();
};

class Defender : public Ability {
    public:
    Defender();

};

class Fighter : public Ability {
    public:
    Fighter();
};

class Novice : public Ability {
    public:
    Novice(); 
};
