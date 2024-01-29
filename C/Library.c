#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    Horror = 1,
    Abenteuer,
    Romantik,
    Sachbuch
} genre_type;

typedef struct {
    char title[32];
    genre_type genre;
    int year;
    int amount;
} book;

typedef struct booknode {
    book books;
    struct booknode* next;
} booknode;

typedef struct borrownode {
    char title[32];
    char name[32];
    struct borrownode* next;
} borrownode;


//Funktion zum Einfügen eines Buches in die Inventarliste.
void insertbook(booknode** inventory){
    // Speicher für ein neues Buchnode-Objekt reservieren
    struct booknode* newBooknode = (booknode*)malloc(sizeof(booknode));
    if(newBooknode == NULL){
        printf("\nFehler beim Speichern des Buches!");
        return;
    }

    // Titel des Buches vom Benutzer eingeben lassen
    printf("\nGeben Sie den Titel ein: ");
    scanf("%s", newBooknode->books.title);

    // Genre des Buches vom Benutzer eingeben lassen
    while(1){
        printf("\nGeben Sie das Genre ein. Horror (1), Abenteuer (2), Romantik (3), Sachbuch (4): ");
        scanf("%d", (int*)&newBooknode->books.genre);
        if(newBooknode->books.genre < 1 || newBooknode->books.genre > 4){
            printf("\nUngueltige Eingabe!");
            continue;
        } else {
            break;
        }
    }

    // Erscheinungsjahr des Buches vom Benutzer eingeben lassen
    while(1){
        printf("\nGeben Sie das Erscheinungsjahr ein: ");
        scanf("%d", &newBooknode->books.year);
        if(newBooknode->books.year <= 0){
            printf("\nUngueltige Eingabe!");
            continue;
        } else {
            break;
        }
    }

    // Anzahl der Exemplare des Buches vom Benutzer eingeben lassen
    while(1){
        printf("\nGeben Sie ein wieviele Exemplare vorhanden sind: ");
        scanf("%d", &newBooknode->books.amount);
        if(newBooknode->books.amount <= 0){
            printf("\nUngueltige Eingabe!");
            continue;
        } else {
            break;
        }
    }

    // Das neue Buchnode-Objekt am Anfang der Inventarliste einfügen
    newBooknode->next = *inventory;
    *inventory = newBooknode;
}

//Druckt die Liste der Bücher im Inventar aus.
void printList(booknode* inventory){
    int counter = 1;
    while(inventory != NULL){
        printf("\n%d: %s, ", counter, inventory->books.title);
        switch (inventory->books.genre) {
            case Horror:
                printf("Horror");
                break;
            case Abenteuer:
                printf("Abenteuer");
                break;
            case Romantik:
                printf("Romantik");
                break;
            case Sachbuch:
                printf("Sachbuch");
                break;
        }
        printf(" (%d)",inventory->books.year);
        inventory = inventory->next;
        counter++;
    }
    printf("\n");
}

//Zählt die Anzahl der Bücher in der Inventarliste.
int countBooks(booknode* inventory) {
    int count = 0;
    while (inventory != NULL) {
        count++;
        inventory = inventory->next;
    }
    return count;
}

// Funktion, um die Anzahl der ausgeliehenen Bücher zu zählen
int countBorrowedBooks(borrownode* borrowed) {
    int count = 0;
    while (borrowed != NULL) {
        count++;
        borrowed = borrowed->next;
    }
    return count;
}

// Funktion, um den Titel eines Buches aus dem Inventar anhand der Auswahl zu erhalten
char* getBookTitle(booknode* inventory, int choice) {
    int count = 1;
    while (inventory != NULL) {
        if (count == choice) {
            return inventory->books.title;
        }
        inventory = inventory->next;
        count++;
    }
    return NULL;
}

// Funktion, um den Titel eines ausgeliehenen Buches anhand der Auswahl zu erhalten
char* getBorrowedTitle(borrownode* borrowed, int choice) {
    int count = 1;
    while (borrowed != NULL) {
        if (count == choice) {
            return borrowed->title;
        }
        borrowed = borrowed->next;
        count++;
    }
    return NULL;
}

// Funktion, um die Länge der ausgeliehenen Bücherliste zu erhalten
int getLength(borrownode* head) {
    int length = 0;
    borrownode* current = head;

    while (current != NULL) {
        length++;
        current = current->next;
    }

    return length;
}

