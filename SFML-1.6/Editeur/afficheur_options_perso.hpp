#ifndef AFFICHEUR_OPTION_PERSO_HPP
#define AFFICHEUR_OPTION_PERSO_HPP

#include <iostream>
//QT
#include <QtGui>
//SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
//perso
#include "Taille_fenetre.hpp"
#include "afficheur_sprite.hpp"
#include "Object.hpp"

class AFFICHEUR_OPTION_PERSO : public QFrame
{
    /***********************************
    cette classe permet de deffinir les differentes
    options du hero
    */
    Q_OBJECT
public:

    AFFICHEUR_OPTION_PERSO(heros* p,OBJECT obj,QWidget* parent =0) :QFrame (parent) //constructeur de base
    {
        animation_perso=new AFFICHEUR_SPRITE(this);
        animation_perso->resize(400,400);
        animation_perso->Set_sprite(obj);
        animation_perso->spr.Resize(300,300);
        animation_perso->spr.SetPosition(150,50);

        perso=p;

        init();
    };

    void init();// initialisation, a appeler dans les constructeurs
    void setPerso(heros* p) {perso=p;}; // permet de definir le hero

    void set_infos(int d,int v,int a) {direction->setCurrentIndex(d); // permet de definire la direction
                                        vitesse->setValue(v); //le vitesse
                                        acceleration->setValue(a);}; //et l'acceleration

public slots:

    void change_direction(int); //permet de changer la direction
    void change_vitesse(int); //permet de changer la vitesse
    void change_acceleration(int); // permet de changer l'acceleration

private:

    AFFICHEUR_SPRITE* animation_perso; //objet qui permet d'afficher une animation du hero

    QComboBox* direction; //choix de la direction du hero
    QSpinBox* vitesse,*acceleration;// choix de la vitesse et de l'acceleration
    QGridLayout* layout; //layout de mise en page des fifferent widgets

    QLabel* vitesse_l,*acceleration_l,*direction_l; //permet d'indiquer à l'utlisateur a quoi servent les different widgets

    heros* perso; // permet de garder en memoir l'adresse du hero


};


#endif
