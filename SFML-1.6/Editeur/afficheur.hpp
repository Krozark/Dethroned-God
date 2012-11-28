#ifndef AFFICHEUR_HPP
#define AFFICHEUR_HPP

#include "QSFMLCanvas.hpp"
#include <iostream>
//QT
#include <QtGui>
//SFML
#include <SFML/Graphics.hpp>
//perso
#include "Taille_fenetre.hpp"
#include "afficheur_options.hpp"
#include "afficheur_options_perso.hpp"
#include "map_options.hpp"
#include "Editeur_skill.hpp"
#include "Editeur_Quette.h"
#include "Text_Encadre.hpp"
#include "sons_option.hpp"

class AFFICHEUR_GRILLE : public QSFMLCanvas
{
    /********************************************
    cette classe, est la classe principal par laquelle passe la mojorité
    des evenemets.
    elle delègue certains calcules à d'autres classe.
    Elle gere surtout tout ce qui concerne l'edition de la carte
    (positionner les decors, objets, pnj, hero ) et delegue
    la modification des caractéristique de ceux ci
    Elle gere également les racourcis clavier pour permettre une edition plus rappide des niveaux
    */
    Q_OBJECT

    public :

    AFFICHEUR_GRILLE(QWidget* Parent=0) : QSFMLCanvas(Parent, QPoint(0,0), QSize(taille_objets_pix*taille_carte_case_x,taille_objets_pix*taille_carte_case_y))
    {
        taille_x=taille_carte_case_x;
        taille_y=taille_carte_case_y;
        init();

    };

    void mousePressEvent ( QMouseEvent * event );       //permet de gere les evenement souris lors du click
    void mouseReleaseEvent (QMouseEvent *event);        //permet de gerer les evenemetn souris lors du relachement
    void mouseMoveEvent ( QMouseEvent * event );        //permet de gere les evenement souris lors de son deplacement
    void keyPressEvent (QKeyEvent * event);             //permet de gere les evenement clavier (raccourcis)
    void keyReleaseEvent ( QKeyEvent * event );         //permet de gere les evenement clavier (stop des raccourcis)

    void OnInit();     //ce qui est fait au premeier lancement de la class
    void OnUpdate();   //se qui est vérifié en permanence et fait à chaque frame
    void init();       //se qui est commun aux constructeurs

    void Set_sprite_courant(sf::Sprite spr) {sprite_courant=spr;};      //permet de dire quel sprite est actuelement en selection
    void add_PNJ(); //permet d'ajouter un PNJ à la carte
    void add_OBJ(); //permet d'ajouter un objet à la carte
    void Set_sprite_courant_PNJ(sf::Sprite s) {sprite_courant_PNJ=s;}; //permet de dire quel est le sprite de l'objet courant
    void Set_sprite_courant_OBJ(sf::Sprite s) {sprite_courant_OBJ=s;}; //idem masi avec les objets
    void Set_grille( std::vector<sf::Sprite> g);    //permet d'obtenir la grille de base à afficher
    void SetAutoImage(sf::Image* img) {AutoImage=img;}; //permet de dire quelle est l'image utilisé pour l'edition des contours automatique
    void SetType(_type_objets t);   // permet de dir le type d'objet qui est actuelement ajouter
    void SetLvl(int l); //permet de modifier le niveau des monstres qui seron ajouter (calcul des stats en fonction du niveau)
    void MajAuto(); //permet de mettre à jours les different sprite autours du curseurs dans le cas d'un ajout ou d'une supretion en mode d'edition automatique

    void UpLevelPnj(int u);

    std::vector<sf::Sprite> grille;             //tableau de tous les srite qui sont affiché (grille, ou sripte de la carte)
    std::vector<sf::Sprite> grille_collision;   //permet de montrer graphiquemet si il y a une collision ou pas à une case donnée

    std::vector<sf::Sprite> tab_PNJ;    //tableau de tous les PNJ qui sont sur la carte (sprite)
    std::vector<_PNJ> tab_PNJ_struct;   //tableau de tous les pnj qui sont sur la carte (leur informations)

    std::vector<sf::Sprite> tab_OBJ;    //tableau de tous les objets qui sont sur la carte (sprites)
    std::vector<_OBJ> tab_OBJ_struct;   //tableau de tous les objet qui sont sur la cartes (infos)

    std::vector<Sons_Spacialise> tab_SONS_struct; //tableau de tous les sons

    sf::Image image_grille,image_colision_no,Image_vide,image_mur,Image_TP,image_son; //differentes images qui sont utilisés (grille , si il u a une colision, image transparente, image des mur invisible, image associé à la sortie des teleporteurs)
    sf::Sprite sprite_mur,sprite_fond,sprite_son;  // sprite associé aux image des mur invisible et à l'image de fond du niveau
    sf::Image image_fond;   //image de fond du niveau


    void Set_PNJ(_PNJ p,sf::Image* i);  //permet d'ajouter un PNG avec les infos de base, et l'image qui lui est assicié
    void Set_OBJ(_OBJ o,sf::Image* i);  //idem masi avec un objet
    void Set_Heros_coord(int x,int y); //permet de modifier la position du hero
    void Set_Map(options_map ma) {map_option->setMap(ma);modifFond();}; //permet de dire quelle sont des differents options associé à la cartes
    void SetAuto(bool a){Auto=a;};  //permet de dir si l'on est en mode auto ou pas (gestion des bordures tou seul)
//    void SetDestTP(_OBJ*);//permet de definir la destination du teleporteur actuelement en cour d'edition
    options_map getMapOption() {return map_option->getMapOption();};    //permet d'avoir les option de la carte