// Funktion, um zu überprüfen, ob ein Buch ausgeliehen ist
int isBookBorrowed(borrownode* borrowed, char* name, char* title) {
    while (borrowed != NULL) {
        if (strcmp(borrowed->name, name) == 0 && strcmp(borrowed->title, title) == 0) {
            return 1;
        }
        borrowed = borrowed->next;
    }
    return 0;
}

// Funktion, um die Anzahl eines Buches im Inventar zu verringern
void decreaseBookAmount(booknode* inventory, int choice) {
    int count = 1;
    while (inventory != NULL) {
        if (count == choice) {
            inventory->books.amount--;
            return;
        }
        inventory = inventory->next;
        count++;
    }
}

// Funktion, um die Anzahl eines Buches im Inventar zu erhöhen
void increaseBookAmount(booknode* inventory, char* title) {
    while (inventory != NULL) {
        if (strcmp(inventory->books.title, title) == 0) {
            inventory->books.amount++;
            break;
        }
        inventory = inventory->next;
    }
}

// Funktion, um ein Buch aus der ausgeliehenen Bücherliste zu entfernen
char* removeBook(borrownode** head, int position) {
    if (*head == NULL)
        return NULL;

    borrownode* temp = *head;

    // Wenn die Position 0 ist, wird das erste Element entfernt
    if (position == 0) {
        *head = temp->next;
        char* title = malloc(strlen(temp->title) + 1);
        strcpy(title, temp->title);
        free(temp);
        return title;
    }

    // Suche nach dem Element an der gegebenen Position
    for (int i=0; temp!=NULL && i<position-1; i++)
         temp = temp->next;

    // Wenn die Position ungültig ist oder das nächste Element nicht existiert, wird NULL zurückgegeben
    if (temp == NULL || temp->next == NULL)
         return NULL;

    // Das nächste Element wird aus der Liste entfernt
    borrownode *next = temp->next->next;
    char* title = malloc(strlen(temp->next->title) + 1);
    strcpy(title, temp->next->title);

    free(temp->next);
    temp->next = next;
    return title;
}

// Funktion, um die Anzahl eines Buches im Inventar zu erhalten
int getBookAmount(booknode* inventory, int choice) {
    booknode* temp = inventory;
    int count = 0;

    while (temp != NULL) {
        count++;
        if (count == choice) {
            return temp->books.amount;
        }
        temp = temp->next;
    }
    return 0;
}

//Funktion, um ein Buch aus dem Inventar auszuleihen.
void borrowbook(booknode* inventory, borrownode** borrowed){

    // Neuen Knoten für ausgeliehenes Buch erstellen
    borrownode* newBorrownode = (borrownode*)malloc(sizeof(borrownode));

    // Überprüfen, ob das Inventar leer ist
    if(inventory == NULL) {
        printf("\n\nEs sind keine Buecher im Inventar vorhanden.");
        return;
    }

    // Liste der Bücher im Inventar ausgeben
    printList(inventory);
    
    int choice;
    int bookcount = countBooks(inventory);
    while(1){
        // Benutzer nach dem gewünschten Buch fragen
        printf("\nWelchen Titel moechten Sie leihen? (1-%d): ", bookcount);
        scanf(" %d",&choice);
        if(choice < 1 || choice > bookcount){
            printf("\nUngueltige Eingabe!");
            continue;
        }
        else {
            break;
        }
    }

    // Den Namen des Benutzers abfragen
    char* name = malloc(sizeof(char) * 32);
    printf("\nGeben Sie Ihren Namen ein: ");
    scanf(" %s", name);

    // Überprüfen, ob das Buch bereits ausgeliehen wurde
    if(isBookBorrowed(*borrowed, name, getBookTitle(inventory, choice))){
        printf("\nSie haben diesen Titel bereits ausgeliehen!");
        free(name);
        return;
    }

    // Überprüfen, ob alle Exemplare des Buchs bereits ausgeliehen wurden
    if(getBookAmount(inventory, choice) == 0){
        printf("\nBereits alle Exemplare ausgeliehen!");
        free(name);
        return;
    }

    // Informationen zum ausgeliehenen Buch in den neuen Knoten kopieren
    strcpy(newBorrownode->title, getBookTitle(inventory, choice));
    strcpy(newBorrownode->name, name);
    
    // Den neuen Knoten am Anfang der Liste der ausgeliehenen Bücher einfügen
    newBorrownode->next = *borrowed;
    *borrowed = newBorrownode;
    
    // Die Anzahl der verfügbaren Exemplare des Buchs verringern
    decreaseBookAmount(inventory, choice);

    free(name);
}

