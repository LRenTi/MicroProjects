enum FieldType{
    EMPTY = 0, //nothing happens
    DANGER_STRENGTH, // 1/6 chance of -1 HP
    DANGER_DEXTERITY, // 1/6 chance of -1 HP
    DANGER_INTELLIGENCE, // 1/6 chance of -1 HP
    WELL, // +1 HP
    RELIC // +1 relic
};


class world{
    private:

        int field[5][5];

    public:

    int get_field(int x, int y);
    void set_field(int x, int y, int value);

    void generation(int &relicCount, character &enemy, int level);
    void display(character player, character enemy);
};
