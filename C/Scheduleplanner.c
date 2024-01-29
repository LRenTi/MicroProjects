#include <stdio.h>

enum Type {f, b, p}; /* Typ: friends(f), business(b), pause(p) */

struct appointment {
    enum Type type; /* Typ: friends(f), business(b), pause(p) */
    int time; /* Beginn */
    int duration; /* Dauer */
};

/* Funktion zum Termin printen */
void printAppointments(struct appointment * schedule, int length) {
    for (int i = 0; i < length; i++) {
        printf("\n%d: ", i + 1);

        switch (schedule[i].type) {
            case f:
                printf("Freunde");
                break;
            case b:
                printf("Geschaeftlich");
                break;
            case p:
                printf("Pause");
                break;
            default:
                printf("\nUngueltige Eingabe!");
                break;
        }
        printf(": %d Uhr, Dauer %dh", schedule[i].time, schedule[i].duration);
    }
}

/* Termin als struct erstellen und gibt diesen dann über return weiter */
struct appointment createAppointment(){
    struct appointment appointment;
    char typeinput;

    while(1){
    printf("\nGib einen Typen ein: Freunde (f), Geschaeftlich (b), Pause (p): ");
    scanf(" %c",&typeinput);
        switch(typeinput) {
            case 'f':
                appointment.type = f;
                break;
            case 'b':
                appointment.type = b;
                break;
            case 'p':
                appointment.type = p;
                break;
            default:
                printf("\nUngueltige Eingabe!");
                continue;
        }
        break;
    }
    while(1){
        printf("\nGib eine Uhrzeit ein (8-21): ");
        scanf(" %d",&appointment.time);

        if(appointment.time < 8 || appointment.time > 21){
            printf("\nUngueltige Eingabe!");
            continue;
        }
        break;
    }
    while(1){
        int maxDuration = 14 - (appointment.time - 8);
        printf("\nGib eine Dauer ein (1-%d): ", maxDuration);
        scanf(" %d",&appointment.duration);

        if(appointment.duration < 1 || appointment.duration > maxDuration){
            printf("\nUngueltige Eingabe!");
            continue;
        }
        break;
    }
    return appointment;

}

/* Terminininformations funktion */
void printInformation(struct appointment *schedule, int length) {
    int collision = 0;
    int bCount = 0, fCount = 0, pCount = 0;

    for (int i = 0; i < length - 1; i++) {
        if (schedule[i].time + schedule[i].duration > schedule[i + 1].time) {
            collision = 1;
            printf("\nEntweder Termin %d oder Termin %d sollte abgesagt werden.", i + 1, i + 2);
        }
    }

    if (!collision) {
        printf("\nDer Kalender hat keine Kollisionen!");
    }

    for (int i = 0; i < length; i++) {
        switch (schedule[i].type) {
            case b:
                bCount++;
                break;
            case f:
                fCount++;
                break;
            case p:
                pCount++;
                break;
        }
    }

    for (int i = 0; i < length - 1; i++) {
        if (schedule[i].type != p && schedule[i + 1].type != p) {
            printf("\nZwischen Termin %d und %d wird eine Pause benoetigt.", i + 1, i + 2);
        }
    }

    printf("\nSie haben %d Geschaeftstermin(e), %d Termin(e) mit Freunden und %d Pause(n) in Ihrem Kalender.", bCount, fCount, pCount);
}

/* Hinzufügen des Termines in den Kalendar */
void addAppointment(struct appointment *schedule, int *length) {
    if (*length >= 10) {
        printf("\nDer Kalender ist voll!");
        return;
    }
    
    /* Erstellt über die Funktion createAppointment einen Termin und fügt ihn zu schedule hinzu */
    schedule[*length] = createAppointment();
    (*length)++;

    /* BubbleSort, um die Termine nach type: time zu sortieren */
    for (int i = 0; i < *length - 1; i++) {
        for (int j = 0; j < *length - i - 1; j++) {
            if (schedule[j].time > schedule[j + 1].time) {
                struct appointment temp = schedule[j];
                schedule[j] = schedule[j + 1];
                schedule[j + 1] = temp;
            }
        }
    }
}

/* Löschen eines Termines */
void deleteAppointment(struct appointment *schedule, int *length){
    if (*length == 0) {
        printf("\nDer Kalender ist leer!");
        return;
    }

    int toDelete;

    printAppointments(schedule,*length);
    printf("\nWelchen dieser Termine moechten Sie loeschen? (1-%d): ",*length);
    
    while(1){
        scanf(" %d",&toDelete);

        if(toDelete < 1 || toDelete > *length){
            printf("Ungueltige Eingabe!");
            continue;
        }
        break;
    
    }

    for(int i = toDelete - 1; i < *length - 1; i++){
        schedule[i] = schedule[i + 1];
    }

    (*length)--;
    printf("\nTermin %d wurde geloescht!", toDelete);
}

/* Start der Main */
int main(){
    struct appointment schedule[10];
    int length = 0; /* Anfangslänge des Schedule Arrays */
    char input;

    /* Start der While Schleife */
    while (1) {
        printf("\nNeuen Termin (n), Termin loeschen (d), Termine auflisten (l), Informationen ausgeben (i), Programm beenden (x)");
        printf("\nAuswahl: ");
        scanf(" %c", &input);

        if(input == 'x'){
            break; /* Schleife wird hier abgebrochen! */
        }

        switch(input) {
            case 'n':
                addAppointment(schedule,&length);
                break;
            case 'd':
                deleteAppointment(schedule,&length);
                break;
            case 'l':
                printAppointments(schedule,length);
                break;
            case 'i':
                printInformation(schedule,length);
                break;
            default:
                printf("\nUngueltige Eingabe!");
                break;
        }
    }
}



