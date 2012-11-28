#ifndef MAP_OPTIONS_HPP_INCLUDED
#define MAP_OPTIONS_HPP_INCLUDED

#include "Taille_fenetre.hpp"

#include <iostream>

#include <QtGui>

class MAP_OPTION : public QFrame
{
    /**************************
    cette classe permet de gere les options qui sont associé à la carte
    */
    Q_OBJECT
public:
    MAP_OPTION ();  //constructeur

    void setMap(options_map ma); //permet de charger des options de carte (depuis un fichier par exemple)
    options_map getMapOption() {return map; }; //permet de récuperer les option de carte courant
public slots:

    void change_gravitee(double); //permet de changer la gravitéé
    void change_frottement(int);//permt de modifier les frottement
    void change_Fond(QString);//permet de modifier l'imaze de fon en spécifiant le chemin de celle ci
    void change_musique(QString); //change le chemin de la musique qui doit etre lancé en fond
    void change_chrono(int);//permet de changer le temps d'un chrono
    void change_NomMap(QString); // permet de changer le nom de la carte
    void change_Type(QString);// permet de changer le nom du type de niveau
    void change_Mythologie(QString);//permet de changer le nom de la mythologie
    void Change_AutoDialogue(); // permet de changer le dialogue qui se lance en debut de niveau

    signals:
    void FondChanged(); //permet de signaler que le fond à ete changé

private :

    options_map map;    //permet de stoquer les valeur des option de la carte

    QGridLayout*  layout; //layout de mis en forme de l'affichage

    QDoubleSpinBox* gravitee; //permet de modifier et choisir la gravitee
    QSpinBox* frottement,*Chrono; //idem pour changer les frottements, et temps pour realiser le niveau
    QComboBox* Fonds,*Sons;//permet de changer le chemin du fonds, et le fond
    QLineEdit *NomMap,*Type,*Mythologie;// permet de modifier les valeur du nom de la carte, de son type, et mythologie
    QTextEdit *AutoDialogue;//permet de changer la valeur du texte de lancement automatique

    QLabel* gravitee_l,*frottement_l,*Fonds_l,*Sons_l,*Chrono_l,*NomMap_l,*Type_l,*Mythologie_l,*AutoDialogue_l;//permet d'indiquer à l'utilisateur à quoi servent les bouton grace à du texte
};

#endif // MAP_OPTIONS_HPP_INCLUDED
