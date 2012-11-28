#include "afficheur_options.hpp"
#include <sstream>
#include <math.h>


AFFICHEUR_OPTION::AFFICHEUR_OPTION()
{
    fenettre_sprite=new AFFICHEUR_SPRITE();
    type_son=RIEN_SON;

    multiplicateur=1;

    setWindowTitle("Reglage Des Options des OBJ et PNJ");
    resize(fenettre_taille_x/3, fenettre_taille_y/3*2);
    hide();

    curent_OBJ=new _OBJ;
    curent_PNJ=new _PNJ;

    type_edition=PNJ;

    //tab onglets
    onglets=new QTabWidget(this);
    onglets->setGeometry(0,0,fenettre_taille_x/3, fenettre_taille_y/3*2);

    //page 1 direction
    page1=new QWidget;
    _page1=new QGridLayout;

    direction=new QComboBox();
    direction->addItem("Bas");
    direction->addItem("Gauche");
    direction->addItem("Droite");
    direction->addItem("Haut");
    QObject::connect(direction,SIGNAL(currentIndexChanged(int)),this,SLOT(change_direction(int)));

    direction_l=new QLabel("Direction");

    lvl=new QSpinBox();
    lvl->setRange(1,59);
    QObject::connect(lvl,SIGNAL(valueChanged(int)),this,SLOT(change_lvl(int)));
    lvl_l=new QLabel("Lvl");

    boss=new QCheckBox("Boss?");
    QObject::connect(boss,SIGNAL(stateChanged(int)),this,SLOT(change_boss(int)));

    volant=new QCheckBox("Volant?");
    QObject::connect(volant,SIGNAL(stateChanged(int)),this,SLOT(change_volant(int)));

    voleur=new QCheckBox("Voleur?");
    QObject::connect(voleur,SIGNAL(stateChanged(int)),this,SLOT(change_voleur(int)));

    eparpieur=new QCheckBox("Eparpieur?");
    QObject::connect(eparpieur,SIGNAL(stateChanged(int)),this,SLOT(change_eparpieur(int)));

    looter=new QCheckBox("Looteur?");
    QObject::connect(volant,SIGNAL(stateChanged(int)),this,SLOT(change_looter(int)));

    taille=new QDoubleSpinBox();
    taille->setRange(0.2,5);
    taille->setDecimals(1);
    QObject::connect(taille,SIGNAL(valueChanged(double)),this,SLOT(change_resize(double)));
    taille_l=new QLabel("taux d'agrandissement du sprite");
    taille->setSingleStep(0.1);


    type_objet=new QComboBox;
    type_objet->addItem("Potion Vie");
    type_objet->addItem("Potion Mana");
    type_objet->addItem("Coffre");
    type_objet->addItem("Parchemin");
    type_objet->addItem("Sauvegarde");
    type_objet->addItem("Sauvgarde Fix");
    type_objet->addItem("Teleporteur");
    type_objet->addItem("Or");
    type_objet->addItem("TRAMPOLINE");
    type_objet->addItem("Accelerateur");
    type_objet->addItem("Piege");
    type_objet->addItem("Portail");
    type_objet->addItem("Marchand");
    type_objet->addItem("Quette");
    type_objet->addItem("Rien");
    type_objet->addItem("Telep.Quette");
    type_objet_l=new QLabel("Type d'objet");

    QObject::connect(type_objet,SIGNAL(currentIndexChanged(int)),this,SLOT(change_type_obj(int)));

    int i=2;
    _page1->addWidget(fenettre_sprite,0,0,2,6);
    _page1->addWidget(boss,i,0);
    _page1->addWidget(volant,i,1);
    _page1->addWidget(voleur,i,2);
    _page1->addWidget(looter,i,3);
    _page1->addWidget(eparpieur,i,4);i++;
    _page1->addWidget(lvl_l,i,0,1,3);
    _page1->addWidget(lvl,i,4,1,2);i++;
    _page1->addWidget(taille_l,i,0,1,3);
    _page1->addWidget(taille,i,4,1,2);i++;
    _page1->addWidget(direction_l,i,0,1,3);
    _page1->addWidget(direction,i,4,1,2);i++;
    _page1->addWidget(type_objet_l,i,0,1,3);
    _page1->addWidget(type_objet,i,4,1,2);i++;

    page1->setLayout(_page1);
    //page 2 vision + degats
    page2=new QWidget;
    _page2=new QGridLayout;

    vision=new QSpinBox();
    vision->setRange(1,taille_carte_case_x);
    QObject::connect(vision, SIGNAL(valueChanged(int)),this,SLOT(change_vue(int)));
    vision_l=new QLabel("Vision");

    degats=new QSpinBox();
    degats->setRange(0,999999);
    QObject::connect(degats, SIGNAL(valueChanged(int)),this,SLOT(change_degats(int)));
    degats_l=new QLabel("Degats");

    exp=new QSpinBox();
    exp->setRange(0,9999);
    QObject::connect(exp,SIGNAL(valueChanged(int)),this,SLOT(change_exp(int)));
    exp_l=new QLabel("Exp");

    agressif=new QCheckBox("Agressif?");
    QObject::connect(agressif,SIGNAL(stateChanged(int)),this,SLOT(change_agressif(int)));

    vie=new QSpinBox();
    vie->setRange(0,9999999);
    QObject::connect(vie,SIGNAL(valueChanged(int)),this,SLOT(change_vie(int)));
    vie_l=new QLabel("Vie");

    _page2->addWidget(vision_l,0,0);
    _page2->addWidget(vision,0,1);

    _page2->addWidget(degats_l,1,0);
    _page2->addWidget(degats,1,1);

    _page2->addWidget(exp_l,2,0);
    _page2->addWidget(exp,2,1);

    _page2->addWidget(vie_l,3,0);
    _page2->addWidget(vie,3,1);

    _page2->addWidget(agressif,4,0);

    page2->setLayout(_page2);

    //page 3 deplacement
    page3=new QWidget;
    _page3=new QGridLayout;

    dep_min=new QSpinBox();
    dep_min->setRange(0,taille_carte_case_x);
    QObject::connect(dep_min, SIGNAL(valueChanged(int)),this,SLOT(change_min(int)));

    dep_min_l=new QLabel("Deplacement arriere");

    dep_max=new QSpinBox();
    dep_max->setRange(0,taille_carte_case_x);
    QObject::connect(dep_max, SIGNAL(valueChanged(int)),this,SLOT(change_max(int)));
    dep_max_l=new QLabel("Deplacement avant");

    vitesse=new QSpinBox;
    vitesse->setRange(0,999999);
    QObject::connect(vitesse,SIGNAL(valueChanged(int)),this,SLOT(change_vitesse(int)));
    vitesse_l=new QLabel("Vitesse/Rebond/Acceleration");

    Choix_TP=new QPushButton("Choix Destination");
    QObject::connect(Choix_TP,SIGNAL(clicked()),this,SLOT(Change_destination()));

    _or=new QSpinBox();
    _or->setRange(0,1000);
    _or->setSingleStep(2);
    QObject::connect(_or,SIGNAL(valueChanged(int)),this,SLOT(change_or(int)));
    _or_l=new QLabel("Or");



    _page3->addWidget(dep_min_l,0,0);
    _page3->addWidget(dep_min,0,1);

    _page3->addWidget(dep_max_l,1,0);
    _page3->addWidget(dep_max,1,1);

    _page3->addWidget(vitesse_l,2,0);
    _page3->addWidget(vitesse,2,1);

    _page3->addWidget(Choix_TP,3,0);

    _page3->addWidget(_or_l,4,0);
    _page3->addWidget(_or,4,1);

    page3->setLayout(_page3);
    page3->setVisible(false);

    //page 4 Sons et dialogue
    page4=new QWidget;
    _page4=new QGridLayout;

    dialogue_tape=new QTextEdit();
    QObject::connect(dialogue_tape,SIGNAL(textChanged()),this,SLOT(change_dialogue()));
    dialogue_tape_l=new QLabel("Dialogue de rencontre avec le heros");

    dialogue_skill=new QSpinBox();
    QObject::connect(dialogue_skill,SIGNAL(valueChanged(int)),this,SLOT(change_dialogue_skill(int)));
    dialogue_skill_l=new QLabel("Skill a ajouter");


    element=new QComboBox;
    element->addItem("Neutre");
    element->addItem("Feu");
    element->addItem("Eau");
    element->addItem("Terre");
    element->addItem("Aire");
    element->addItem("Ombre");
    element->addItem("Lumiere");
    QObject::connect(element,SIGNAL(currentIndexChanged(int)),this,SLOT(change_element(int)));
    element_l=new QLabel("Element");

    quette=new QSpinBox;
    quette->setRange(0,99);
    quette_l=new QLabel("numero de quette (0=non)");
    QObject::connect(quette,SIGNAL(valueChanged(int)),this,SLOT(change_quette(int)));

    i=0;
    _page4->addWidget(dialogue_tape_l,i,0);
    _page4->addWidget(dialogue_tape,i,1);i++;
    _page4->addWidget(dialogue_skill_l,i,0);
    _page4->addWidget(dialogue_skill,i,1);i++;
    _page4->addWidget(element_l,i,0);
    _page4->addWidget(element,i,1);i++;
    _page4->addWidget(quette_l,i,0);
    _page4->addWidget(quette,i,1);

    page4->setLayout(_page4);
    page4->setVisible(false);

    //mis en place des onglets
    onglets->addTab(page1,"General");
    onglets->addTab(page2,"Vision,Degats et Vie");
    onglets->addTab(page3,"Deplacement");
    onglets->addTab(page4,"Interactions");

    pnj.SetPosition(0,0);
    multiplicateur=1;
};

