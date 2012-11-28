#include "map_options.hpp"
#include "lister.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

MAP_OPTION::MAP_OPTION()
{
    setWindowTitle("Option generale de la carte");

    layout=new QGridLayout;

    frottement=new QSpinBox;
    frottement->setRange(100,2000);
    frottement->setSingleStep(50);


    frottement_l=new QLabel("Frottement");

    Chrono=new QSpinBox;
    Chrono->setRange(-1,1000);
    Chrono->setSingleStep(1);

    Chrono_l=new QLabel("Chrono en sec");

    gravitee=new QDoubleSpinBox;
    gravitee->setRange(2,15);
    gravitee->setSingleStep(0.1);

    gravitee_l=new QLabel("Gravite");

    NomMap=new QLineEdit;
    NomMap_l=new QLabel("Nom Carte");

    Type_l=new QLabel("Type de carte");
    Type=new QLineEdit;


    Mythologie=new QLineEdit;
    Mythologie_l=new QLabel("Mythologie");


    LISTEUR listeur;

    Fonds=new QComboBox();

    Fonds_l=new QLabel("Fond");
    vector<string> tab=listeur.lister("./resources/image/fonds");


    Sons=new QComboBox();
    Sons_l=new QLabel("Musique");

    unsigned int _size=tab.size();
    for (unsigned int i=0;i<_size;i++)
        Fonds->addItem(tab[i].c_str());

    tab=listeur.lister("./sons/fond");
    _size=tab.size();
    for(unsigned int i=0;i<_size;i++)
        Sons->addItem(tab[i].c_str());

    AutoDialogue_l=new QLabel("Auto Dialogue de debut");
    AutoDialogue=new QTextEdit();


    QObject::connect(frottement,SIGNAL(valueChanged(int)),this,SLOT(change_frottement(int)));
    QObject::connect(gravitee,SIGNAL(valueChanged(double)),this,SLOT(change_gravitee(double)));
    QObject::connect(Chrono,SIGNAL(valueChanged(int)),this,SLOT(change_chrono(int)));

    QObject::connect(Fonds,SIGNAL(currentIndexChanged(QString)),this,SLOT(change_Fond(QString)));
    QObject::connect(Sons,SIGNAL(currentIndexChanged(QString)),this,SLOT(change_musique(QString)));

    QObject::connect(NomMap,SIGNAL(textChanged(QString)),this,SLOT(change_NomMap(QString)));
    QObject::connect(Type,SIGNAL(textChanged(QString)),this,SLOT(change_Type(QString)));
    QObject::connect(Mythologie,SIGNAL(textChanged(QString)),this,SLOT(change_Mythologie(QString)));

    QObject::connect(AutoDialogue,SIGNAL(textChanged()),this,SLOT(Change_AutoDialogue()));


    frottement->setValue(2000);
    Chrono->setValue(-1);
    gravitee->setValue(9.80);
    NomMap->setText("Aucun");
    Type->setText("Normal");
    Mythologie->setText("Aucune");

    change_frottement(frottement->value());
    change_chrono(Chrono->value());
    change_gravitee(gravitee->value());

    change_Fond(Fonds->currentText());
    change_musique(Sons->currentText());

    change_NomMap(NomMap->text ());
    change_Type(Type->text());
    change_Mythologie(Mythologie->text());

    strcpy(map.AutoDialogue," ");

    int i=0;
    layout->addWidget(frottement_l,i,0);
    layout->addWidget(frottement,i,1);i++;
    layout->addWidget(gravitee_l,i,0);
    layout->addWidget(gravitee,i,1);i++;
    layout->addWidget(Fonds_l,i,0);
    layout->addWidget(Fonds,i,1);i++;
    layout->addWidget(Sons_l,i,0);
    layout->addWidget(Sons,i,1);i++;
    layout->addWidget(Chrono_l,i,0);
    layout->addWidget(Chrono,i,1);i++;
    layout->addWidget(NomMap_l,i,0);
    layout->addWidget(NomMap,i,1);i++;
    layout->addWidget(Type_l,i,0);
    layout->addWidget(Type,i,1);i++;
    layout->addWidget(Mythologie_l,i,0);
    layout->addWidget(Mythologie,i,1);i++;
    layout->addWidget(AutoDialogue_l,i,0);
    layout->addWidget(AutoDialogue,i,1);


    setLayout(layout);

};

void MAP_OPTION::setMap(options_map ma)
{
    map=ma;
    frottement->setValue(map.frottement);
    gravitee->setValue(map.gravitee/10.0);

    Chrono->setValue(map.chrono);

    int i;
    //on met les combo box à jour
    //image de fond
    string fond=map.fond;
    string path="./resources/image/fonds/";

    if ((i=fond.find(path))<(int)fond.size())
        fond.replace(i,path.size(),"");
    if ((i=Fonds->findText(fond.c_str()))>-1)
        Fonds->setCurrentIndex(i);

    //sons de fond
    fond=map.sons;
    path="./sons/fond/";
    if ((i=fond.find(path))<(int)fond.size())
        fond.replace(i,path.size(),"");
    if ((i=Sons->findText(fond.c_str()))>-1)
        Sons->setCurrentIndex(i);

    NomMap->setText(map.nomMap);
    Type->setText(map.Type);
    Mythologie->setText(map.mythologie);
    AutoDialogue->setPlainText(map.AutoDialogue);

};

void MAP_OPTION::change_frottement(int f)
{
    map.frottement=f;
};

void MAP_OPTION::change_gravitee(double g)
{

    map.gravitee=(int)(((float)g)*10.0f);
};

void MAP_OPTION::change_Fond(QString s)
{
    const char* nom=string("./resources/image/fonds/"+s.toStdString()).c_str();
    strncpy(map.fond,nom,99);
    emit FondChanged();
};

void MAP_OPTION::change_musique(QString s)
{
    const char* nom=string("./sons/fond/"+s.toStdString()).c_str();
    strncpy(map.sons,nom,99);
};

void MAP_OPTION::change_chrono(int c)
{
  map.chrono=c;
};

void MAP_OPTION::change_NomMap(QString s)
{
    strncpy(map.nomMap,s.toStdString().c_str(),99);
};

void MAP_OPTION::change_Type(QString s)
{
    strncpy(map.Type,s.toStdString().c_str(),99);
};

void MAP_OPTION::change_Mythologie(QString s)
{
    strncpy(map.mythologie,s.toStdString().c_str(),99);
};

void MAP_OPTION::Change_AutoDialogue()
{
    strncpy(map.AutoDialogue,AutoDialogue->toPlainText().toStdString().c_str(),999);
};
