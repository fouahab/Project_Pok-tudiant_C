#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "poketudiant.h"

VarPoke parlfor = {Parlfor, NOISY, 1, 0, 60, 40, 60};
VarPoke ismar = {Ismar, NOISY, 1, 1, 50, 30, 40};
VarPoke rigolamor = {Rigolamor, NOISY, 0, 0, 85, 55, 70};

VarPoke tabVarPoke[10] = {
        {Parlfor, NOISY, 1, 0, 60, 40, 60}, 
        {Ismar, NOISY, 1, 1, 50, 30, 40}, 
        {Rigolamor, NOISY, 0, 0, 85, 55, 70},
        {Procrastino, LAZY, 1, 0, 40, 60, 60},
        {Couchtar, LAZY, 1, 1, 30, 50, 40},
        {Nuidebou, LAZY, 0, 0, 55, 85, 70},
        {Buchafon, MOTIVATED, 1, 0, 50, 50, 60},
        {Belmention, MOTIVATED, 1, 1, 30, 50, 40},
        {Promomajor, MOTIVATED, 0, 0, 70, 70, 70},
        {Enseignant_dresseur, TEACHER, 0, 0, 100, 100, 100}
    };

Attaque tabAttaques[12] = {
        {OBOULO, 10, MOTIVATED},
        {EXO_MAISON, 15, MOTIVATED},
        {MAX_REVIZ, 20, MOTIVATED},
        {BAILLEMENT, 10, LAZY},
        {PTISOM, 15, LAZY},
        {SUPERDODO, 20, LAZY},
        {BAVARDAGE, 10, NOISY},
        {GROBOUCAN, 15, NOISY},
        {MEGAPHONE_VOCAL, 20, NOISY},
        {TITQUESTION, 10, TEACHER},
        {POSER_COLLE, 15, TEACHER},
        {FATAL_INTERRO, 20, TEACHER}
    };

//======================================================================================================
//======================================================================================================

//================================================
//============Utilitaires=========================

int aleatoire (int Min, int Max){
    // int, int -> int
    //renvoie nb aleatoire entre min et max
    Max++; // pour inclure le Max
    //srand(time(NULL));
    int nombre_aleatoire ;
    nombre_aleatoire  = (Min+(rand()%(Max-Min)));
    return nombre_aleatoire ;
}

float coefAleatoire () {
    //void -> flaot
    //renvoie un float aleatoire compris entre 0,9 et 1,1

    //srand(time(NULL));
    float r = (float)aleatoire(900, 1100) / 1000;

    return r;
}

//=====================================================
//============= Gestion poketudiant====================
VarPoke genereVariete() {
    // void -> VarPoke
    // Genere une variete aleatoire qui n'est pas un pokétudiant évolué ou un enseignant_dresseur
    int alea;
    do {
        alea = aleatoire(0,7);
    } while (alea == 2 || alea ==5);
    return tabVarPoke[alea];
}

Attaque genereAttaqueMemeType(PKETD *pk) {
    //PKETD -> Attaque
    // genere une attaque du meme type que le poketudiant passe en parametre
    if (pk->vart.type == MOTIVATED) return tabAttaques[aleatoire(0,2)];
    if (pk->vart.type == LAZY) return tabAttaques[aleatoire(3,5)];
    if (pk->vart.type == NOISY) return tabAttaques[aleatoire(6,8)];
    if (pk->vart.type == TEACHER) return tabAttaques[aleatoire(9,11)];
    return tabAttaques[aleatoire(0,11)];
}

Attaque genereAttaqueAutreType (PKETD *pk) {
    //PKETD -> Attaque
    // genere une attaque d'un autre type que le poketudiant passe en parametre 
    // l'attaque ne doit pas etre du type TEACHER
    int alea;
    do {
        alea = aleatoire(0,8); // on genere un nb aleatoire de 0 à 9, i.e. toutes les atqs sauf celles de type teacher
    } while (tabAttaques[alea].type == pk->vart.type); // on test si l'atq n'est pas du meme type que le pokemon

    return tabAttaques[alea];
}

