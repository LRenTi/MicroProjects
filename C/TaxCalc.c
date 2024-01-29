#include <stdio.h>
double guthaben = 100;

double calctax(double betrag){
    if(betrag <= 20){
        return 0;
    }
    else if(betrag <= 50){
        return (betrag - 20) * 0.1;
    }
    else if(betrag <= 200){
        return 30 * 0.1 + (betrag - 50) * 0.2;
    }
    else{
        return 30 * 0.1 + 150 * 0.2 + (betrag - 200) * 0.4;
    }
}

double addincome(double *guthaben, double betrag, double *totaltax){
    double tax = calctax(betrag);
    *totaltax += tax;
    *guthaben += betrag - tax;
    double income = betrag - tax;
    printf("\nGezahlte Steuern: %.2lf Euro",tax);
    printf("\nGuthaben: %.2lf Euro", *guthaben);
    return income;
}

void addtaxfree(double *guthaben, double betrag){
    *guthaben += betrag;
    printf("\nGuthaben: %.2lf Euro",*guthaben);
}

int addcost(double *guthaben, double betrag){
    if(*guthaben < betrag){
        printf("\nDiese Kosten koennen nicht bezahlt werden.");
        printf("\nGuthaben: %.2lf Euro", *guthaben);
        return 1;
    }
    else {
        *guthaben -= betrag;
        printf("\nGuthaben: %.2lf Euro", *guthaben);
        return 0;
    }
}

int main(){
    double income = 0;
    int count_income = 0;
    double cost = 0;
    int count_cost = 0;
    double totaltax = 0;

    printf("\nGuthaben: %.2f Euro",guthaben);

    while(1){
        char type;
        double betrag;

        printf("\nTyp:");
        scanf(" %c",&type);

        if(type == '='){
            break;
        }

        printf("\nBetrag in Euro:");
        scanf(" %lf",&betrag);

        if(betrag < 0){
            printf("\nDer Betrag kann nicht negativ sein.");
            printf("\nGuthaben: %.2lf Euro", guthaben);
            continue;
        }

        switch(type){
        case 's':
            addtaxfree(&guthaben,betrag);
            income += betrag;
            count_income++;
            break;
        case 'e':
            income += addincome(&guthaben,betrag,&totaltax);
            count_income++;
            break;
        case 'k':
            if(addcost(&guthaben,betrag)){
                break;
            }
            else {
                count_cost++;
                cost += betrag;
                break;
            }
        default:
            printf("Falsches Typ Zeichen eingegeben!");
            break;
        }
    }
    printf("\nRestguthaben: %.2lf Euro",guthaben);
    printf("\n%d Einnahmen mit durchschnittlichem Wert %.2lf Euro",count_income,count_income == 0 ? 0 : (income/count_income));
    printf("\n%d Ausgaben mit durchschnittlichem Wert %.2lf Euro",count_cost,count_cost == 0 ? 0 : (cost/count_cost));
    printf("\nGezahlte Steuern: %.2lf Euro",totaltax);
}
