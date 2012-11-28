#include "Editeur_Quette.h"
#include "stdio.h"
#include "stdlib.h"
#include <sstream>

using namespace std;

void Editeur_Quette::init()
{
    resize(fenettre_taille_x,fenettre_taille_y);
    this->setWindowTitle("Editeur de Quette");

    scroll=new QScrollArea();
    conteneur=new QWidget();
    layoutScroll=new QGridLayout();
    layout=new QGridLayout();
    Reload=new QPushButton("Reload");
    QObject::connect(Reload,SIGNAL(clicked()),this,SLOT(reload()));

    ChoixTypeEdit= new QComboBox();
    ChoixTypeEdit->addItem("Objets");
    ChoixTypeEdit->addItem("Boss");
    QObject::connect(ChoixTypeEdit,SIGNAL(currentIndexChanged(int)),this,SLOT(Change_type_Edit(int)));


    int i=0;
    entete[i]=new QLabel("Type");i++;
    entete[i]=new QLabel("Dialogue");i++;
    entete[i]=new QLabel("Num Quette");i++;
    entete[i]=new QLabel("Nom Skill");i++;

    for (int i=0;i<4;i++)
            layoutScroll->addWidget(entete[i],0,i);

    conteneur->resize(fenettre_taille_x,40);
    conteneur->setLayout(layoutScroll);
    scroll->setWidget(conteneur);
    layout->addWidget(scroll,0,0,1,5);
    layout->addWidget(Reload,1,0,1,4);
    layout->addWidget(ChoixTypeEdit,1,4,1,1);
    this->setLayout(layout);
    TYPEEDITION=EDIT_OBJETS;

    conteneur->show();
};
void Editeur_Quette::reload()
{
    if (TYPEEDITION==EDIT_OBJETS) reload_OBJ();
    else if(TYPEEDITION==EDIT_PNJ) reload_PNJ();
    else cerr<<"Erreur dans le choix d'edition de l'editeur de quette"<<endl;
};
void Editeur_Quette::reload_PNJ()
{
    /*********************************
    remise à zero
    **********************************/
     delete layoutScroll;
    layoutScroll=new QGridLayout();

    for (unsigned int i=0;i<tab_type.size();i++)
        delete tab_type[i];
    tab_type.clear();

    for (unsigned int i=0;i<tab_dialogue.size();i++)
        delete tab_dialogue[i];
    tab_dialogue.clear();

    for (unsigned int i=0;i<tab_lvl.size();i++)
        delete tab_lvl[i];
    tab_lvl.clear();

    for (unsigned int i=0;i<tab_id_skill.size();i++)
        delete tab_id_skill[i];
    tab_id_skill.clear();

    for (unsigned int i=0;i<tab_var_quette.size();i++)
        delete tab_var_quette[i];
    tab_var_quette.clear();

    int ligne=0;
    entete[3]->setText("Lvl");
    for (ligne=0;ligne<4;ligne++)
            layoutScroll->addWidget(entete[ligne],0,ligne);

    /******************************************
    on charge les elements
    ******************************************/

    const unsigned int _size=tab_PNJ_struct->size();
    string type;

    for (int i=0;i<_size;i++)       //pour tous les PNJ
    {
            type="PNJ ";
            type+=((*tab_PNJ_struct)[i].boss)? "Boss":"Normal";

            tab_type.push_back(new QLabel(type.c_str()));
            tab_dialogue.push_back(new QTextEditIndex(i,(*tab_PNJ_struct)[i].dialogue));
            tab_var_quette.push_back(new QSpinBoxIndex(i,(*tab_PNJ_struct)[i].quette));
            tab_lvl.push_back(new QSpinBoxIndex(i,(*tab_PNJ_struct)[i].lvl));

            if ((*tab_PNJ_struct)[i].quette >0) //alors on l'ajoute
            {
                ligne++;

                layoutScroll->addWidget(tab_type[i],ligne,0);

                QObject::connect(tab_dialogue[i],SIGNAL(EmitIndex(int)),this,SLOT(Changer_dialogue_pnj(int)));
                layoutScroll->addWidget(tab_dialogue[i],ligne,1);

                QObject::connect(tab_var_quette[i],SIGNAL(EmitIndex(int)),this,SLOT(Changer_quette_pnj(int)));
                layoutScroll->addWidget(tab_var_quette[i],ligne,2);

                QObject::connect(tab_lvl[i],SIGNAL(EmitIndex(int)),this,SLOT(Changer_lvl_pnj(int)));
                layoutScroll->addWidget(tab_lvl[i],ligne,3);

            }

    }


    /******************************************
    on remet le layout corespondant
    *******************************************/
    conteneur->setLayout(layoutScroll);
    conteneur->resize(fenettre_taille_x-20,20*(ligne+1));
};

