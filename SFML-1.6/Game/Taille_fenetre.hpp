#ifndef TAILLE_FENETRE_HPP
#define TAILLE_FENETRE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "data/skills_enum.hpp"

#define TAILLEBLOC           32

#define MAPSIZECARTEX       200
#define MAPSIZECARTEY       100
#define MAPSIZEX            (TAILLEBLOC*MAPSIZECARTEX)
#define MAPSIZEY            (TAILLEBLOC*MAPSIZECARTEY)

#define NB_PAR_OBJETS       100

#define BONUSPOPO           50

#define SPRINT_SURCHAUFFE   2.5

/*****************************************************
*   EDITEUR + JEU                                    *
*****************************************************/
enum _type_direction {BAS=0,GAUCHE,DROITE,HAUT,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15};
//attention : changer la définition des skins pour les objets spéciaux dans le constructeur de objet au moindre changement de l'enum ci dessous
enum _type_objets{POTION_VIE=0,POTION_MANA,COFFRE,PARCHEMIN,SAUVEGARDE,SAUVEGARDE_FIX,TELEPORTEUR,OR,TREMPOLINE,ACCELERATEUR,PIEGE,PORTAIL,MARCHAND,OBJET_QUETTE,OBJET_RIEN,TELEPORTEUR_QUETTE,NB_OBJETS};
enum _type_element {NEUTRE=0,FEU,EAU,TERRE,AIRE,OMBRE,LUMIERE};

/*******************************
*   JEU                        *
*******************************/
enum type_objets_lootables{OR_LOOT=0,POTION_VIE_LOOT,POTION_MANA_LOOT,PARCHEMIN_LOOT,SAUVEGARDES_LOOT,TAILLE_OBJETS_LOOT};
enum _type_temps_recharges { TEMPS_REGEN_POPO=10, TEMPS_REGEN_SAUVEGARDE=300 };
enum _type_animtemp {ANIM_MOBS_DEGATS=0,ANIM_HEAL,ANIM_MANA,ANIM_LVLUP,ANIM_TELEPORT,ANIM_TELEPORT_ARRIVED, ANIM_PTS_COMP, ANIM_MILATHEA, ANIM_RESPAWN, ANIM_TAILLE};
enum _type_animgame {ANIM_GAME_RECHARGE=0, ANIM_GAME_TAILLE};
enum AVANCE { NUL, MARCHER, COURIR };
enum TYPE_GUI {FOND_GUI=0,VIE_GUI,MANA_GUI,SPRINT_GUI,EXP_GUI,F1_GUI,F2_GUI,F3_GUI,F4_GUI,F5_GUI,F6_GUI,F7_GUI,F8_GUI,F9_GUI,F10_GUI,CONTOUR_GUI,VIDE_GUI,TAILLE_GUI};
enum TYPE_GUI_RECHARGE{F1_GUI_RECHARGE=0,F2_GUI_RECHARGE,F3_GUI_RECHARGE,F4_GUI_RECHARGE,F5_GUI_RECHARGE,F6_GUI_RECHARGE,F7_GUI_RECHARGE,F8_GUI_RECHARGE,F9_GUI_RECHARGE,TAILLE_GUI_RECHARGE};
enum GUI_STATS { VIE_STR, MANA_STR, SPRINT_STR, TAILLE_GUI_STATS };
enum TYPE_SON { POTIONRAMASSE=0, POTIONUTIL, MENUBOUGER, MENUVALIDER , GAMEOVER, PARCHEMIN_UTIL, LVLUP, UPCARAC, DOWNCARAC, TAPE_AIR1, TAPE_AIR2, TAPE_MOB1, TAPE_MOB2, TAPE_MOB3,
                TAPE_MOB4, BOSS_TUE, MISS_MOB, EPIC_FAIL, SAVE, TELEPORT, TELEPORT_ARRIVED, SON_OR, SON_TREMPOLINE, SON_ACCELERATEUR, SON_PIEGE, SON_ERROR_MENU,
                SON_RESPAWN, SON_MILATHEA, TAILLE_SON};
enum STATS { NB_POINTS_SKILLS=0, ATTAQUE, DEFENSE, VITESSE, ENDURANCE, INTELLIGENCE, LEVEL, TAILLE_STATS };
enum EXPERIENCE { XP=2, XPLEVEL, TAILLE_EXP };
enum VIEMANA { VIE=0, MANA , TAILLE_VIEMANA };
enum _scores {NB_MOBS_KILL=0,NB_MOBS_BOSS_KILL,XP_TOTAL,DEGATS_TOTAL,DEGATS_BEST,VIE_LOSE,MANA_LOSE,TAILLE_SCORES};
enum statusJeu { LOADING_GAME=0, MENUED, GAME_OVER, CHEAT, LOADED, GAMED, PAUSED, PARCHEMINED, MARCHANDISE, SAVED };
enum RACCOURCIS_BARRE_SORTS { F1_BS=0, F2_BS, F3_BS, F4_BS, F5_BS, F6_BS, F7_BS, F8_BS, F9_BS, F10_BS, TAILLE_RACCOURCIS_BARRE_SORTS };
enum infosLevelSlot { NOM_LEVEL=0, NOM_VERIF_LEVEL, MYTHOLOGIE_LEVEL, TYPE_LEVEL, TAILLE_LEVEL };