void AFFICHEUR_OPTION::set_options(_PNJ* P,Sprite* s)
{
    type_edition=PNJ;
    curent_PNJ=P;
    taillex=s->GetImage()->GetWidth()/P->tx;
    tailley=s->GetImage()->GetHeight()/P->ty;
    current_object_selection=s;


    string nom=curent_PNJ->nom;
    pnj_img.LoadFromFile(string(curent_PNJ->dossier)+"/"+nom);
    pnj.charger_anim(pnj_img,curent_PNJ->tx,curent_PNJ->ty,(int)curent_PNJ->direction+1);

    if (nom.find("_EAU_")<nom.size())
        curent_PNJ->element=EAU;
    else if (nom.find("_TERRE_")<nom.size())
        curent_PNJ->element=TERRE;
    else if (nom.find("_AIRE_")<nom.size())
        curent_PNJ->element=AIRE;
    else if (nom.find("_FEU_")<nom.size())
        curent_PNJ->element=FEU;
    else if (nom.find("_OMBRE_")<nom.size())
        curent_PNJ->element=OMBRE;
    else if (nom.find("_LUMIERE_")<nom.size())
        curent_PNJ->element=LUMIERE;
    else curent_PNJ->element=NEUTRE;

    if (nom.find("_VOL_")<nom.size() || nom.find("DRAGON_")<nom.size() || nom.find("OISEAU_")<nom.size())
        curent_PNJ->volant=true;
    else curent_PNJ->volant=false;

    if(nom.find("DRAGON_")<nom.size())
        type_son=DRAGON;
    else if(nom.find("OISEAU_")<nom.size())
        type_son=OISEAU;
    else if(nom.find("ANGE_")<nom.size())
        type_son=ANGE;
    else if(nom.find("POISSON_")<nom.size())
        type_son=POISSON;
    else if(nom.find("LOUP_")<nom.size())
        type_son=LOUP;
    else if(nom.find("BETTE_")<nom.size())
        type_son=BETTE;
    else if(nom.find("DEVIL_")<nom.size())
        type_son=DEVIL;
    else if(nom.find("GHOST_")<nom.size())
        type_son=GHOST;
    else if(nom.find("SERPENT_")<nom.size())
        type_son=SERPENT;
    else if(nom.find("ELEMENTAL_")<nom.size())
        type_son=ELEMENTAL;
    else if(nom.find("GOBLIN_")<nom.size())
        type_son=GOBLIN;
    else if(nom.find("INSECT_")<nom.size())
        type_son=INSECT;
    else if(nom.find("IMFORME_")<nom.size())
        type_son=IMFORME;
    else if(nom.find("UNDEAD_")<nom.size())
        type_son=UNDEAD;
    else if(nom.find("HUMAN_")<nom.size())
        type_son=HUMAN;
    else type_son=RIEN_SON;

    curent_PNJ->voleur=(nom.find("VOLEUR_") < nom.size());
    curent_PNJ->looter=(nom.find("LOOTEUR_")< nom.size());
    curent_PNJ->eparpieur=(nom.find("EPERPIEUR_")<nom.size());

    curent_PNJ->skin_tape=(nom.find("_T_")<nom.size())? true:false;

    fenettre_sprite->Set_sprite(pnj);

    change_lvl(P->lvl);
    lvl->setValue(P->lvl);
    taille->setValue(P->resize/10);
    vitesse->setValue(P->vitesse);

    afficher_boutons_pnj();
};

