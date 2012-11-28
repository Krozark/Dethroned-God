#ifndef AFFICHEUR_OPTIONS_HPP
#define AFFICHEUR_OPTIONS_HPP

//#include "QSFMLCanvas.hpp"
#include <iostream>
//QT
#include <QtGui>
//SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
//perso
#include "Object.hpp"
#include "afficheur_sprite.hpp"
//#include "sons_pnj_option.hpp"

class AFFICHEUR_OPTION : public QFrame
{
    /**********************************
    Cette classe permet de modifier les caractéristique d'un objet, ou d'un pnj
    */
    Q_OBJECT

public:

    AFFICHEUR_OPTION(); //constructeur par defaut

    void set_options(_PNJ* P,sf::Sprite* o);        //permet de spécifier le pnj qui doit etre modifié , et l'image qui lui coresspond à afficher /modifier
    void set_options(_OBJ* O,sf::Sprite* o);        //permet de spécifier l'objet qui doit etre modifier, et l'image qui lui coresspond à afficher /modifier
    void set_sprite_all_items(sf::Sprite *s) {sprite_all_items=s;}; //permet d'avoir l'adresse du sprite des objet spéciaux

public slots:

    void change_direction(int); //permet de changer la direction de deplacement (PNJ) / skin (objet)
    void change_vue( int);//permet de modifier la distance de vue (pnj)
    void change_min( int);//permet de changer le nombre de case vers la gauche de deplacement (pnj)
    void change_max( int);//idem avec la droite
    void change_agressif(int);//permet de dire le pnj est agressif ou pas
    void change_boss(int);  //permet de dir si c'est un boss ou pas
    void change_volant(int); //permet de dire si il doit voler ou pas
    void change_degats( int);//permet de spésifier les degats
    void change_vie(int); //permet de changer la vie
    void change_vitesse(int);//permet de changer la vitesse de deplacement/frappe
    void change_lvl(int);//permet de changer le niveau d'un pnj
    void change_exp(int);//permet de changer l'exp que le pnj donne lors de sa mort
    void change_resize(double);//permet de spécifier le taux d'agrandissement du pnj/objet
    void change_quette(int);//permet de spécifier la variable de quette assiciée
    void change_element(int);//permet de spécifier l'element du pnj
    void change_type_obj(int);//permet de spécifier le type de l'objet
    void change_voleur(int);//permet de dir si le pnj est voleur ou pas
    void change_eparpieur(int);//permet de dire si le pnj balonce des objet partout ou pas
    void change_looter(int);//permet de dir si le pnj rammase les objet ou pas
    void change_or(int);//permet de definir l'or obtenu avec l'objet
    void Change_destination() {emit Choix_DestinationPressed();};//permet de changer la destination d'un teleporteur

    void change_dialogue(); //permet de deffinir le text qui est affiché lors du la mort d'un pnj, ou lors de l'utilisation d'un objet
    void change_dialogue_skill(int);    //permet de definir le texte affiche dans un parchemin pour introduir le gain d'un skill

signals:

    void Choix_DestinationPressed();    //permet de dire à d'autre objet que l'on vau modifier la destination d'un teleporteur

private:

    void afficher_boutons_pnj();    //permet d'activer ou desactiver les bouton liés au pnj
    void afficher_boutons_obj();    //idem avec les objets
//resources
    OBJECT pnj; //pnj animé
    sf::Sprite* current_object_selection; // sprite de l'objet qui est selectionné
    sf::Sprite* sprite_all_items;//sprite des objets spéciaux
    int taillex;    // permet de savoir le nombre de subdivision des images en x
    int tailley;    //idem en y
    sf::Image pnj_img; //image du pnj animé

    _PNJ* curent_PNJ;//structure contenant les infos du PNJ courant
    _OBJ* curent_OBJ;//idem avec un objet

    _type_edition type_edition;//permet de savoir le mode d'édition en cour (PNJ ou OBJET)

//fenettre avec anim du perso
    AFFICHEUR_SPRITE* fenettre_sprite;  //objet qui permet d'afficher le pnj en animé
//different bouton
    QComboBox* direction,*element,*type_objet; //bouton pour permetre de choisir le direction de depart, l'element du pnj, ou le type d'objet
    //QSlider
    QSpinBox * vision,*dep_min,*dep_max,*degats,*vie,*vitesse,*lvl,*exp,*quette,*dialogue_skill,*_or;  //permet de choisir le nombre de case de vision, de deplacement vers la gauche, vers la droite, le nombre de degats, la vie, vitesse de frappe/deplacement, niveau, expériance donné, variable de quette,texte d'interaction avec le hero, et l'or donné
    QDoubleSpinBox* taille; //permt de choisir le coeficient de reduction/augmentation de taille
    QTextEdit* dialogue_tape;//permet de choisir le texte a afficher
    QCheckBox* agressif,*boss,*volant,*voleur,*eparpieur,*looter;//permet de choisir les caractéristique de base
    QLabel* direction_l,*vision_l,*dep_max_l,*dep_min_l,*degats_l,*vie_l,*vitesse_l,*lvl_l,*dialogue_tape_l,*exp_l,*taille_l,*quette_l,*element_l,*type_objet_l,*dialogue_skill_l,*_or_l; // permet de montrer à l'utilisateur à quoi servent les boutons.
    QString son_mort,son_tape; //permet de stoquer les chemin des sons
    QPushButton *Choix_TP;//bouton qui permet de choisir la redefinition de la destination d'un teleporteur
//mise en page
    QWidget* page1,*page2,*page3,*page4;//widget de chaque page
    QTabWidget *onglets; //widget qui permet de cree des onglets
    QGridLayout* _page1,*_page2,*_page3,*_page4; // layout de chaque page

    _type_objet_sons type_son; // permet de choisir le type de son

    unsigned short int multiplicateur; //si c'est un boos coeficient multiplicateur de force entre monstre normeau et boss
};


#endif
