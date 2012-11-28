#include "afficheur_options_perso.hpp"

using namespace std;

void AFFICHEUR_OPTION_PERSO::init()
{
    setWindowTitle("Reglage Des Options du Heros");
    direction=new QComboBox;
    direction->addItem("Bas");
    direction->addItem("Gauche");
    direction->addItem("Droite");
    direction->addItem("Haut");
    QObject::connect(direction,SIGNAL(currentIndexChanged(int)),this,SLOT(change_direction(int)));

    direction_l=new QLabel("Direction :");

    vitesse=new QSpinBox;
    vitesse->setRange(50,600);
    QObject::connect(vitesse,SIGNAL(valueChanged(int)),this,SLOT(change_vitesse(int)));

    vitesse_l=new QLabel("Vitesse: ");

    acceleration=new QSpinBox;
    acceleration->setRange(200,2000);
    QObject::connect(acceleration,SIGNAL(valueChanged(int)),this,SLOT(change_acceleration(int)));

    acceleration_l=new QLabel("Acceleration :");

    layout=new QGridLayout;

    layout->addWidget(animation_perso,0,0,2,2);
    layout->addWidget(direction_l,3,0);
    layout->addWidget(direction,3,1);
    layout->addWidget(vitesse_l,4,0);
    layout->addWidget(vitesse,4,1);
    layout->addWidget(acceleration_l,5,0);
    layout->addWidget(acceleration,5,1);

    setLayout(layout);

    resize(500,500);


};

void AFFICHEUR_OPTION_PERSO::change_vitesse(int v)
{
    perso->vitesse=v;
};

void AFFICHEUR_OPTION_PERSO::change_acceleration(int a)
{
    perso->acceleration=a;
};

void AFFICHEUR_OPTION_PERSO::change_direction(int d)
{
    perso->direction=(_type_direction) d;

};