void AFFICHEUR_OPTION::set_options(_OBJ* O,Sprite* s)
{
    type_son=(O->type==MARCHAND)? MERCO:RIEN_SON;
    type_edition=OBJ;
    curent_OBJ=O;
    taillex=s->GetImage()->GetWidth()/O->tx;
    tailley=s->GetImage()->GetHeight()/O->ty;

    taille->setValue(O->resize/10);
    current_object_selection=s;
    type_objet->setCurrentIndex(O->type);


    pnj_img.LoadFromFile(string(curent_OBJ->dossier)+"/"+string(curent_OBJ->nom));
    pnj.charger_anim(pnj_img,curent_OBJ->tx,curent_OBJ->ty,(int)curent_OBJ->direction+1);

    fenettre_sprite->Set_sprite(pnj);

    afficher_boutons_obj();
};

void AFFICHEUR_OPTION::change_direction(int d)
{
    _type_direction dir;
    switch(d)
    {
        case 0:pnj.SetAnim(&pnj.GoDown);dir=BAS;
        break;

        case 1:pnj.SetAnim(&pnj.GoLeft);dir=GAUCHE;
        break;

        case 2:pnj.SetAnim(&pnj.GoRight);dir=DROITE;
        break;

        case 3:pnj.SetAnim(&pnj.GoUp);dir=HAUT;
        break;

        default:pnj.SetAnim(&pnj.GoDown);dir=(_type_direction)d;
        break;

    }

    if (type_edition==PNJ)
    {
        curent_PNJ->direction= dir;
        current_object_selection->SetSubRect(IntRect(0,(int)curent_PNJ->direction*tailley,taillex,(int)curent_PNJ->direction*tailley+tailley));


    }
    else if (type_edition==OBJ)
    {
        curent_OBJ->direction=dir;
        current_object_selection->SetSubRect(IntRect(0,(int)curent_OBJ->direction*tailley,taillex,(int)curent_OBJ->direction*tailley+tailley));

    }
    fenettre_sprite->Set_sprite(pnj);
};