PKETD* generePoketudiant(int a) {
    //int -> PKETD
    //genere aleatoirement un poketudiant au lvl 1 qui n'est pas un poketudiant deja evolue ou un teacher
    //a permet de choisir quel type de poketudiant generer
    // 0 -> aleatoire
    // 1 -> NOISY
    // 2 -> LAZY
    // 3 -> MOTIVATED
    // 4 -> TEACHER
    static int identifiant = 0;
    PKETD *pk = malloc(sizeof(PKETD));

    switch(a) {
        case 0 : pk->vart = genereVariete();break;
        case 1 : pk->vart = tabVarPoke[aleatoire(0,1)];break;
        case 2 : pk->vart = tabVarPoke[aleatoire(3,4)];break;
        case 3 : pk->vart = tabVarPoke[aleatoire(6,7)];break;
        case 4 : pk->vart = tabVarPoke[9];break;
    }
    pk->id = identifiant;
    identifiant ++;
    pk->attaque1 = genereAttaqueMemeType(pk);//attaque aléatoire parmi les attaques du meme type que le poke
    pk->attaque2 = genereAttaqueAutreType(pk);//attaque aléatoire parmi les attaques d'un type different du poke
    pk->defense = round((double)coefAleatoire() * pk->vart.defense); // on divisera vart.defense par pk.defense pour retrouver le coef'
    pk->attaque = round((double)coefAleatoire() * pk->vart.attaque);
    pk->pvMax = round((double)coefAleatoire() * pk->vart.pvMax);
    pk->pv = pk->pvMax; // le poke a toute sa vie quand il est genere
    pk->niveau = 1; // tous les pokes sont generes au lvl 1
    pk->experience = 0; // on considere que le poke n'a jamais combattu avant generation

    return pk;
}

//=======================================
//====Gestion equipe et cafet============

Equipe initEquipe() {
    //void -> Equipe
    //initilaise une equipe de 6 poketudiants à NULL
    Equipe team = {NULL, NULL, NULL, NULL, NULL, NULL};
    team.nbElt = 0;
    return team;
}

void afficheEquipe(Equipe team) {
    // Equipe -> void
    //affiche de facon concise chaque pokemon de l'equipe
    // on considere qu'il n'y a pas de trou dans le tableau
    if (team.nbElt == 0) printf("Aucun poketudiant dans l'equipe\n");
    else {
        int i;
        for (i = 0; i < team.nbElt; i++) affichePoketudiantConcis(team.tab[i]);
    }
}

void afficheIemePokeEquipe(Equipe team, int i) {
    //Equipe, int -> void
    // affiche le ième poketudiant de l'equipe

    if (i < 0 || i > 6) {
        printf("erreur parametre i\n");
    }
    else {
        if (team.tab[i] != NULL)
            affichePoketudiantConcis(team.tab[i]);
    }
}

int equipePleine(Equipe team) {
    return (team.nbElt == 6);
}

int ajouteEquipe(Equipe *team, PKETD *pk) {
    //Equipe, PKETD -> int
    //ajoute un pketd à la premiere case du tableau disponible, si il y'en a une
    //retourne 1 si tout s'est bien passé, 0 sinon

    if (equipePleine(*team)) return 0;
    team->tab[team->nbElt] = pk;
    team->nbElt++;
    return 1;
}

void switchEquipe(Equipe *team, int pk1, int pk2) {
    //Equipe, int, int -> void
    //permet d'echanger le poketudiant à la place pk1 avec celui à la place pk2

    if (team->tab[pk1] == NULL || team->tab[pk2] == NULL) printf("Echange impossible\n");
    else {
        PKETD *tmp;
        tmp = team->tab[pk1];
        team->tab[pk1] = team->tab[pk2];
        team->tab[pk2] = tmp;
    }
}

PokeCafeteria initPokeCafeteria() {
    //void -> PokeCafetaria
    // initialise tous les poke de la cafet à NULL
    PokeCafeteria cafet;
    int i,j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            cafet.tab[i][j] = NULL;
        }
    }
    cafet.nbElt = 0;
    return cafet;
}

int cafetPleine(PokeCafeteria cafet) {
    // Pokecafeteria -> int
    // test si la cafet est pleine
    return (cafet.nbElt == 12);
}

