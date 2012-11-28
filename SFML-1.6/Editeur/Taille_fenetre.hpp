#ifndef TAILLE_FENETTRE_HPP
#define TAILLE_FENETTRE_HPP

#define fenettre_taille_x   1600
#define fenettre_taille_y   800
#define fenettre_taille_pix 32

#define taille_objets_pix   32

#define taille_carte_case_x 200
#define taille_carte_case_y 100

#include <string.h>

enum _type_edition { MAP=0,PNJ,OBJ,HEROS,OPTIONS,SKILLS,QUETTE,SONS};        //mode d'édition courant
enum _type_direction {BAS=0,GAUCHE,DROITE,HAUT,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15};  //enumération des direction que peu prendre un objet pour plus de lisibilite. a partir de _4 c'est que pour les objet spéciaux
enum _type_element {NEUTRE=0,FEU,EAU,TERRE,AIRE,OMBRE,LUMIERE}; //different element associée au monstres
enum _type_sons {MORT=0,DEPLACEMENT,TAPE,MUSIC,IMAGE};      //different sont possible d'associer à un objet (en vu de la spacialisation )
enum _type_objets{POTION_VIE=0,POTION_MANA,COFFRE,PARCHEMIN,SAUVEGARDE,SAUVEGARDE_FIX,TELEPORTEUR,OR,TRAMPOLINE,ACCELERATEUR,PIEGE,PORTAIL,MARCHAND,OBJ_QUETTE,RIEN,TELEPORTEUR_QUETTE,NB_OBJET}; //defferent type d'objet possible
enum _type_objet_sons{DRAGON=0,OISEAU,ANGE,POISSON,LOUP,BETTE,DEVIL,GHOST,SERPENT,ELEMENTAL,GOBLIN,INSECT,IMFORME,UNDEAD,HUMAN,MERCO,RIEN_SON}; //differents type de sons possible en vu d'avoir des sons differents pour les monstres, objets etc... (pas géré)

#define TAILLE_DIALOGUE_OBJ 1000
struct _PNJ_
{
  char nom[100];    //nom de l'image utilisé dans l'editeur
  char dossier[100];    //chemin relatif du dossier utiliser dasn l'editeur
  unsigned int img; //index de l'image (eviter les doublons) utilisé dans le jeu
  int vue,min,max,tx,ty; //vision en longeur //dep max //dep min // taille en x de l'image en case // en y
  _type_direction direction; //direction de depart
  int x,y; // coordonées
  bool agressif; // pour savoir si le monstre est agressife ou pas de base
  bool boss,looter,voleur,eparpieur,skin_tape; //differente autre caractéristiques
  int degats;   //degats moyens infligés
  int vie;  //vie de depart
  int vitesse;  //vitesse de deplacement et frappe
  int lvl;  //niveau
  bool volant;  //si il vol ou pas (IA differente)
  _type_element element;    //ellement associé au monstre
  //ajouter sdt::vector<_type_objets> tab_objet dans le jeu pour les
  int quette;   //si c'est un bosse il à un nombre ici , cela permet de gere l'avencement
  char dialogue[TAILLE_DIALOGUE_OBJ]; //différent dialogue qui peuvent etre associé à un monstre
  int exp;  //expérience que donne le monstre si il est tué
  int resize;   //taux d'agrandisement comparé à l'image d'aorigine
};
#define _PNJ struct _PNJ_

#define TAILLE_DIALOGUE_PNJ 1300
struct _OBJ_
{
  char nom[50];     //nom du fichier image (editeur)
  char dossier[50]; //chemlein relatif du dossier de l'image (editeur)
  unsigned int img; //idex dans le tableau des images pour eviter les doubleon (jeu)
  unsigned int tx,ty; //nombre de subdivision de l'image d'origine
  _type_direction direction;//direction de depart de l'objets (ligne su l'image)
  _type_objets type;   //type de l'objet
  char dialogue[TAILLE_DIALOGUE_PNJ];  //texte à afficher, interaction avec le hero
  int idSkill;  //id du skill donné
  char son_deplacement[100];    //chemin relatif du son a asocier (son passif)
  int quette;   //indice de quette de l'objet (utilisable à partir de..
  int x,y;  //position de depart
  int destx,desty;  //pour les teleporteurs ou rebond ou quantitée d'or ou acceleration (pour destx) & exp dans desty
  int resize;   //coeficient d'agrendicement en fonction de la taille de l'image de base
};
#define _OBJ struct _OBJ_

struct spr{
    unsigned int img;   //index dans le tableau d'imge
    unsigned int x,y,d,g,h,b;       //position (x,y), IntRect(d,g,h,b)
    bool colision;//si il y a une collision ou non avec se sprite
};

struct img{
    char nom[100];  //chemin relatif se l'image
};


struct unsigned_int{
    unsigned int i; // permet d'etre stoqué dans un fichier en tant que stucture (fwrite)
};

struct options_map{
    int gravitee;   //gravitée du lvl   x10
    int frottement; //frottement du lvl
    char sons[100]; //musique de fond
    char fond[100]; //image de foncd
    int chrono;      //temps pour realiser le lvl
    char nomMap[100];   //nom en toute lettre du niveau
    char nomVerif[16];  //nom permetant l'antis cheat
    char Type[100]; //type du niveau
    char mythologie[100];   //mythologie du lvl
    char AutoDialogue[1000];   //texte affiché des le debut du niveau automatiquement
};


struct heros{
  int acceleration;     //acceleration max du hero
  _type_direction direction;    //direction de depard du hero
  int vitesse;  //vitesse associé au hero
  int x,y;  //position de base
};

struct SKILLS{
    unsigned int ID;    //identifiant (cela sert pour etre comparé avec l'enumération créé automatiquement pour plus de lisibilitéé)
    char nom[50];   //nom du skill
    char description[1000]; //description de se que permet le skill
    char img[100];  //chemin relatif de l'image associé au skill
    int duree,recharge,mana;    //durée du skill, temps qu'il met à se recharger, et le mana consomé lors de l'utilisation
};

struct Sons_Spacialise
{
    int x,y;    //position de base
    int attenuation;    //atenuation x10
    int MinDistance; //distance minimum a laquelle on entende le sons x10
    int Volume; //volume par defaut x10
    char nom[50];  //chemin relatif du son
    char dossier[50];
};


#endif