struct _barreSorts
{
    int id;
    bool isObject;
};

struct _options
{
    int volumeMusiqueJeux;
    int volumeMusiqueMenus;
    int volumeEffetsSonores;
    int volumeSons;
    int fps;
    bool verticalSync;
    bool fullscreen;
    char resolutionX[10], resolutionY[10];

    _options()
    {
        volumeMusiqueJeux=50;
        volumeMusiqueMenus=50;
        volumeEffetsSonores=50;
        volumeSons=100;
        fps=30;
        verticalSync=false;
        fullscreen=false;

        std::string resolX, resolY;
        resolX = "800";
        resolY = "600";
        for (unsigned int i=0; i<10; ++i)
        {
            resolutionX[i] = (i<resolX.size()) ? resolX[i] : '\0';
            resolutionY[i] = (i<resolY.size()) ? resolY[i] : '\0';
        }
    }
};

struct _touches_heros
{
    sf::Key::Code gauche,droite,sauter,courrir,taper,pause,ramasse,screenshot,gui1,gui2,gui3,gui4,gui5,gui6,gui7,gui8,gui9,gui10;
    int tabBarreSortsId[TAILLE_RACCOURCIS_BARRE_SORTS];
    bool tabBarreSortsObj[TAILLE_RACCOURCIS_BARRE_SORTS];

    _touches_heros()
    {
        gauche = sf::Key::Q;
        droite = sf::Key::D;
        sauter = sf::Key::Z;
        courrir = sf::Key::Space;
        taper = sf::Key::RShift;
        ramasse = sf::Key::E;
        pause = sf::Key::Escape;
        screenshot = sf::Key::I;
        gui1 = sf::Key::F1;
        gui2 = sf::Key::F2;
        gui3 = sf::Key::F3;
        gui4 = sf::Key::F4;
        gui5 = sf::Key::F5;
        gui6 = sf::Key::F6;
        gui7 = sf::Key::F7;
        gui8 = sf::Key::F8;
        gui9 = sf::Key::F9;
        gui10 = sf::Key::F10;
    }
};

struct _touches_sys
{
    sf::Key::Code gauche,droite,haut,bas,entree,retour;

    _touches_sys()
    {
        gauche = sf::Key::Left;
        droite = sf::Key::Right;
        haut = sf::Key::Up;
        bas = sf::Key::Down;
        entree = sf::Key::Return;
        retour = sf::Key::Escape;
    }
};

struct _saveHeros
{
    int statsPerso[TAILLE_STATS];
    int scoresPerso[TAILLE_SCORES];
    int xpPerso[TAILLE_EXP];
    unsigned int tab_objets[TAILLE_OBJETS_LOOT];
    int vie;
    int mana;
    bool tabSkills[TAILLE_SKILLS]; //skills
    float totalGameTime; //temps de jeu
};


/*****************************************************
*   EDITEUR + JEU                                    *
*****************************************************/
/*****************************************************
*   EDITEUR + JEU                                    *
*****************************************************/
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
    unsigned int img;
    unsigned int x,y,d,g,h,b;       //position (x,y), IntRect(d,g,h,b)
    bool colision;

};

struct img{
    char nom[100];
};

struct sons{
    char nom[100];
};

struct unsigned_int{
    unsigned int i;
};

struct options_map{
    int gravitee;   //gravitée du lvl
    int frottement; //frottement du lvl
    char sons[100]; //musique de fond
    char fond[100]; //image de foncd
    int chrono;      //temps pour realiser le lvl
    char nomMap[100];   //nom en toute lettre du niveau
    char nomVerif[16];  //nom permetant l'antis cheat
    char Type[100]; //type du niveau
    char mythologie[100];   //mythologie du lvl
    char AutoDialogue[1000];
};


struct heros{
  int acceleration;
  _type_direction direction;
  int vitesse;
  int x,y;
};

struct _skills{
    unsigned int ID;    //identifiant (cela sert pour etre comparé avec l'enumération créé automatiquement pour plus de lisibilitéé)
    char nom[50];   //nom du skill
    char description[1000]; //description de se que permet le skill
    char img[100];  //chemin relatif de l'image associé au skill
    int duree,recharge,mana;    //durée du skill, temps qu'il met à se recharger, et le mana consomé lors de l'utilisation
};

struct _sons_spacialises {
    int x,y;
    int attenuation;
    int minDistance;
    int volume;
    char nom[50];
    char dossier[50];
};
#endif