int tablePleine(PokeCafeteria cafet, int table) {
    //Pokecafeteria, int -> int
    // test si la table de la cafet est pleine
    int i;
    for (i = 0; i < 4; i++) {
        if (cafet.tab[table][i] == NULL) return 0;
    }
    return 1;
}

void affichePokeCafeteria(PokeCafeteria cafet) {
    //PokeCafeteria -> void
    // affiche tous les poke de la cafet
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            if (cafet.tab[i][j] != NULL) {
                printf("poketudiant a la table %d\n", i);
                affichePoketudiantConcis(cafet.tab[i][j]);
            }
        }
    }
}

void afficheRevisionTable(PokeCafeteria cafet, int i) {
    //PokeCafeteria, int -> void
    //affiche la table i de la cafet
    if (i > 3) { printf("Mauvais argument\n"); return;}
    int j;
    for (j = 0; j < 4; j++) {
            if (cafet.tab[i][j] != NULL) {
                affichePoketudiantConcis(cafet.tab[i][j]);
            }
        }
}

/*
void moveTable(PokeCafeteria *cafet, int i, int j, int table) {
    //PokeCafeteria*, int, int, int -> void
    //deplace le poketudiant en (i,j) à la table (table)
    if (!tablePleine(*cafet, table)) {printf("la table est deja pleine\n"); return;}
    if (cafet->tab[i][j] == NULL) {printf("poketudiant inexistant\n"); return;}

    int k;
    int var = 0;
    for(k = 0; k < 4 ; k++) {
        if(var == 1) break;
        if(cafet->tab[table][k] == NULL) {
            cafet->tab[table][k] = cafet->tab[i][j];
            var = 1;
        }
    }
    cafet->tab[i][j] = NULL;
}
*/

void moveTable(PokeCafeteria *cafet, int table, PKETD* pk) {
    //
    //
    // buguée
    //Attention, si le pk est dans une equipe, il faut le supprimer de l'equipe et decaler les autres pokes de l'equipe
    //si il n'etait pas encore dans la cafet, on fait cafet.nbElt++
    //faire les free apres execution en fonction du cas
    if (!tablePleine(*cafet, table)) {printf("la table est deja pleine\n"); return;}
    if (pk == NULL) {printf("poketudiant inexistant\n"); return;}

    int k;
    int var = 0;
    PKETD *tmp = pk;
    for(k = 0; k < 4 ; k++) {
        printf("dans for\n");
        if(var == 1) break;
        if(cafet->tab[table][k] == NULL) {
            printf("dans if\n");
            cafet->tab[table][k] = tmp;
            pk = NULL;
            var = 1;
        }
    }
    printf("fin fonc\n");
}

void ajouteCafet(PokeCafeteria *cafet, PKETD* pk) {

    if (cafetPleine(*cafet)) {printf("Erreur, cafet pleine\n");return;}
    

    int i;
    int j;
    int var = 0;
    for (i = 0; i < 3; i ++) {
        if (var == 1) break;
        for (j = 0; j  < 4; j++) {
            if (var == 1) break;
            if (cafet->tab[i][j] == NULL) {
                cafet->tab[i][j] = pk;
                cafet->nbElt++;
                var = 1;
            }
        }
    }
}

void drop(Equipe *team, PokeCafeteria *cafet, int i) {
    //Equipe, Pokecafeteria, int -> void
    // deplace le pokemon en team[i] vers la cafet

    if (cafetPleine(*cafet)) {printf("Erreur, cafet pleine\n");return;}
    if (team->tab[i] == NULL) {printf("Erreur, argument faux\n");return;}
    ajouteCafet(cafet, team->tab[i]);
    //on decale ensuite tous les pk pour eviter les trous dans l'equipe;

    while(i < 6) {
        team->tab[i] = team->tab[i+1];
        i++;
    }
    team->nbElt --;

}

//=======================================
//============= affichage================

void affichePoketudiant(PKETD *pk) {
    printf("=========================\n");
    printf("Poketudiant numero : %d, ", pk->id);
    afficheVarietePoketudiant(pk);
    printf("de Type : ");
    afficheTypePoketudiant(pk);

    printf("Statistiques du poketudiant : \n");
    printf("niveau %d, %d xp sur %d \n", pk->niveau, pk->experience, xpNext(pk));
    printf("Attaque : %d\n", pk->attaque);
    printf("Defense : %d\n", pk->defense);
    printf("Points de vie : %d / %d \n", pk->pv, pk->pvMax);

    printf("liste des attaques : \n");
    afficheAttaquesPoketudiant(pk);
}