void AFFICHEUR_OPTION::change_vue( int v)
{
    if (type_edition==PNJ)
    curent_PNJ->vue=v;

};

void AFFICHEUR_OPTION::change_or(int v)
{
    if(type_edition==OBJ && curent_OBJ->type==OR)
        curent_OBJ->destx=v;
};

void AFFICHEUR_OPTION::change_min( int d)
{
     if (type_edition==PNJ)
    curent_PNJ->min=d;

};


void AFFICHEUR_OPTION::change_max( int d)
{
     if (type_edition==PNJ)
    curent_PNJ->max=d;

};

void AFFICHEUR_OPTION::change_agressif(int a)
{
     if (type_edition==PNJ)
            curent_PNJ->agressif=(a==2);
};

void AFFICHEUR_OPTION::change_voleur(int a)
{
  if(type_edition ==PNJ)
        curent_PNJ->voleur=(a == 2);
};

void AFFICHEUR_OPTION::change_eparpieur(int a)
{
  if(type_edition ==PNJ)
        curent_PNJ->eparpieur=(a == 2);
};

void AFFICHEUR_OPTION::change_looter(int a)
{
  if(type_edition ==PNJ)
        curent_PNJ->looter=(a == 2);
};

void AFFICHEUR_OPTION::change_boss(int a)
{
    if (type_edition==PNJ)
    {
        if (a==2)
        {
            curent_PNJ->boss=true;
            multiplicateur=2;
            quette->setEnabled(true);
            quette->setValue(1);
        }
        else
        {
            curent_PNJ->boss=false;
            multiplicateur=1;
            quette->setValue(0);
            quette->setEnabled(false);
        }
         change_lvl(curent_PNJ->lvl);
    }
};

void AFFICHEUR_OPTION::change_volant(int a)
{
    if (type_edition==PNJ)
    {
        if (a==2)
            curent_PNJ->volant=true;
        else
            curent_PNJ->volant=false;
    }
}

void AFFICHEUR_OPTION::change_degats( int d)
{
     if (type_edition==PNJ)
        curent_PNJ->degats=d;
    else if (type_edition==OBJ && curent_OBJ->type ==PIEGE)
        curent_OBJ->destx=d;

};

