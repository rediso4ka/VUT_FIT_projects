#include <stdio.h>
#include <stdlib.h>

int prav_1(char heslo[102]);
int prav_2(char heslo[102], int X);
int prav_3 (char heslo[102], int X);
int prav_4 (char heslo[102], int X);
int stats(char heslo[102], int *ruzn, int *mindel, int *pocet, int *sumdel, float *prumdel);
int chyby_obv(int argc, char *argv[]);
int def_levelparamstats(int argc, char *argv[], int *level, int *param, int *st);
int kroky(int level, char heslo[102], int param);

int main(int argc, char *argv[]){
    char heslo[102] = {0};
    int ruzn[95] = {0};      //pole pro vypocet ruznych znaku
    int sumruzn = 0;        //tady bude pocet ruznych znaku
    int mindel = 0;         // dal bych 101 (max 100), ale potrebujeme taky resit pripad prazdneho hesla.txt
    int pocet = 0;          //pocet hesel
    int sumdel = 0;
    float prumdel = 0.0;    //nedaval bych to do zbytecne promenne, ale potrebujeme taky resit pripad prazdneho hesla.txt
    int st = 0;             //pokud =1, tak piseme stats
    int level = 0;
    int param = 0;
    if (def_levelparamstats(argc, argv, &level, &param, &st)){       //pokud nespravny format - chyba, jinak definujeme level a param, pripadne stats
        return 1;
    }
    while (fgets(heslo,102,stdin)){
        if (heslo[0] == '\n' || heslo[0] == '\0'){
            continue;
        }
        if (heslo[100] >= 32 && heslo[100] <= 126){
            fprintf(stderr,"ERROR: Nespravna delka hesla!\n");
            return 1;
        }
        if (st == 1){           //pokud mame --stats (nebo [--stats]), pocitame pro kazde heslo
            stats(heslo,ruzn,&mindel,&pocet,&sumdel,&prumdel);      //tyhle promenne jsou obecne pro vsechna hesla,
        }                                                           //proto pouzivam ukazatele
        kroky(level, heslo, param);
    }
    if (st == 1){                  //spocitame pocet ruznych znaku
        for (int i = 0; i < 95; i++){
            sumruzn += ruzn[i];
        }
        printf("Statistika:\nRuznych znaku: %d\nMinimalni delka: %d\nPrumerna delka: %.1f\n", sumruzn, mindel, prumdel);
    }
    return 0;
}


int prav_1(char heslo[102]){
    int vel = 0, mal = 0;
    for (int i = 0; heslo[i] != '\n' && heslo[i] != '\0'; i++){
        if (vel == 0 && heslo[i] >= 65 && heslo[i] <= 90){
            vel++;
        }
        else if (mal == 0 && heslo[i] >= 97 && heslo[i] <= 122){
            mal++;
        }
        if (mal == 1 && vel == 1){
            return 1;
        }
    }
    return 0;
}

int prav_2(char heslo[102], int X){
    int skup[4] = {0};        //pole skupin
    if (X > 4){
        X = 4;
    }
    for (int i = 0; heslo[i] != '\n' && heslo[i] != '\0'; i++){
        if (heslo[i] >= 32 && heslo[i] <= 126){         //prvni 3 skupiny lezi v 4.
            if (heslo[i] >= 97 && heslo[i] <= 122){
                skup[0] = 1;
            }
            else if (heslo[i] >= 65 && heslo[i] <= 90){
                skup[1] = 1;
            }
            else if (heslo[i] >= 48 && heslo[i] <= 57){
                skup[2] = 1;
            }
            else {
                skup[3] = 1;
            }
            if (skup[0] + skup[1] + skup[2] + skup[3] == X){
                return 1;
            }
        }
    }
    return 0;
}

int prav_3 (char heslo[102], int X){
    int sekv = 1;
    for (int i = 1; heslo[i] != '\n' && heslo[i] != '\0'; i++){
        if (heslo[i] == heslo[i-1]){
            sekv++;
        }
        else {
            sekv = 1;
        }
        if (sekv == X){
            return 0;
        }
    }
    return 1;
}

