#define NB_ATTAQUES1 3
#define NB_ATTAQUES2_TEACHER 9
#define NB_ATTAQUES2_OTHER 6
#define NB_Varietes 3
#define NB_TYPE 4

typedef enum {NOISY,LAZY,MOTIVATED,TEACHER}Type;
typedef enum {Parlfor,Ismar,Rigolamor,Procrastino,Couchtar,Nuidebou,Buchafon,Belmention,Promomajor,Enseignant_dresseur}Variete;
typedef enum {BAVARDAGE,GROBOUCAN,MEGAPHONE_VOCAL,BAILLEMENT,PTISOM,SUPERDODO,OBOULO,EXO_MAISON,MAX_REVIZ,TITQUESTION,POSER_COLLE,FATAL_INTERRO}Attaques;


typedef struct atq{
	Attaques descr; // nom de l'attaque
	int puissance;
	Type type;
} Attaque;

typedef struct pokevar { //informations de base du poké
	Variete var; // inutile ?
	Type type;
	unsigned short int capturable;
	unsigned short int evolue; //booleen
	int attaque;
	int defense;
	int pvMax;

} VarPoke;


typedef struct POKETUDIANT{
	unsigned int id;
	Attaque attaque1; // attaque du meme type du poké
	Attaque attaque2; // attaque de l'autre type != pokéteacher
	VarPoke vart;
	//stats : 
   	int defense;
	int pv; // pv actuels
	int pvMax;
	int attaque;
	int experience;
	int niveau;
} PKETD;

typedef struct MAILLON_POKETUDIANT{
    PKETD *poke;
    struct MAILLON_POKETUDIANT* suivant;
} Maillon_poketudiant;

typedef struct LIST_POKETUDIANT{
    Maillon_poketudiant* premier;
} List_poketudiant;


List_poketudiant* tab_poketudiant;

typedef struct cafet { // /!\ initialiser avec tous les poke a NULL
	PKETD* tab[3][4];
	int nbElt;
}PokeCafeteria;

typedef struct team {
	PKETD* tab[6];
	int nbElt;
}Equipe;


PKETD* generePoketudiant(int);
VarPoke genereVariete();
Attaque genereAttaqueMemeType(PKETD *pk);
Attaque genereAttaqueAutreType (PKETD *pk);
int capturer(PKETD* pk, Equipe* equipe, PokeCafeteria* cafet);
int calculEfficacite(PKETD pkA, PKETD pkD);
int CalculDommage(PKETD pkA, PKETD pkD, int puis);
void recalculStat(PKETD *pk, float coefAtq, float coefDef, float coefPv);
void evoluer(PKETD* pk);
void soin(PKETD *pk);
int xpNext(PKETD *pk);
float calculeProbaCapturer(PKETD *pk);


void affichePoketudiant(PKETD *pk);
void affichePoketudiantConcis(PKETD *pk);
void afficheAttaquesPoketudiant(PKETD *pk);
void afficheTypePoketudiant(PKETD *pk);
void afficheVarietePoketudiant(PKETD *pk);
void afficheTypeAttaque1(PKETD *pk);
void afficheTypeAttaque2(PKETD *pk);

Equipe initEquipe();
int ajouteEquipe(Equipe *team, PKETD *pk);
int equipePleine(Equipe team);
void afficheEquipe(Equipe team);
void afficheIemePokeEquipe(Equipe team, int i);
void switchEquipe(Equipe *team, int pk1, int pk2);


PokeCafeteria initPokeCafeteria();
int cafetPleine(PokeCafeteria cafet);
int tablePleine(PokeCafeteria cafet, int table);
void affichePokeCafeteria(PokeCafeteria cafet);
void afficheRevisionTable(PokeCafeteria cafet, int i);
void moveTable(PokeCafeteria *cafet, int table, PKETD* pk);
void ajouteCafet(PokeCafeteria *cafet, PKETD* pk);
void drop(Equipe *team, PokeCafeteria *cafet, int i);


int aleatoire (int Min, int Max);
float coefAleatoire ();


List_poketudiant* init();
int structreVide(List_poketudiant* liste);
int nbEltSupX(List_poketudiant* liste, int x);
void insererStructure(List_poketudiant* liste, PKETD *pk);
void insertion_en_tete (List_poketudiant* liste, PKETD *pk);
int nb_poketudiants(List_poketudiant* tete);
Maillon_poketudiant* ieme_maillon(List_poketudiant* liste, int i);