void AFFICHEUR_OPTION::change_vie(int v)
{
    if(type_edition==PNJ)
    curent_PNJ->vie=v;

}

void AFFICHEUR_OPTION::change_vitesse(int v)
{
    if(type_edition==PNJ)
        curent_PNJ->vitesse=v;
    else if (type_edition==OBJ && (curent_OBJ->type==TRAMPOLINE || curent_OBJ->type==ACCELERATEUR))
        curent_OBJ->destx=v;
}

void AFFICHEUR_OPTION::change_lvl(int i)
{
    if(type_edition==PNJ)
    {
        curent_PNJ->lvl=i;
        //change degats, vitesse, vie, exp
        vision->setValue(i+6.0+(i/2.0)*multiplicateur*1.5);
        vie->setValue(23.11*pow(i,1.4)*(multiplicateur/1.5));
        vitesse->setValue(149+(i*multiplicateur));
        degats->setValue((4+6*i*(multiplicateur/1.5)));
        exp->setValue((i*5)*multiplicateur);
    }
};

void AFFICHEUR_OPTION::change_exp(int i)
{
    if(type_edition==PNJ)
        curent_PNJ->exp=i;
    else if (type_edition==OBJ && curent_OBJ->type==OBJ_QUETTE)
        curent_OBJ->desty=i;
};

void AFFICHEUR_OPTION::change_resize(double d)
{
    if(type_edition==PNJ)
        curent_PNJ->resize=d*10;
    else if (type_edition==OBJ)
        curent_OBJ->resize=d*10;
};


void AFFICHEUR_OPTION::change_dialogue()
{
    QString s=dialogue_tape->toPlainText();

    if(type_edition==PNJ)
        strncpy(curent_PNJ->dialogue,s.toAscii().data(),1000);
    else if(type_edition==OBJ)
        strncpy(curent_OBJ->dialogue,s.toAscii().data(),1000);
};

void AFFICHEUR_OPTION::change_dialogue_skill(int s)
{
    if(type_edition==OBJ)
        curent_OBJ->idSkill=s;
};

void AFFICHEUR_OPTION::afficher_boutons_obj()
{
    page1->setEnabled(true);

    if (type_edition==OBJ)
    {
        lvl->setEnabled(false);
        boss->setEnabled(false);
        lvl_l->setEnabled(false);
        boss->setEnabled(false);
        element->setEnabled(false);
        type_objet->setEnabled(true);
        volant->setEnabled(false);
        voleur->setEnabled(false);
        eparpieur->setEnabled(false);
        degats->setEnabled(curent_OBJ->type ==PIEGE);
        looter->setEnabled(false);
        vision->setEnabled(false);
        agressif->setEnabled(false);
        vie->setEnabled(false);

        dep_max->setEnabled(false);
        dep_min->setEnabled(false);
        vitesse->setEnabled(curent_OBJ->type == TRAMPOLINE or curent_OBJ->type ==ACCELERATEUR);
        _or->setEnabled(curent_OBJ->type==OR or curent_OBJ->type==OBJ_QUETTE);
        dialogue_tape->setEnabled(curent_OBJ->type==PARCHEMIN or curent_OBJ->type==OBJ_QUETTE or curent_OBJ->type==RIEN);
        quette->setEnabled(curent_OBJ->type==PARCHEMIN or curent_OBJ->type==OBJ_QUETTE or curent_OBJ->type == TELEPORTEUR_QUETTE);


        dialogue_skill->setEnabled(curent_OBJ->type == PARCHEMIN);
        exp->setEnabled((curent_OBJ->type== PARCHEMIN || curent_OBJ->type == OBJ_QUETTE));

        Choix_TP->setEnabled(curent_OBJ->type==TELEPORTEUR or curent_OBJ->type == TELEPORTEUR_QUETTE);

        taille->setValue(curent_OBJ->resize/10);

        direction->setCurrentIndex((int)curent_OBJ->direction);

        quette->setValue(curent_OBJ->quette or curent_OBJ->type==TELEPORTEUR_QUETTE or curent_OBJ->type==OBJ_QUETTE or curent_OBJ->type==PARCHEMIN);
        dialogue_tape->setPlainText(curent_OBJ->dialogue);

        if (curent_OBJ->type == ACCELERATEUR || curent_OBJ->type== TRAMPOLINE ) vitesse->setValue(curent_OBJ->destx); else vitesse->setValue(0);
        if (curent_OBJ->type== OR) _or->setValue(curent_OBJ->destx); else _or->setValue(0);
        if (curent_OBJ->type == OBJ_QUETTE || curent_OBJ->type == PARCHEMIN) exp->setValue(curent_OBJ->desty); else exp->setValue(0);
        if (curent_OBJ->type ==PIEGE)    degats->setValue(curent_OBJ->destx); else degats->setValue(0);


        dialogue_skill->setValue(curent_OBJ->idSkill);
    }
    else if(type_edition==PNJ)
    {
        direction->setCurrentIndex((int)curent_PNJ->direction);
    }

};


