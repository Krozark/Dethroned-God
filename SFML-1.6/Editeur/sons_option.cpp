#include "sons_option.hpp"

using namespace std;

void SONS_OPTION::init()
{
    curent_son=NULL;
    setWindowTitle("Editeur des sons spacialises");

    int i=0;

    layout=new QGridLayout;

    Play_Stop=new QPushButton("Play");
    QObject::connect(Play_Stop,SIGNAL(clicked()),this,SLOT(Play_stop()));

    chemin="./sons/autres/";

    vector<string> tab_nom=listeur.lister(chemin.c_str());

    Choix_son=new QComboBox();
    for (unsigned int i=0;i<tab_nom.size();i++)
    {
        Choix_son->addItem(tab_nom[i].c_str());
    }
    QObject::connect(Choix_son,SIGNAL(currentIndexChanged(QString)),this,SLOT(ChangeSon(QString)));

    music.OpenFromFile(chemin+Choix_son->itemText(0).toStdString());
    layout->addWidget(Choix_son,i,0);
    layout->addWidget(Play_Stop,i,1);++i;

    Type_Sons=new QPushButton("Fonds");
    QObject::connect(Type_Sons,SIGNAL(clicked()),this,SLOT(ChangeTypeSon()));
    layout->addWidget(Type_Sons,i,0);++i;

    Label[0]=new QLabel("Attenuation");
    Label[1]=new QLabel("MinDistance");
    Label[2]=new QLabel("Volume");

    attenuation=new QDoubleSpinBox;
    attenuation->setRange(0.1,50);
    attenuation->setSingleStep(0.5);
    attenuation->setValue(1);
    QObject::connect(attenuation,SIGNAL(valueChanged(double)),this,SLOT(ChangeAttenuation(double)));
    layout->addWidget(Label[0],i,0);
    layout->addWidget(attenuation,i,1);++i;


    MinDistance=new QDoubleSpinBox;
    MinDistance->setRange(1,1000);
    MinDistance->setSingleStep(1);
    MinDistance->setValue(100);
    QObject::connect(MinDistance,SIGNAL(valueChanged(double)),this,SLOT(ChangeMinDistance(double)));
    layout->addWidget(Label[1],i,0);
    layout->addWidget(MinDistance,i,1);++i;

    Volume=new QDoubleSpinBox;
    Volume->setRange(0.5,100);
    Volume->setSingleStep(0.5);
    Volume->setValue(100);
    QObject::connect(Volume,SIGNAL(valueChanged(double)),this,SLOT(ChangeVolume(double)));
    layout->addWidget(Label[2],i,0);
    layout->addWidget(Volume,i,1);++i;


    setLayout(layout);
    hide();

};

void SONS_OPTION::closeEvent(QCloseEvent * event)
{
    if (Play_Stop->text()=="Stop")
    {
        int res=QMessageBox::critical(this, "Verification", "Voulez vous stoper la musique?",QMessageBox::Yes | QMessageBox::No);
        if (res ==QMessageBox::Yes)
           Play_stop();
    }

    event->accept();
}

void SONS_OPTION::MAJ()
{
    int i=0;


    if (string(curent_son->nom)==" ")
        strncpy(curent_son->nom,Choix_son->	itemText(0).toStdString().c_str(),99);

    if ((i=Choix_son->findText(curent_son->nom))>-1)
        Choix_son->setCurrentIndex(i);
    else
    {
        ChangeTypeSon();
        if ((i=Choix_son->findText(curent_son->nom))>-1)
            Choix_son->setCurrentIndex(i);
        else
             Choix_son->setCurrentIndex(0);
    }

    attenuation->setValue(curent_son->attenuation/10.0);
    MinDistance->setValue(curent_son->MinDistance/10.0);
    Volume->setValue(curent_son->Volume/10.0);

};

void SONS_OPTION::ChangeTypeSon()
{
    if (Type_Sons->text()=="Fonds")
    {
        Type_Sons->setText("Autres");
        chemin="./sons/fond/";
    }
    else
    {
        Type_Sons->setText("Fonds");
        chemin="./sons/autres/";
    }

    vector<string> tab_nom=listeur.lister(chemin.c_str());

    Choix_son->clear();
    for (unsigned int i=0;i<tab_nom.size();i++)
    {
        Choix_son->addItem(tab_nom[i].c_str());
    }
    music.OpenFromFile(chemin+Choix_son->itemText(0).toStdString());
};

void SONS_OPTION::Play_stop()
{
    if (Play_Stop->text()=="Play")
    {
        Play_Stop->setText("Stop");
        music.Play();
    }
    else
    {
        Play_Stop->setText("Play");
        music.Stop();
    }

};

void SONS_OPTION::ChangeSon(QString s)
{
    music.Stop();

    strncpy(curent_son->nom,s.toStdString().c_str(),49);
    strncpy(curent_son->dossier,chemin.c_str(),49);

    music.OpenFromFile(chemin+s.toStdString());

     if (Play_Stop->text()=="Stop")
         music.Play();

};

void SONS_OPTION::ChangeAttenuation(double d)
{
    curent_son->attenuation=(int)(d*10.0);

};

void SONS_OPTION::ChangeMinDistance(double d)
{
    curent_son->MinDistance=(int)(d*10.0);

};

void SONS_OPTION::ChangeVolume(double d)
{
    curent_son->Volume=(int)(d*10.0);
    music.SetVolume(d);

};