void Editeur_Quette::reload_OBJ()
{
    /*********************************
    remise à zero
    **********************************/
    delete layoutScroll;
    layoutScroll=new QGridLayout();

    for (unsigned int i=0;i<tab_type.size();i++)
        delete tab_type[i];
    tab_type.clear();

    for (unsigned int i=0;i<tab_dialogue.size();i++)
        delete tab_dialogue[i];
    tab_dialogue.clear();

    for (unsigned int i=0;i<tab_id_skill.size();i++)
        delete tab_id_skill[i];
    tab_id_skill.clear();

    for (unsigned int i=0;i<tab_lvl.size();i++)
        delete tab_lvl[i];
    tab_lvl.clear();

    for (unsigned int i=0;i<tab_var_quette.size();i++)
        delete tab_var_quette[i];
    tab_var_quette.clear();


    int ligne=0;
    entete[3]->setText("Nom Skill");
    for (ligne=0;ligne<4;ligne++)
            layoutScroll->addWidget(entete[ligne],0,ligne);
     //stringstream out;

    /******************************************
    on charge les elements
    ******************************************/

    //lecture de tous les skill
    vector<string> tab_nom_skill;       //nom des skills trié par ID
    FILE* skill_fic=NULL;
    struct SKILLS skill;

    skill_fic=fopen("./lvl/skills","rb");

    if (skill_fic != NULL)  //lecture des skills actuels
    {
        fread(&skill,sizeof(skill),1,skill_fic);
        while(!feof(skill_fic))
        {
            tab_nom_skill.push_back(skill.nom); //on ajoute le nom du skill dans le tableau
            fread(&skill,sizeof(skill),1,skill_fic);
        }
        fclose(skill_fic);
    }

    unsigned int _size2=tab_nom_skill.size();
    unsigned int _size=tab_OBJ_struct->size();
    string type;

    for(unsigned int i=0;i<_size;i++)      //pour tous les OBJ on ajoute une ligne
     {
            type="OBJET ";
            _type_objets _type=(*tab_OBJ_struct)[i].type;

            switch (_type)
            {

                case PARCHEMIN:type+="Parchemin";
                break;

                case OBJ_QUETTE: type+="de Quette";
                break;

                case RIEN:type+="Rien";
                break;

                case TELEPORTEUR_QUETTE: type+="Telep.Quette";
                break;

                default:type+="Autre";
                break;
            }

             tab_type.push_back(new QLabel(type.c_str()));

             tab_dialogue.push_back(new QTextEditIndex(i,(*tab_OBJ_struct)[i].dialogue));

             tab_var_quette.push_back(new QSpinBoxIndex(i,(*tab_OBJ_struct)[i].quette));

             tab_id_skill.push_back(new QComboBoxIndex(i));
             int idskill=(*tab_OBJ_struct)[i].idSkill;

            if (_type== PARCHEMIN || _type == RIEN || _type == OBJ_QUETTE)
            {
                ligne++;
                layoutScroll->addWidget(tab_type[i],ligne,0);

                QObject::connect(tab_dialogue[i],SIGNAL(EmitIndex(int)),this,SLOT(Changer_dialogue_objet(int)));
                layoutScroll->addWidget(tab_dialogue[i],ligne,1);

                if (_type != RIEN)
                {
                    QObject::connect(tab_var_quette[i],SIGNAL(EmitIndex(int)),this,SLOT(Changer_quette_objet(int)));
                    layoutScroll->addWidget(tab_var_quette[i],ligne,2);
                }

                if(_type== PARCHEMIN)
                {
                    QObject::connect(tab_id_skill[i],SIGNAL(EmitIndex(int)),this,SLOT(Changer_skill_objet(int)));
                    layoutScroll->addWidget(tab_id_skill[i],ligne,3);

                    tab_id_skill[i]->addItem("Aucun");
                    for(unsigned int j=0;j<_size2;j++) //ajouts des nom de skill dans la combo box
                        tab_id_skill[i]->addItem(tab_nom_skill[j].c_str());
                }

                tab_id_skill[i]->setCurrentIndex(idskill);
            }
            else if (_type==TELEPORTEUR_QUETTE)
            {
                 ligne++;
                 layoutScroll->addWidget(tab_type[i],ligne,0);

                 QObject::connect(tab_var_quette[i],SIGNAL(EmitIndex(int)),this,SLOT(Changer_quette_objet(int)));
                 layoutScroll->addWidget(tab_var_quette[i],ligne,2);

            }
     }

    conteneur->setLayout(layoutScroll);
    conteneur->resize(fenettre_taille_x-20,50*(ligne+1));
};


void Editeur_Quette::Changer_dialogue_objet(int id)
{
    strncpy((*tab_OBJ_struct)[id].dialogue,tab_dialogue[id]->toPlainText().toStdString().c_str(),TAILLE_DIALOGUE_PNJ-1);
};


void Editeur_Quette::Changer_quette_objet(int id)
{
  (*tab_OBJ_struct)[id].quette= tab_var_quette[id]->value();
};

void Editeur_Quette::Changer_skill_objet(int id)
{
  (*tab_OBJ_struct)[id].idSkill=tab_id_skill[id]->currentIndex();
};

void Editeur_Quette::Changer_lvl_pnj(int id)
{
    (*tab_PNJ_struct)[id].lvl=tab_lvl[id]->value();
};

void Editeur_Quette::Changer_dialogue_pnj(int id)
{
    strncpy((*tab_PNJ_struct)[id].dialogue,tab_dialogue[id]->toPlainText().toStdString().c_str(),TAILLE_DIALOGUE_OBJ-1);
};

void Editeur_Quette::Changer_quette_pnj(int id)
{
    (*tab_PNJ_struct)[id].quette= tab_var_quette[id]->value();
};

void Editeur_Quette::Change_type_Edit(int t)
{
    TYPEEDITION=(_TYPEEDIT)t;
};