void AFFICHEUR_OPTION::afficher_boutons_pnj()
{
    afficher_boutons_obj();

    lvl->setEnabled(true);
    lvl_l->setEnabled(true);
    boss->setEnabled(true);
    element->setEnabled(true);
    quette->setEnabled(true);
    element->setEnabled(true);
    type_objet->setEnabled(false);
    dialogue_skill->setEnabled(false);
    volant->setEnabled(true);
    voleur->setEnabled(true);
    eparpieur->setEnabled(true);
    looter->setEnabled(true);
    Choix_TP->setEnabled(false);
    vision->setEnabled(true);
    agressif->setEnabled(true);
    vie->setEnabled(true);
    dep_max->setEnabled(true);
    dep_min->setEnabled(true);
    vitesse->setEnabled(true);
    _or->setEnabled(false);
    dialogue_tape->setEnabled(true);
    degats->setEnabled(true);
    exp->setEnabled(true);


    //initialisation des variables
    vision->setValue (curent_PNJ->vue);
    dep_min->setValue (curent_PNJ->min);
    dep_max->setValue (curent_PNJ->max);
    degats->setValue (curent_PNJ->degats);
    vie->setValue(curent_PNJ->vie);
    vitesse->setValue(curent_PNJ->vitesse);
    exp->setValue(curent_PNJ->exp);
    taille->setValue(curent_PNJ->resize/10);
    element->setCurrentIndex(curent_PNJ->element);
    quette->setValue(curent_PNJ->quette);


    dialogue_tape->setPlainText(curent_PNJ->dialogue);

    element->setCurrentIndex(curent_PNJ->element);

    agressif->setCheckState((curent_PNJ->agressif)? Qt::Checked:Qt::Unchecked);
    volant->setCheckState((curent_PNJ->volant)? Qt::Checked:Qt::Unchecked);
    voleur->setCheckState((curent_PNJ->voleur)? Qt::Checked:Qt::Unchecked);
    looter->setCheckState((curent_PNJ->looter)? Qt::Checked:Qt::Unchecked);
    eparpieur->setCheckState((curent_PNJ->eparpieur)? Qt::Checked:Qt::Unchecked);

    if(curent_PNJ->boss)
    {
        boss->setCheckState(Qt::Checked);
        multiplicateur=2;
        quette->setEnabled(true);

    }
    else
     {
        boss->setCheckState(Qt::Unchecked);
        multiplicateur=1;
        quette->setEnabled(false);
     }
};


void AFFICHEUR_OPTION::change_element(int i)
{
    if(type_edition==PNJ)
    curent_PNJ->element=(_type_element) i;
};

void AFFICHEUR_OPTION::change_quette(int i)
{

    if(type_edition==PNJ)
        curent_PNJ->quette=i;

    else if(type_edition==OBJ && (curent_OBJ->type==PARCHEMIN || curent_OBJ->type==OBJ_QUETTE))
        curent_OBJ->quette=i;
};

void AFFICHEUR_OPTION::change_type_obj(int t)
{
  if (type_edition==OBJ)
        curent_OBJ->type=(_type_objets)t;

    switch (curent_OBJ->type)
    {
        case OR :
            _or->setEnabled(true);
            break;
        case OBJ_QUETTE:
            quette->setValue(1);
            _or->setEnabled(false);
            break;
        default :
            _or->setEnabled(false);
            quette->setValue(0);
            break;
    }

    if (curent_OBJ->type <=PIEGE)
    {
       change_direction(t);
       // sprite_courant.SetSubRect(subRect);
        strcpy(curent_OBJ->dossier,"./resources/image");
        strcpy(curent_OBJ->nom,"allItems.png");
    }
};