    void setPointOrigineVue(Vector2i i) {point=i;}; //permet d'avoir les coordonnée de l'ange haut gauche pour n'afficher que se qu'il faut à l'écrant (un bout de la carte)
    void setClickSouris(Vector2f * cl,Vector2f * rel) {clickPush=cl;clickRelase=rel;}   //permet de dir ou la souris à ete enfoncé, et ou elle est relaché (pour permetre de bouger la carte ave l'apuie de la molette)

    heros curent_Heros; //caractéristique du hero
    TEXT_ENCADRE Aide;  //texte d'aide afin de dire se qui est actuelement fait

    public slots :

    void reset();       //permet d'effacer les info de la cartes acctuelle poour en commencer une nouvelle
    void Set_type_edition(int c);   //permet de dir se qui est actuelement editer
    void modifFond();   //permet de mettre a jour l'image du fond de niveau
    void ChangeDestTp() {DestTP=true;}; //permet de dir que l'on veu changer la destination d'un teleporteur

    signals:
    void Clicked(); //pour dire que la souris à été enfoncée
    void Relased(); //pour dire que le souris à etre relachée


private :

    Vector2i point;         //coordonée de l'angle supérieur gauche
    Vector2f *clickPush,*clickRelase;   //coordonée des point d'enfoncement et de relachement de la souris

    std::vector<sf::Sprite>::iterator grille_iter; //permet le parcour du tableau de sprite

    sf::Sprite  sprite_courant,     //sprite qui est actuelement utilisé
                sprite_courant_PNJ, //sprite du PNJ acctuelement en edition
                sprite_courant_OBJ, //idemen avec un objet
                sprite_all_items;   //sprite contenant le skill de tous les objets spéciaux

    _PNJ curent_PNJ;    //information sur le pnj courant
    _OBJ curent_OBJ,*Telep; //information sur l'objet courant, et sur le teleporteur courant
    _type_objets currentType,currentTypeReal;   //permet de gérer le type des objet avec un raccourcis clavier, et pendant un raccourcis clavier
    int curentLvl;  //niveau qui doit etre associer au monstre qui sont ajoutes
    MAP_OPTION* map_option; //information de la carte


    sf::Image hero_img,image_all_items,telepor_vert,etoile_boss_I;    //image utilisé pour le hero, les objet spéciaux, et teleporteur
    sf::Image* AutoImage;   //pointeur sur l'image du skin de niveau en mode automatique

    OBJECT *heros_obj;  //animation du hero
    AFFICHEUR_OPTION_PERSO* option_perso;   //fenettre permetant de gere les options du hero

    sf::Sprite gr,vide,TP,telepor,etoile_boss_S;  //different sprites grille de base, sprite transparent,destination de teleporteur en cour d'edition, et destination des autres teleporteur
    Editeur_Skill* Fenettre_skill;  //fenettre qui permet de gerer l'edition des skill
    Editeur_Quette* Fenettre_quette;//fenettre qui permet d'editer plus facilement toutes les interaction de quette en une seulle fenettre
    AFFICHEUR_OPTION* Options;  //fenettre qui permets de gere les option des objets ou PNJ
    SONS_OPTION *    sons_option;

    bool show_options,Auto,collision,PressedRight,PressedLeft,DestTP;   //different bool pour tester l'etat. savoir si il faut afficher les options du mode d'edition en cour, savoir si le mod auto est activé , savoir si il y a des collision ou pas
    //savoir si le boutant droit ou gauche de la souris à ete pressé, et pour savoir si on doit modifier la destination du teleporteur courant

    void MajSpriteCourant(int c,bool first=false);  // fonction qui permet de mettre à jours la grille en lui specifiant la case (cette fonction est recursive dans la cas du l'edition auto, d'ou le bool )
    inline void MajSpriteCourant(int x,int y,bool first=false) {MajSpriteCourant(convertToCase(x,y),first);}; //idem, masi avec les coordonée de la case à la place de l'index
    void MajSpriteCourantDel(int c); //permet de supprimer le decort mis ) la case spécifié
    inline bool IsImageAuto(int c) {return grille[c].GetImage()==AutoImage;}; // savoir si le sprite à la case spécifié fait partie de l'edition auto
    inline bool IsImageAuto(int x,int y) {return IsImageAuto(convertToCase(x,y));}; //idemen, mais avec les coordonée

    inline int convertToCase(Vector2i v) {return (v.y*taille_x+v.x);}; //fonction qui permet de convertir une position en index du tableau de la grille
    inline Vector2i convertToVector(int c) { Vector2i v;v.x=c%taille_x;v.y=c/taille_x;return v;};   // permet de convertir un idex en coordonées
    inline int convertToCase(int x,int y) {return convertToCase(Vector2i(x,y));};   //fonction qui permet de convertir une position en index du tableau de la grille

    inline bool testProximite(Vector2i V) { return (grille[convertToCase(V.x,V.y)].GetImage() != &image_grille && grille[convertToCase(V.x,V.y)].GetImage() != &image_mur);}; // permet de savoir si il y a des sprite mis en edition auto autour d'une coordonées
    inline bool testProximite(int c) { return testProximite(convertToVector(c));}; //idem, masi avec un l'index du talbeau en parametre
    inline bool testProximite(int x,int y){ return testProximite(convertToCase(x,y));}; //idem avec des coordonée en parametre

    struct {
    unsigned int i; //case du pnj concernéé dans le vector
    enum DIR{MIN=0,MAX} dir; //si c'est le minimum ou le max à redéfinir
    } ref_pnj; //structure qui sert a savoir quel pnj est utilisé en se moment

    int taille_x,taille_y; //taille en nombre de cases
    _type_edition  type_edition; //type d'édition en cour

};

#endif