int prav_4 (char heslo[102], int X){
    for (int i = 0;  heslo[i] != '\n' && heslo[i] != '\0'; i++){
        for (int j = i+1; heslo[j] != '\n' && heslo[j] != '\0'; j++){
            int flag = 0;
            for (int k = 0; k <= X; k++){           //potrebujeme ten for, aby napriklad
                if (heslo[i+k] != heslo[j+k]){    //pri X==2 a hesle "JiJJi" nebyl spravny
                    break;
                }
                else{
                    flag++;
                    if (flag == X){
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

int stats(char heslo[102], int *ruzn, int *mindel, int *pocet, int *sumdel,float *prumdel){    //pouzivam ukazatele
    int delka = 0;                                               //protoze kazde heslo meni ty promenne
    for (int i = 0; heslo[i] != '\n' && heslo[i] != '\0'; i++){
        if (*(ruzn+heslo[i]-32) == 0){
            (*(ruzn+heslo[i]-32))++;   //budeme mit pole 0 a 1 v ruzn
        }
        delka++;
    }
    if (delka < *mindel || *mindel == 0){
        *mindel = delka;
    }
    (*pocet)++;
    *sumdel += delka;
    *prumdel = (float)(*sumdel)/(float)(*pocet);
    return 0;
}

int chyby_obv(int argc, char *argv[]){      //pomocna funkce pro zakladni chyby
    if (argc < 3 || argc > 4){
        fprintf(stderr,"ERROR: Nespravny pocet argumentu!\n");
        return 1;
    }
    for (int i = 0; argv[1][i] != '\0'; i++){
        if (argv[1][i] < '0' || argv[1][i] > '9'){
            fprintf(stderr, "ERROR: Nespravny format levelu!\n");
            return 1;
        }
    }
    for (int i = 0; argv[2][i] != '\0'; i++){
        if (argv[2][i] < '0' || argv[2][i] > '9'){
            fprintf(stderr, "ERROR: Nespravny format parametru!\n");
            return 1;
        }
    }
    return 0;
}

int def_levelparamstats (int argc, char *argv[], int *level, int *param, int *st){
    char zkouska[10] = "--stats";   //pro overeni, ze uzivatel pise prave --stats
    if (argc == 1){         //specialni pripad premioveho reseni
        *level = 1;
        *param = 1;
    }
    else if ((argv[1][0] != '-' || (argv[1][1] >= '0' && argv[1][1] <= '9')) && (argv[2] == NULL || argv[2][0] != '-' || (argv[2][1] >= '0' && argv[2][1] <= '9'))){   //obvykly pripad (ne premiovy)
        if (chyby_obv(argc, argv)){
            return 1;
        }
        *level = atoi(argv[1]);
        *param = atoi(argv[2]);
        if (argv[3] != NULL){
            for (int i = 0; zkouska[i] != '\0' || argv[3][i] != '\0'; i++){
                if (argv[3][i] != zkouska[i]){
                    fprintf(stderr, "ERROR: Nespravny format --stats!\n");     //dalsi chyba
                    return 1;
                }
                *st = 1;
            }
        }
    } else {                 //premiovy pripad
        for (int i = 1; i < argc; i++){
            if (argv[i][0] == '-'){
                if (argv[i+1] == NULL && argv[i][1] != '-'){
                    fprintf(stderr, "ERROR: neni hodnota po prepinaci!");
                    return 1;
                }
                if (argv[i][1] == 'l'){
                    *level = atoi(argv[i+1]);
                }
                else if (argv[i][1] == 'p'){
                    *param = atoi(argv[i+1]);
                }
                else if (argv[i][1] == '-'){    //je to pripad [--stats]
                    for (int j = 0; zkouska[j] != '\0' || argv[i][j] != '\0';j++){
                        if (argv[i][j] != zkouska[j]){
                            fprintf(stderr, "ERROR: Nespravny format --stats!\n");
                            return 1;
                        }
                    }
                    *st = 1;
                }
                else {
                    fprintf(stderr, "ERROR: Nespravny format prepinacu!");
                    return 1;
                }
            }
        }
        for (int i = 1; i < argc; i++){
            if ((argv[i-1][0]!='-' || argv[i-1][1]=='-') && (argv[i][0]!='-' || (argv[i][1] >= '0' && argv[i][1] <= '9'))){
                if (*level != 0 && *param != 0){
                    fprintf(stderr, "ERROR: mnoho svobodnych argumentu pri pouzivani prepinacu!\n");
                    return 1;
                } else if (*level == 0 && *param == 0){
                    fprintf(stderr, "ERROR: jednoho argumentu nestaci!\n");
                    return 1;
                } else if (*level == 0){
                    *level = atoi(argv[i]);
                } else if (*param == 0){
                    *param = atoi(argv[i]);
                }
            }
        }

        if (*level == 0){              //podle zadani, pokud neni prepinac, tak argument=1
            (*level)++;
        }
        if (*param == 0){
            (*param)++;
        }
    }

    if (*level <= 0 || *level >= 5){      //dalsi chyby
        fprintf(stderr,"ERROR: level nemuze mit hodnotu %d!\n", *level);
        return 1;
    }
    if (*param <= 0){
        fprintf(stderr,"ERROR: param nemuze mit hodnotu %d!\n", *param);
        return 1;
    }
    return 0;
}

int kroky(int level, char heslo[102], int param){
    int podminky[4] = {1,1,1,1};
    if (level == 4){
        podminky[3] = prav_4(heslo, param);
        level--;
    }
    if (level == 3){
        podminky[2] = prav_3(heslo, param);
        level--;
    }
    if (level == 2){
        podminky[1] = prav_2(heslo, param);
        level--;
    }
    if (level == 1){
        podminky[0] = prav_1(heslo);
        level--;
    }
    if (podminky[0]*podminky[1]*podminky[2]*podminky[3] == 1){    //pokud splneno co potrebujeme,
        printf("%s", heslo);                                     //vypiseme to heslo
    }
    return 0;
}