void affichePoketudiantConcis(PKETD *pk) {
    if (pk != NULL) {
        printf("ID : %d\n", pk->id);
        printf("Variete : "); afficheVarietePoketudiant(pk);
        printf("Niveau : %d \n", pk->niveau);
        printf("PV : %d / %d\n", pk->pv, pk->pvMax);
    }
}

void afficheVarietePoketudiant(PKETD *pk){
    int a;
    a = pk->vart.var;

    switch(a) {
        case 0 : printf("Parlfor\n");break;
        case 1 : printf("Ismar\n");break;
        case 2 : printf("Rigolamor\n");break;
        case 3 : printf("Procrastino\n");break;
        case 4 : printf("Couchtar\n");break;
        case 5 : printf("Nuidebou\n");break;
        case 6 : printf("Buchafon\n");break;
        case 7 : printf("Belmention\n");break;
        case 8 : printf("Promomajor\n");break;
        case 9 : printf("Enseignant_dresseur\n");break;
    }
}

void afficheTypePoketudiant(PKETD *pk) {
    int a;
    a = pk->vart.type;

    switch(a) {
        case 0 : printf("NOISY\n");break;
        case 1 : printf("LAZY\n");break;
        case 2 : printf("MOTIVATED\n");break;
        case 3 : printf("TEACHER\n");break;
        default : printf("Erreur\n");
    }
}

void afficheTypeAttaque1(PKETD *pk) {
    int a;
    a = pk->attaque1.type;

    switch(a) {
        case 0 : printf("NOISY\n");break;
        case 1 : printf("LAZY\n");break;
        case 2 : printf("MOTIVATED\n");break;
        case 3 : printf("TEACHER\n");break;
        default : printf("Erreur\n");
    }
}
void afficheTypeAttaque2(PKETD *pk) {
    int a;
    a = pk->attaque2.type;

    switch(a) {
        case 0 : printf("NOISY\n");break;
        case 1 : printf("LAZY\n");break;
        case 2 : printf("MOTIVATED\n");break;
        case 3 : printf("TEACHER\n");break;
        default : printf("Erreur\n");
    }
}


void afficheAttaquesPoketudiant(PKETD *pk) {
    int a;
    a = pk->attaque1.descr;
    printf("Attaque 1 :  ");
    switch(a) {
        case 0 : printf("BAVARDAGE");break;
        case 1 :printf("GROBOUCAN");break;
        case 2 :printf("MEGAPHONE_VOCAL");break;
        case 3 :printf("BAILLEMENT");break;
        case 4 :printf("PTISOM");break;
        case 5 :printf("SUPERDODO");break;
        case 6 :printf("OBOULO");break;
        case 7 :printf("EXO_MAISON");break;
        case 8 :printf("MAX_REVIZ");break;
        case 9 :printf("TITQUESTION");break;
        case 10 :printf("POSER_COLLE");break;
        case 11 :printf("FATAL_INTERRO");break;
        default : printf("Erreur");
    }

    printf(" de type :");
    afficheTypeAttaque1(pk);

    a = pk->attaque2.descr;

    printf("Attaque 2 :  ");
    switch(a) {
        case 0 : printf("BAVARDAGE");break;
        case 1 :printf("GROBOUCAN");break;
        case 2 :printf("MEGAPHONE_VOCAL");break;
        case 3 :printf("BAILLEMENT");break;
        case 4 :printf("PTISOM");break;
        case 5 :printf("SUPERDODO");break;
        case 6 :printf("OBOULO");break;
        case 7 :printf("EXO_MAISON");break;
        case 8 :printf("MAX_REVIZ");break;
        case 9 :printf("TITQUESTION");break;
        case 10 :printf("POSER_COLLE");break;
        case 11 :printf("FATAL_INTERRO");break;
        default : printf("Erreur");
    }

    printf(" de type : ");
    afficheTypeAttaque2(pk);
}

//=============================================
//===============Combat========================