// Diese Funktion ermöglicht es dem Benutzer, ein ausgeliehenes Buch zurückzugeben.
void returnBook(borrownode** borrowed, booknode** inventory) {
    // Überprüfen, ob keine Bücher ausgeliehen sind
    if (*borrowed == NULL) {
        printf("\n\nEs sind keine Titel ausgeliehen!");
        return;
    }

    // Bücher auflisten, die ausgeliehen wurden
    int count = 1;
    borrownode* temp = *borrowed;
    while (temp != NULL) {
        printf("\n%d: %s geliehen von %s", count, temp->title, temp->name);
        temp = temp->next;
        count++;
    }
    printf("\n");

    // Benutzer nach dem Buch fragen, das zurückgegeben werden soll
    int choice;
    int bookcount = countBorrowedBooks(*borrowed);
    printf("\nWelchen Titel moechten Sie retournieren? (1-%d): ", bookcount);
    scanf("%d", &choice);

    // Überprüfen, ob die Eingabe gültig ist
    if (choice < 1 || choice > getLength(*borrowed)) {
        printf("Ungueltige Eingabe!\n");
        return;
    }

    // Buch aus der Liste der ausgeliehenen Bücher entfernen und die Anzahl im Inventar erhöhen
    char* title = removeBook(borrowed, choice - 1);
    increaseBookAmount(*inventory, title);
    free(title);
}

// Funktion, um den Genre-Typ in einen String umzuwandeln
const char* genreToString(genre_type g) {
    switch (g) {
        case 1:
            return "Horror";
        case 2:
            return "Abenteuer";
        case 3:
            return "Romantik";
        case 4:
            return "Sachbuch";
        default:
            return "Unbekannt";
    }
}

// Funktion, um die Liste der Bücher in aufsteigender Reihenfolge zu drucken
void printListinOrder(booknode* head) {
    if (head == NULL) {
        return;
    }

    // Zählt die Anzahl der Bücher in der Liste
    int count = 0;
    booknode* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    // Erstellt ein Array und kopiere die Bücher hinein
    book books[count];
    temp = head;
    for (int i = 0; i < count; i++) {
        books[i] = temp->books;
        temp = temp->next;
    }

    // Sortiert das Array nach dem Erscheinungsjahr
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (books[i].year > books[j].year) {
                book tempNode = books[i];
                books[i] = books[j];
                books[j] = tempNode;
            }
        }
    }

    // Druckt das sortierte Array
    for (int i = 0; i < count; i++) {
        printf("\n%d: %s, %s (%d)", i + 1, books[i].title, genreToString(books[i].genre), books[i].year);
    }
    printf("\n");
}

// Funktion, um den Speicher freizugeben
void freeMemory(booknode** inventory, borrownode** borrowed){

    booknode* currentBook = *inventory;
    // Schleife zum Freigeben des Speichers für die Bücher im Inventar
    while(currentBook != NULL){
        // Speicher für den Titel des Buches freigeben
        booknode* nextBook = currentBook->next;
        // Speicher für das Buch freigeben
        free(currentBook);
        // Den nächsten Knoten auswählen
        currentBook = nextBook;
    }
    *inventory = NULL;

    borrownode* currentBorrowed = *borrowed;
    while(currentBorrowed != NULL){
        borrownode* nextBorrowed = currentBorrowed->next;
        free(currentBorrowed);
        currentBorrowed = nextBorrowed;
    }
    *borrowed = NULL; 
}

int main(void){
    booknode* inventory = NULL;
    borrownode* borrowed = NULL;
    char input;

    while(1){

        while(1){
        printf("\nNeues Buch eingeben (n), Buch ausleihen (b), Buch zurueckgeben (r), Buecher auflisten (l), Buecher sortieren (s), Programm beenden (x)");
        printf("\nAuswahl: ");
        scanf(" %c", &input);
        if(input == 'n' || input == 'b' || input == 'r' || input == 'l' || input == 's' || input == 'x'){
            break;
        } else {
            printf("\nUngueltige Eingabe!");
            continue;
        }
        }

        if(input == 'x'){
            break;
        }

        switch(input){
            case 'n':
                insertbook(&inventory);
                break;
            case 'b':
                borrowbook(inventory, &borrowed);
                break;
            case 'r':
                returnBook(&borrowed, &inventory);
                break;
            case 'l':
                printList(inventory);
                break;
            case 's':
                printListinOrder(inventory);
                break;
            default:
                printf("Ungueltige Eingabe!");
                break;
        }
    }
    freeMemory(&inventory, &borrowed);
}
