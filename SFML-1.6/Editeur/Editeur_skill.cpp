#include "Editeur_skill.hpp"

#include <sstream>
#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#include <string.h>



using namespace std;

void Editeur_Skill::init()
{
        resize(fenettre_taille_x,fenettre_taille_y);
        this->setWindowTitle("Editeur Skills");

        Scroll=new QScrollArea();
        VLayout=new QVBoxLayout();
        layoutScroll=new QGridLayout();
        conteneur=new QWidget;

        Add=new QPushButton("Ajouter 1 Skill");
        QObject::connect(Add,SIGNAL(clicked()),this,SLOT(addLigne()));

        Save=new QPushButton("Enregistrer");
        QObject::connect(Save,SIGNAL(clicked()),this,SLOT(ecrireFichier()));


        entete[0]=new QLabel("ID");
        entete[1]=new QLabel("Nom");
        entete[2]=new QLabel("Commentaire");
        entete[3]=new QLabel("Icone");
        entete[4]=new QLabel("Duree");
        entete[5]=new QLabel("Recharge");
        entete[6]=new QLabel("Mana");

        for (int i=0;i<7;i++)
            layoutScroll->addWidget(entete[i],0,i);

        conteneur->resize(fenettre_taille_x-20,40);
        conteneur->setLayout(layoutScroll);
        Scroll->setWidget(conteneur);
        VLayout->addWidget(Scroll);
        VLayout->addWidget(Add);
        VLayout->addWidget(Save);


        this->setLayout(VLayout);

        conteneur->show();

        chargerFichier();
         hide();

};

void Editeur_Skill::addLigne()
{
    stringstream out;
    unsigned int i=tab_ID.size();
    out<<(i +1);

    tab_ID.push_back(new QLabel(QString(out.str().c_str())));
    tab_description.push_back(new QTextEdit());
    tab_Nom.push_back(new QLineEdit());
    tab_Icon.push_back(new QComboBox());
    tab_duree.push_back(new QSpinBox());
    tab_mana.push_back(new QSpinBox());

    tab_duree[i]->setRange(0,3600);
    tab_recharge.push_back(new QSpinBox());
    tab_recharge[i]->setRange(0,3600);
    tab_mana[i]->setRange(0,3600);


    vector <string> tab_Icon_nom;

    tab_Icon_nom=Listeur.lister("resources/image/icones/");

    out.str(string());
    out<<"resources/image/icones/";

    for(unsigned int j=0; j<tab_Icon_nom.size();j++)
    {
            tab_Icon[i]->addItem(string(out.str()+tab_Icon_nom[j]).c_str());
    }
    unsigned int j=0;
    layoutScroll->addWidget(tab_ID[i],i+1,j);j++;
    layoutScroll->addWidget(tab_Nom[i],i+1,j);j++;
    layoutScroll->addWidget(tab_description[i],i+1,j);j++;
    layoutScroll->addWidget(tab_Icon[i],i+1,j);j++;
    layoutScroll->addWidget(tab_duree[i],i+1,j);j++;
    layoutScroll->addWidget(tab_recharge[i],i+1,j);j++;
    layoutScroll->addWidget(tab_mana[i],i+1,j);
    conteneur->resize(conteneur->size().width(),conteneur->size().height()+100);
};

void Editeur_Skill::ecrireFichier()
{
    /* ajouter la cretion de l'enumeration automatique du nom des skill en fonction de leur ID*/
    FILE* Fichier=NULL;       //fichier contenant les caractéristiques des skills
    FILE* FichierHpp=NULL;  //fichier contenant l'enumeration des skills pour le jeu (leur ID)
    bool existe=false;

    string str="./lvl/skills";
    Fichier=fopen(str.c_str(),"r"); //ouvertur du fichier en lecture

     if (Fichier != NULL)
    {

        int res=QMessageBox::critical(this, "Erreur", "Fichier deja existant, enregistrement impossible, forcer l'enregistrement?",QMessageBox::Yes | QMessageBox::No);

        if ( res ==QMessageBox::Yes )
            existe=false;
        else if (res== QMessageBox::No)
            existe =true;
        fclose(Fichier);
    }

    if (!existe) //ouvertur du fichier en ecriture
    {
         Fichier=fopen(str.c_str(),"w+");


         if (Fichier != NULL)   //on ecrit les données
         {
                FichierHpp=fopen(string(str+"_enum.hpp").c_str(),"w+");
                struct SKILLS skill_struct;
                for(unsigned int i=0;i<tab_ID.size();i++)   //parcourt de tous les skills ecrit
                {
                    //modifications des valeur contenue dans la structure
                    strncpy(skill_struct.description,tab_description[i]->toPlainText().toStdString().c_str(),999);
                    skill_struct.ID=tab_ID[i]->text().toInt();
                    strncpy(skill_struct.nom,tab_Nom[i]->text().toStdString().c_str(),49);
                    strncpy(skill_struct.img,tab_Icon[i]->currentText().toStdString().c_str(),99);
                    skill_struct.duree=tab_duree[i]->value();
                    skill_struct.recharge=tab_recharge[i]->value();
                    skill_struct.mana=tab_mana[i]->value();

                    fwrite(&skill_struct,sizeof(skill_struct),1,Fichier); //on ecrit dans le fichier
                    //on genere l'enumeration corespondante au skill

                    char skill_nom[50]="";
                    strncpy(skill_nom,skill_struct.nom,49);
                    for (int cpt=0;skill_nom[cpt]!='/0' and cpt < 50;cpt++ ) //on regarde si il y a un " " dans le nom, dans se cas on l'enleve
                    {
                        skill_nom[cpt]=toupper(skill_nom[cpt]);
                        if (skill_nom[cpt]==' ')
                            skill_nom[cpt]='_';
                    }
                    if (i==0)
                         fprintf(FichierHpp,"enum SKILLS{SKILL_%s=0",skill_nom);
                    else
                        fprintf(FichierHpp,",SKILL_%s",skill_nom);
                }
                fprintf(FichierHpp,",TAILLE_SKILLS};");
                fclose(FichierHpp);
                fclose(Fichier);
         }
    }
};

void Editeur_Skill::chargerFichier()
{
    FILE* Fichier=NULL;
    string str="./lvl/skills";

    Fichier=fopen(str.c_str(),"r"); //ouvertur du fichier en lecture

     if (Fichier != NULL)   //on ecrit les données
     {
        tab_description.clear();
        tab_Icon.clear();
        tab_ID.clear();
        tab_Nom.clear();

        struct SKILLS skill_struct;
        fread(&skill_struct,sizeof(skill_struct),1,Fichier);

        while(!feof(Fichier))   //tant qu'il y a des skills
        {
            addLigne(); //on cree une  nouvelle ligne
            unsigned int i=tab_description.size() -1;

            //on met à jour la ligne qui vien d'etre créé
            tab_description[i]->insertPlainText(skill_struct.description);
            stringstream out;
            out<<skill_struct.ID;
            tab_ID[i]->setText(out.str().c_str());
            tab_Icon[i]->setCurrentIndex(tab_Icon[i]->findText(skill_struct.img));
            tab_Nom[i]->setText(skill_struct.nom);
            tab_duree[i]->setValue(skill_struct.duree);
            tab_recharge[i]->setValue(skill_struct.recharge);
            tab_mana[i]->setValue(skill_struct.mana);

            fread(&skill_struct,sizeof(skill_struct),1,Fichier);    //on lit le prochain skill
        }
         fclose(Fichier);
    }


};