int capturer(PKETD* pk, Equipe* equipe, PokeCafeteria *cafet) {
    //PKETD -> int
    //fonction pour capturer un poketudiant
    //renvoie 1 si le pk est capture, 0 sinon
    if (pk->vart.capturable == 0){
        printf("le pokemon n'est pas capturable\n");
        return 0;
    } 
    int i, j;
    int probacapture = (int) (calculeProbaCapturer(pk) * 1000);
    int alea = aleatoire(0,1000);
    if (alea < probacapture) { // reussit
        if (equipe->nbElt < 6) {
            ajouteEquipe(equipe, pk);
            return 1;
        }
        else if (cafet->nbElt < 12) {
            for(i = 0;i < 3; i++) {
                for(j = 0; j < 4; j++) {
                    if (cafet->tab[i][j] == NULL) {
                        cafet->tab[i][j] = pk;
                        cafet->nbElt ++ ;
                        return 1;
                    }
                }

            }
        }

        else { //cafet pleine
            free(pk);
            printf("cafet et equipe pleines\n");
            return 1;
        }
    }
    return 0;
}

float calculeProbaCapturer(PKETD *pk) {
    float a = 2*(0.5 - pk->pv / pk->pvMax);
    if (a < 0) return 0;
    return a;
}

/*
int probabilite(float p){
    int i = p*10;
    int k = aleatoire (1,10);
    if(k<i){
        return 1;
    }
    return 0;
}
*/



//FONCTION FUIRE LE COMBAT

/*
int fuire(int nb){
    int k = aleatoire (1,100);
    if(k<nb){
        return 1;
    }
    return 0;
}
int pokeValable(PKETD* pk){
    int j;
    for(j=0;j<sizeof(pk->id)/sizeof(int);j++){
        Maillon_poketudiant* courant = ieme_maillon(tab_poketudiant,pk1.id[j]);
        PKETD* pokeCourant = courant->poke;
        if(pokeCourant->pv) return 1;
    }
    return 0;

}

*/

int calculEfficacite(PKETD pkA, PKETD pkD) {
    // PKETD, PKETD -> int
    // calcule l'efficacité d'un type sur un autre et renvoie le facteur multiplicateur en fonction
    if(pkA.vart.type == NOISY && pkD.vart.type == LAZY) return 2;
    if(pkA.vart.type == LAZY && pkD.vart.type == MOTIVATED) return 2;
    if(pkA.vart.type == MOTIVATED && pkD.vart.type == NOISY) return 2;
    if(pkA.vart.type == TEACHER && pkD.vart.type != TEACHER) return 2;
    else return 1;
}

int CalculDommage(PKETD pkA, PKETD pkD, int puis){
    //PKETD, PKETD, int -> int
    //effectue le calcul des dommages en fonction des stats des deux poke et de leur type
    int o = aleatoire(9,11);
    float k = (float)o/10;

    int eff = calculEfficacite(pkA, pkD);

    int res = round((k*(pkA.attaque/pkD.defense)*puis)*eff);
    return res;
}

void recalculStat(PKETD *pk, float coefAtq, float coefDef, float coefPv) {
    // PKETD, float, float, float -> void
    // recalcule les stats d'un pokemon qui vient d'evoluer
    pk->attaque = round(pk->vart.attaque * coefAtq * 1.2);
    pk->defense = round(pk->vart.defense * coefDef * 1.2);
    pk->pvMax = round(pk->vart.pvMax * coefPv * 1.2);
}

