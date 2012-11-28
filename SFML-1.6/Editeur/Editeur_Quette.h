#ifndef EDITEUR_QUETTE_HPP
#define EDITEUR_QUETTE_HPP

#include "Taille_fenetre.hpp"
#include "lister.hpp"
#include "ClasseRemake.hpp"

#include <iostream>
#include <QtGui>

/****************************************************
cette classe permet d'editer les interactions
de tous les objets et monstres boss
en les regroupan dans une seule fenettre,
e qui evite de les selectionner un à un
*/

class Editeur_Quette : public QFrame
{
    Q_OBJECT

public:

    //constructeur
    Editeur_Quette(QWidget *parent=0): QFrame(parent)
    {
        init();
    };

    //fonction d'initialisation commune à faire pour tous les constructeurs
    void init();

    //permet de donner les adresse des tableau auxquel ils faut acceder pour voir et modifier les différentes infos
    void SetPnjObj(std::vector<_OBJ> *tab_OBJ_struct,std::vector<_PNJ> *tab_PNJ_struct) {this->tab_OBJ_struct=tab_OBJ_struct;this->tab_PNJ_struct=tab_PNJ_struct;reload();};

public slots:

    /*******************************
        toutes les fonction suivantes
        prennent en paramettre
        l'index du tableau qu'il faut
        modifier
    */
    void Changer_dialogue_objet(int); //permet de changer la variable de dialogue de l'objet à la case spécifier du tableau
    void Changer_quette_objet(int);   //permet de changer la variable de quette de l'objet à la case spécifier du tableau
    void Changer_skill_objet(int);  //permet de changer la variable de skill de l'objet à la case spécifier du tableau

    void Changer_dialogue_pnj(int); //permet de changer la variable de dialogue du PNJ à la case spécifier du tableau
    void Changer_quette_pnj(int);//permet de changer la variable de quette du PNJ à la case spécifier du tableau
    void Changer_lvl_pnj(int);//permet de changer le niveau du PNJ à la case spécifier du tableau

    void Change_type_Edit(int);//permet de passe de la modification d'objet à boss ou inversement

    void reload();//permet de metre à jour l'affichage en fonction des element nouveau ajouter/suprimés, et pour voir que le type selectionné (monstres ou objets)

private:

    void reload_OBJ();  //permet de remettre à jour avec uniquement les objets
    void reload_PNJ();  //idem sauf que c'est avec les PNj de type Boss

    QScrollArea* scroll;    //objet permetant d'avoir une fenettre déronlante si il y a beaucoup à afficher
    QGridLayout* layoutScroll;  //layout prncipale contenant tous les vector
    QGridLayout* layout;
    QWidget* conteneur;         //objet contenant le layout principal, il contient tous les autre objet
    QPushButton* Reload;        //bouton permetant de mettre a jour la page on fonction d'un eventuel nouveau choix, ou nouveau ajouts sur la carte

    std::vector<_OBJ> *tab_OBJ_struct;      //tableau de tous les Objets
    std::vector<_PNJ> *tab_PNJ_struct;      //tableau de tous les pnj


    QLabel* entete[4];      //talbzeau contenant les entette pour indiquer à quoi servent les collones affichées
    QComboBox* ChoixTypeEdit;       //bouton permetant le choix du tri (pnj ou objets)
    std::vector<QLabel*> tab_type;  //tableau donnant une information basique sur l'objet
    std::vector<QTextEditIndex*> tab_dialogue;  //tableau des dialogues
    std::vector<QSpinBoxIndex*> tab_var_quette; //tableau des variables de quette necessaire (utilisé dans le jeu)
    std::vector<QSpinBoxIndex*> tab_lvl;    //tableau des niveau
    std::vector<QComboBoxIndex*> tab_id_skill;  //tableau contenant le skill associé

    enum _TYPEEDIT{EDIT_OBJETS=0,EDIT_PNJ} TYPEEDITION; //pour savoir quel mode d'edition est actuelement en cour
};


#endif
