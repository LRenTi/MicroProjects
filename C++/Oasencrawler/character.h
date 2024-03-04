class world;

enum Attribute{
    STRENGTH = 1,
    INTELLIGENCE = 2,
    DEXTERITY = 3
};

class character{
    private:
        int x, y; // Position
        int hp; // Health
        int rp; // Relic Points
        // Levels
        int strength;
        int intelligence;
        int dexterity;
        // Items
        bool totem_of_undying;
        bool strength_potion;
        bool intelligence_potion;
        bool dexterity_potion;
        bool dagger;

    public:

        // Getter
        int get_x();
        int get_y();
        int get_hp();
        int get_rp();
        int get_strength();
        int get_intelligence();
        int get_dexterity();
        bool get_totem_of_undying();
        bool get_strength_potion();
        bool get_intelligence_potion();
        bool get_dexterity_potion();
        bool get_dagger();

        // Setter
        void set_x(int x);
        void set_y(int y);
        void set_hp(int hp);
        void set_rp(int rp);
        void set_totem_of_undying(bool totem_of_undying);
        

        character();

        void moveUp();
        void moveDown();
        void moveLeft();
        void moveRight();
        void onDanger(int attribute, int level);
        void addRelic();
        void addHealth();
        void useDagger(character &enemy);
        void enemyInteraction(character &enemy, int curr_level, int &lost_hp);
        void findItem();
        void fieldInteraction(world &gameworld, int level);
        void removeField(world &gameworld);

        //Enemy
        void chase(character &player);

};