void evoluer(PKETD* pk) {
    // PKETD -> void
    // fait evoluer un poketudiant
    if (pk->vart.evolue == 0) { printf("ERREUR : le pokétudiant ne peut pas évoluer\n"); return; }

    if (pk->vart.var == Ismar) {
        float coefAtq = (float)(pk->vart.attaque / pk->attaque);
        float coefDef = (float)(pk->vart.defense / pk->defense);
        float coefPv  = (float)(pk->vart.pvMax / pk->pvMax);
        pk->vart.var = Rigolamor;
        recalculStat(pk, coefAtq, coefDef, coefPv);
    }

    if (pk->vart.var == Couchtar) {
        float coefAtq = (float)(pk->vart.attaque / pk->attaque);
        float coefDef = (float)(pk->vart.defense / pk->defense);
        float coefPv  = (float)(pk->vart.pvMax / pk->pvMax);
        pk->vart.var = Nuidebou;
        recalculStat(pk, coefAtq, coefDef, coefPv);
    }

    if (pk->vart.var == Belmention) {
        float coefAtq = (float)(pk->vart.attaque / pk->attaque);
        float coefDef = (float)(pk->vart.defense / pk->defense);
        float coefPv  = (float)(pk->vart.pvMax / pk->pvMax);
        pk->vart.var = Promomajor;
        recalculStat(pk, coefAtq, coefDef, coefPv);
    }
    soin(pk);
}
void soin(PKETD *pk) {
    // PKETD -> void
    // soigne un poketudiant
    pk->pv = pk->pvMax;
}


int xpNext(PKETD *pk) {
    // PKETD -> int
    // calcule l'xp que necessaire pour passer au niveau suivant
    int exp = round(500*(1+pk->niveau)/2);
    return exp;
}

/*
void combatSauvage (Equipe *team, PKETD *pk2) {


}
*/

/*
void experience(List_poketudiant* pk1,PKETD* pk2, int exp){
    int nombre = nb_poketudiants(pk1);
    int part = exp/nombre;

    Maillon_poketudiant* mail ;
    PKETD* courant;
    int i;
    for(i=0;i<nombre;i++){
        mail = iem_maillon(pk1,i);
        courant = mail->poke;
        courant->experience += part;
        niveau(courant);
    }
    pk2->experience -= (10*pk2->experience/100);
    if(pk2->experience<0) pk2->experience=0;
}
*/

/*
void combat(PKETD* pk1, PKETD* pk2){
    int fuite = 0, experience1, experience2;

    List_poketudiant* participe = init();
    List_poketudiant* participe2 = init();
    if(!pk1->id){
        printf("vous n'avez pas de pokétudiants \n");
    }else{
        PKETD* poke1,*poke2,*pokeCourant;
        int choix = afficherAttaquer();
        int at2,at1, choixChanger;
        Maillon_poketudiant* courant;
        Attaque attaque1,attaque2;
        courant = ieme_maillon(tab_poketudiant,pk1.id[1]);
        poke1 = courant->poke;
        insererStructure(participe,poke1);
        experience1 = poke1->experience;

        if(pk2->type != TEACHER){
            while(!fuite && pk1->pv > 0 && pokeValable(pk1) && pk2->pv > 0){
                srand(time(NULL));
                switch(choix){
                    case 1 : at1 = aleatoire (0,NB_ATTAQUES1);
                            attaque1 = pk1->attaque1[at1];
                            attaquer(pk1,pk2,attque1);
                            if(est_ko(pk2))
                                experience(participe,experience1);
                    break;
                    case 2 :
                            at2 = aleatoire (0,NB_ATTAQUES2_TEACHER);
                            attaque2 = pk1->attaque2[at2];
                            attaquer(pk1,pk2,attaque2);
                            if(est_ko(pk2))
                                experience(participe,experience1);

                    break;
                    case 3 :
                        if(sizeof(pk1->id)/sizeof(int) == 1){
                            printf("vous n'avez pas d'autres pokétudiants \n");
                        }else{
                            printf("Par quel pokétudiant vous voulez changer :\n")
                            for(i=1;i<sizeof(pk1->id)/sizeof(int); i++){
                                if(poke1->id != pk1.id[i]){
                                    courant = ieme_maillon(tab_poketudiant,pk1.id[i]);
                                    pokeCourant = courant->poke;
                                    printf("%d- %s",i,pokeCourant->vart);
                                }
                             }
                             scanf("%d",&choixChanger);
                             courant = ieme_maillon(tab_poketudiant,pk1.id[choixChanger]);
                             poke1 = courant->poke;
                             insererStructure(participe,poke1);
                             experience1 = poke1->experience;
                        }
                    case 4 :
                             capturer(pk1,pk2);
                    break;
                    case 5 :
                        int level = difference(poke1,pk2);
                        if(level>=3){
                            fuite = 1;
                        }else{
                            switch(level){
                                case 2 :
                                    fuite = fuire(90);
                                break;
                                case 1 :
                                    fuite = fuire(75);
                                break;
                                case 0 :
                                    fuite = fuire(50);
                                break;
                                case -1 :
                                    fuite = fuire(40);
                                break;
                                case -2 :
                                    fuite = fuire(25);
                                break;
                                default :
                                    fuite = 0;

                            }
                        }
                    break;
                }
            }
            }else{
                experience2 = pk2->experience;
                while(!fuite && pk1->pv > 0 && pokeValable(pk1) && pokeValable(pk2) && pk2->pv > 0){

                    List_poketudiant* participe2 = init();
                    PKETD* poke1,*poke2;
                    int at2,at1, choixChanger;
                    Maillon_poketudiant* courant,*courant2;

                    Attaque attaque1,attaque2;
                    courant = ieme_maillon(tab_poketudiant,pk1.id[1]);
                    poke1  = courant->poke;
                    insererStructure(participe,poke1);

                    courant2 = ieme_maillon(tab_poketudiant,pk2.id[1]);
                    poke2 = courant2->poke;
                    insererStructure(participe2,poke2);

                    srand(time(NULL));
                    int choix = afficherAttaquer();
                    switch(choix){
                        case 1 : at1 = aleatoire (0,NB_ATTAQUES1);
                                 attaque1 = pk1->attaque1[at1];
                                 attaquer(pk1,pk2,attaque1);
                                 if(est_ko(pk2))
                                    experience(participe,experience2);
                        break;
                        case 2 : at2 = aleatoire (0,NB_ATTAQUES2_TEACHER);
                                pk1->attaque2[at2];
                                attaque2 = attaquer(pk1,pk2,attaque2);
                                if(est_ko(pk2))
                                    experience(participe,experience2);
                        break;
                        case 3 :
                            if(sizeof(pk1->id)/sizeof(int) == 1){
                                printf("vous n'avez pas d'autres pokétudiants \n");
                            }else{
                                printf("Par quel pokétudiant vous voulez changer :\n")
                                for(i=1;i<sizeof(pk1->id)/sizeof(int); i++){
                                    if(poke1->id != pk1.id[i]){
                                        courant = ieme_maillon(tab_poketudiant,pk1.id[i]);
                                        pokeCourant = courant->poke;
                                        printf("%d- %s",i,pokeCourant->vart);
                                    }
                                 }
                                 scanf("%d",&choixChanger);
                                 courant = ieme_maillon(tab_poketudiant,pk1.id[choixChanger]);
                                 poke1 = courant->poke;
                                 insererStructure(participe,poke1);
                            }
                        case 4 :
                                capturer(pk1,pk2);
                        break;
                        case 5 :
                            int level = difference(poke1,pk2);
                            if(level>=3){
                                fuite = 1;
                            }else{
                                switch(level){
                                    case 2 :
                                        fuite = fuire(90);
                                    break;
                                    case 1 :
                                        fuite = fuire(75);
                                    break;
                                    case 0 :
                                        fuite = fuire(50);
                                    break;
                                    case -1 :
                                        fuite = fuire(40);
                                    break;
                                    case -2 :
                                        fuite = fuire(25);
                                    break;
                                    default :
                                        fuite = 0;

                                }
                            }
                        break;

                    }

                    int choix = afficherAttaquer();
                    switch(choix){
                        case 1 : at1 = aleatoire (0,NB_ATTAQUES1);
                                 attaque1 = pk2->attaque1[at1];
                                 attaquer(pk2,pk1,attaque1);
                                 if(est_ko(pk1))
                                    experience(participe2,experience2);
                        break;
                        case 2 :
                                at2 = aleatoire (0,NB_ATTAQUES2_TEACHER);
                                attaque2 = pk2->attaque2[at2];
                                attaquer(pk2,pk1,attaque2);
                                if(est_ko(pk1))
                                    experience(participe2,experience2);
                        break;
                        case 3 :
                                if(sizeof(pk1->id)/sizeof(int) == 1){
                                    printf("vous n'avez pas d'autres pokétudiants \n");
                                }else{
                                    printf("Par quel pokétudiant vous voulez changer :\n")
                                    for(i=1;i<sizeof(pk1->id)/sizeof(int); i++){
                                        if(poke1->id != pk1.id[i]){
                                            courant = ieme_maillon(tab_poketudiant,pk1.id[i]);
                                            pokeCourant = courant->poke;
                                            printf("%d- %s",i,pokeCourant->vart);
                                        }
                                     }
                                    scanf("%d",&choixChanger);
                                    courant = ieme_maillon(tab_poketudiant,pk1.id[choixChanger]);
                                    poke1 = courant->poke;
                                    insererStructure(participe2,poke1);
                                }
                        case 4 :
                                capturer(pk1,pk2);
                        break;
                        case 5 :
                            int level = difference(poke1,pk2);
                            if(level>=3){
                                fuite = 1;
                            }else{
                                switch(level){
                                    case 2 :
                                        fuite = fuire(90);
                                    break;
                                    case 1 :
                                        fuite = fuire(75);
                                    break;
                                    case 0 :
                                        fuite = fuire(50);
                                    break;
                                    case -1 :
                                        fuite = fuire(40);
                                    break;
                                    case -2 :
                                        fuite = fuire(25);
                                    break;
                                    default :
                                        fuite = 0;

                                }
                            }
                    break;
                }
            }
        }
    }
}
*/

/*
void wild(PKETD* ens, int i, int j){
    srand(time(NULL));
    int niv = aleatoire (i,j);
	int type = aleatoire (1,NB_TYPE);
    PKETD* sauvage;
    switch(type){
    case 1: sauvage = initPokeMotivated(niv);
        break;
    case 2: sauvage = initPokeNoisy(niv);
        break;
    case 3: sauvage = initPokeLasy(niv);
        break;
    }
}
*/

//=============================================
//=====Version liste chainee===================

List_poketudiant* init(){
    // void -> List_poketudiant*
    //initialise une liste chainee de poketudiants vide et la renvoie;
    List_poketudiant *tete = (List_poketudiant*) malloc(sizeof(List_poketudiant));
    Maillon_poketudiant *maillon = malloc(sizeof(Maillon_poketudiant));
    maillon->poke = NULL;
    maillon->suivant = NULL;
    tete->premier = maillon;
    return tete;
}

int structreVide(List_poketudiant* liste){
    return (liste->premier == NULL) ? 1 : 0;
}

int nbEltSupX(List_poketudiant* liste, int x) {
    int cpt = 0;
    Maillon_poketudiant *tmp = liste->premier;
    while (tmp->suivant != NULL) {
        tmp = tmp->suivant;
        cpt++;
    }
    return (cpt >= x);
}

void insererStructure(List_poketudiant* liste, PKETD *pk) {
    //List_poketudiant*, PKTED -> void
    //insere un element en queue de la liste
    if (structreVide(liste)) {
        insertion_en_tete(liste, pk);
        return;
    }

    Maillon_poketudiant *nd;
    Maillon_poketudiant *tmp = liste->premier;
    nd = malloc(sizeof(*nd));
    nd->poke = pk;
    nd->suivant = NULL;
    while (tmp->suivant != NULL)
        tmp = tmp->suivant;
    tmp->suivant = nd;
}

void insertion_en_tete (List_poketudiant* liste, PKETD *pk) {
    //List_poketudiant*, PKTED -> void
    //insere un element en tete de la liste

    Maillon_poketudiant *nd;
    nd = malloc(sizeof(*nd));
    nd->poke = pk;
    nd->suivant = NULL;
    nd->suivant = liste->premier;
    liste->premier = nd;
}

//nombre pokétudiant
int nb_poketudiants(List_poketudiant* tete){
    int i=0;
    if(!structreVide(tete)){
        Maillon_poketudiant* courant;
        i+=1;
        for(courant = tete->premier; courant->suivant != NULL; courant = courant->suivant){
            i++;
        }
    }
    return i;
}

//l'élement de la liste chainée qui stock tout les pokétudiant 
Maillon_poketudiant* ieme_maillon(List_poketudiant* liste, int i){
    if(!structreVide(liste)){
        Maillon_poketudiant* courant;
        courant = liste->premier;
        int j = i;
        while(courant && j>0){
            courant = courant->suivant;
            j--;
        }
        return courant;
    }
    return NULL;
}