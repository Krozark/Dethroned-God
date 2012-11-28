#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include "editeur_MAP.hpp"

#include <SFML/Network.hpp>



//#define DEBUGING

using namespace std;
using namespace sf;

void EDITEUR_MAP::OnInit()
{
    Set_type_edition(MAP);
}

void EDITEUR_MAP::OnUpdate()
{
    this->Clear(Color(150,150,150));
    affiche->Aide.Draw(this);

    switch (type_edition)
        {
            case MAP:
            {
                PNJFrame->hide();
                OBJFrame->hide();
                afficher_resources_images();
                B_Maps->show();

            }break;
            case PNJ:
            {
                OBJFrame->hide();
                PNJFrame->show();
                B_Maps->hide();
                if (PNJEditeur->Get_curent()!=PNJEditeur->Get_passed())
                {
                    //initialisation du PNJ
                    _PNJ PNJ;

                    initPNJ(&PNJ);

                    affiche->Set_PNJ(PNJ,&PNJEditeur->resources_image[PNJEditeur->Get_curent()]);

                }
            }break;
            case OBJ:
            {
                 OBJFrame->show();
                 PNJFrame->hide();
                 B_Maps->hide();
                 if (OBJEditeur->Get_curent()!=OBJEditeur->Get_passed())
                 {
                     _OBJ OBJ;

                    strncpy(OBJ.nom,OBJEditeur->resources_image_nom[OBJEditeur->Get_curent()].c_str(),49);
                    strncpy(OBJ.dossier,OBJEditeur->Get_dossier().c_str(),49);
                    OBJ.direction=(_type_direction) OBJEditeur->Get_ligne();
                    OBJ.tx=OBJEditeur->Get_x();
                    OBJ.ty=OBJEditeur->Get_y();
                    OBJ.x=5;
                    OBJ.y=5;
                    OBJ.idSkill=0;

                    affiche->Set_OBJ(OBJ,&OBJEditeur->resources_image[OBJEditeur->Get_curent()]);
                 }
            }break;
            default:break;
        }

        affiche->setPointOrigineVue(Vector2i(F_Edition->horizontalScrollBar()->value(),F_Edition->verticalScrollBar()->value()));
    Draw(Erreur);

    if (bougerFenettre)
    {
        Vector2f diff=Vector2f((clickPush.x-clickRelase.x)/1.5+F_Edition->horizontalScrollBar()->value(),(clickPush.y-clickRelase.y)/1.5+F_Edition->verticalScrollBar()->value());
        clickPush=clickRelase;
        F_Edition->horizontalScrollBar()->setSliderPosition((diff.x));
        F_Edition->verticalScrollBar()->setSliderPosition((diff.y));
    }


    //Display();
}


void EDITEUR_MAP::init()
 {
    SetFramerateLimit(60); //eviter de prendre trop de rousources

    affiche=new AFFICHEUR_GRILLE(this);
    F_Edition=new QScrollArea(this); //fenettre assensseure pour la carte
    F_Edition->move(taille_objets_pix*4,taille_objets_pix);
    F_Edition->setWidget(affiche);
    F_Edition->resize(fenettre_taille_x-(taille_objets_pix*5),fenettre_taille_y-taille_objets_pix*2);

    //bouton selectioneur de dossiers
    B_Maps=new QComboBox(this);
    QObject::connect(B_Maps,SIGNAL(currentIndexChanged ( const QString)),this,SLOT(Set_map(const QString)));

    tx=taille_objets_pix*20;
    ty=taille_objets_pix*30;

    PNJFrame=new QFrame;
    PNJFrame->setWindowTitle("Editeur Dethroned God,Edition PNJ");
    PNJFrame->resize(fenettre_taille_x-2*taille_objets_pix,fenettre_taille_y-2*taille_objets_pix);

    PNJEditeur=new EDITEUR_FENETTRE("./resources/PNJ",4,4,PNJFrame,fenettre_taille_x-2*taille_objets_pix,fenettre_taille_y-2*taille_objets_pix);
    PNJEditeur->Set_fenettre_principale(this);
    QObject::connect(PNJEditeur,SIGNAL(LvlChanged(int)),this,SLOT(SetLvl(int)));

    OBJFrame=new QFrame;
    OBJFrame->setWindowTitle("Editeur Dethroned God,Edition OBJ");
    OBJFrame->resize(tx,ty);

    OBJEditeur=new EDITEUR_FENETTRE("./resources/OBJ",4,4,OBJFrame,tx,ty);
    OBJEditeur->Set_fenettre_principale(this);
    QObject::connect(OBJEditeur,SIGNAL(TypeChanged(int)),this,SLOT(SetType(int)));


    PNJFrame->hide();
    OBJFrame->hide();

    maj_resources();
    charger_resources_map();

    I_grille.LoadFromFile("./resources/image/32_32_2.png");
    S_grille.SetImage(I_grille);
    grille_map.push_back(S_grille);



    Enregistrer=new QPushButton("Enregistrer map",this);
    Enregistrer->move(120,0);
    QObject::connect(Enregistrer,SIGNAL(clicked()),this,SLOT(ecrire_fichier()));

    Reset=new QPushButton("Reset",this);
    Reset->move(fenettre_taille_x-200,0);
    QObject::connect(Reset,SIGNAL(clicked()),this,SLOT(reset()));

    NomFichier=new QLineEdit(this);
    NomFichier->move(250,0);

    Load=new QPushButton("Load",this);
    Load->move(400,0);
    QObject::connect(Load, SIGNAL(clicked()), this, SLOT(Set_map_load()));

    edition_auto=new QCheckBox("Edition Auto",this);
    edition_auto->move(650,4);
    QObject::connect(edition_auto,SIGNAL(toggled(bool)),this,SLOT(ChangeAutotEnabled(bool)));


    B_Choix=new QComboBox(this);
    B_Choix->move(500,0);
    B_Choix->addItem("Edition MAP"); //0
    B_Choix->addItem("Edition PNJ"); //1
    B_Choix->addItem("Edition OBJ"); //2
    B_Choix->addItem("Edition HERO"); //3
    B_Choix->addItem("Edition OPTIONS"); //4
    B_Choix->addItem("Edition SKILLS"); // 5
    B_Choix->addItem("Edition QUETTE");//6
    B_Choix->addItem("Edition Sons");   //7
    QObject::connect(B_Choix,SIGNAL(currentIndexChanged (int)),this,SLOT(Set_type_edition(int)));

    curent_selection_image.LoadFromFile("./resources/image/selection.png");
    curent_selection.SetImage(curent_selection_image);
    curent_selection.SetPosition(0,taille_objets_pix);

    affiche->Set_sprite_courant(resources_sprite_map[0]);
    affiche->setClickSouris(&clickPush,&clickRelase);

    QObject::connect(affiche,SIGNAL(Clicked()),this,SLOT( BougerFenettreCliked ()));
    QObject::connect(affiche,SIGNAL(Relased()),this,SLOT(BougerFenettreRelase()));

    Erreur.SetPosition(800,0);
    Erreur.SetSize(19);
    Erreur.SetColor(Color(0,0,0));

    B_Quit=new QPushButton("Quitter?",this);
    QObject::connect(B_Quit, SIGNAL(clicked()), this, SLOT(quitter()));
    B_Quit->move(fenettre_taille_x-100,0);

    Site= new QPushButton("Envoyer au site",this);
    QObject::connect(Site,SIGNAL(clicked()),this,SLOT(envoie_site()));
    Site->move(fenettre_taille_x-150,fenettre_taille_y-taille_objets_pix+5);
 }

 void EDITEUR_MAP::envoie_site()
 {
    int res=QMessageBox::critical(this, "Verification", "Voulez vous vraiment envoyer les informations au site?",QMessageBox::Yes | QMessageBox::No);
    if ( res ==QMessageBox::Yes )
    {
        options_map map=affiche->getMapOption();
        string donnes="Voulez vous vraiment envoyer les informations suivantes au site:\n\nID: <"+NomFichier->text().toStdString()+">\nNom: <"+string(map.nomMap)+">\nType: <"+string(map.Type)+">\nMythologie: <"+string(map.mythologie)+">";

        res=QMessageBox::critical(this, "Verification",donnes.c_str(),QMessageBox::Yes | QMessageBox::No);

         if ( res ==QMessageBox::Yes )
         {
             sf::Http http("http://dethronedgod.hebergratuit.com");

            sf::Http::Request request;
            request.SetURI("editeur_reception.php");
            request.SetMethod(sf::Http::Request::Post);

            string Body="ID="+NomFichier->text().toStdString()+"&Nom="+string(map.nomMap)+"&Type="+string(map.Type)+"&Mythologie="+string(map.mythologie);
            request.SetBody(Body);

            sf::Http::Response response = http.SendRequest(request);
            QMessageBox::critical(this, "Réception",response.GetBody().c_str(),QMessageBox::Yes | QMessageBox::No);
            res=QMessageBox::critical(this, "Verification", "Voulez vous envoyer le niveau au site?",QMessageBox::Yes | QMessageBox::No);

            if (res == QMessageBox::Yes)
            {
                sf::Ftp Server;
                if (Server.Connect("ftp.hebergratuit.com",21,3).IsOk())
                {
                    // Ok, nous sommes connectés
                    if (Server.Login("rober_7364480", "yoyoperson").IsOk())
                    {
                        // Ok, nous sommes identifiés
                        string str="./lvl/";
                        str+=string(NomFichier->text().toAscii().data());
                        Server.Upload(str, "/htdocs/DL/lvl", sf::Ftp::Binary);
                        QMessageBox::critical(this, "Réception", "Fichier bien envoye");

                    }else
                        QMessageBox::critical(this, "Erreur", "Impossible de s'identifier au serveur");
                }else
                    QMessageBox::critical(this, "Erreur", "Impossible de se connecter au serveur");
                Server.Disconnect();

            }

         }
    }
 };

void EDITEUR_MAP::quitter()
{
    int res=QMessageBox::critical(this, "Verification", "Voulez vous vraiment quitter?",QMessageBox::Yes | QMessageBox::No);

    if ( res ==QMessageBox::Yes )
        qApp->quit();
};

 void EDITEUR_MAP::reset()
 {
    int res=QMessageBox::critical(this, "Verification", "Voulez vous vraiment reset la map?",QMessageBox::Yes | QMessageBox::No);

    if ( res ==QMessageBox::Yes )
        affiche->reset();
 }

void EDITEUR_MAP::ChangeAutotEnabled(bool a)
{
    AutoIs=a;
    if (a)
        Erreur.SetText("Mode Edition auto Active");
    else
        Erreur.SetText(("Mode Edition auto Desactivee"));
    if (VerifAuto())
        affiche->SetAutoImage(&resources_image_map[0]);
}


void EDITEUR_MAP::mousePressEvent ( QMouseEvent * event )
{
    int x=(event->pos().x()/taille_objets_pix);
    int y=((event->pos().y()-taille_objets_pix)/taille_objets_pix);


    if (type_edition==MAP) //si c'est en edition de map
    {
        if (x<4)
        {
            unsigned int c=y*4+x;

            if (c<resources_sprite_map.size())
            {
                curent_selection.SetPosition(x*taille_objets_pix,taille_objets_pix+y*taille_objets_pix);
                affiche->Set_sprite_courant(resources_sprite_map[c]);
            }
        }
    }
};


void EDITEUR_MAP::maj_resources()
{
    tab_nom_map=resources.lister("./resources","image","PNJ","OBJ");
    B_Maps->clear();

 for (unsigned int i=0;i<tab_nom_map.size();i++)
     B_Maps->addItem(tab_nom_map[i].c_str());

}

void EDITEUR_MAP::Set_map(const QString m)
 {
     curent_map=string("./resources/")+string(m.toAscii().data());  //definition du chemin de la carte selectionnée
     resources_image_map_nom.clear();
     resources_image_map_nom.push_back("Auto.png");

     vector<string> nom=resources.lister(curent_map,"Auto.png"); //listage de toute les carte, et mise place des noms dans le tableau corespondant
     for (unsigned int i=0;i<nom.size();i++)
        resources_image_map_nom.push_back(nom[i]);

    charger_resources_map();
 }

 void EDITEUR_MAP::Set_map_load()
 {
     /***************************************
    *           STRUCTURE DU FICHIER:       *
    *                                       *
    *       -i nombre d'images pour sprite  *
    *       -i*img  chemin relatif du sprite*
    *                                       *
    *       -i nombre de sprite             *
    *       -i*spr  sprite avec "pointer"   *
    *               sur le tableau d'image  *
    *               precedent et coodonées  *
    *                                       *
    *       -i nombre d'images pour les PNJ *
    *       -i*img chemin relatif des images*
    *                                       *
    *       -i nombre de PNJ                *
    *       -i _PNJ_    PNJ avec pointer sur*
    *                   le tableau d'images *
    *                   precedent           *
    *                                       *
    *       -i nombre d'images pour les OBJ *
    *       -i img  chemin relatif des image*
    *                                       *
    *       -i nombre d'OBJ                 *
    *       -i _OBJ_ OBJ avec pointer sur le*
    *                pointeur su le tableau *
    *                d'image precedent      *
    *                                       *
    *       -i nombre d'Hero                *
    *       -i _PNJ_ PNJ avec pointer sur le*
    *                pointeur su le tableau *
    *                d'image precedent      *
    *                                       *
    *       -i nombre de sons               *
    *       -i son   chemin des sons        *
    *                                       *
    *       -i nombre d'images              *
    *       -i img chemin de l'image relatif*
    *                                       *
    *       -option de la carte             *
    ****************************************/
     affiche->reset();
     string fichier=string("./lvl/")+string(NomFichier->text().toAscii().data());  //definition du chemin du lvl
     curent_map="lvl";

     if(fichier=="./lvl/")
     {
        Erreur.SetText("Nom de niveau vide");
     }
    else
    {
        FILE* Fichier;
        Fichier=fopen(fichier.c_str(),"rb"); //ouvertur du fichier en lecture

         if (Fichier != NULL)
        {
                 spr s; //srites
                 img img; //images
                 unsigned_int i; //nombre
                 _PNJ_ pnj;
                 _OBJ_ obj;

                fread(&i,sizeof(i),1,Fichier); //recuperation du nombre d'image dans le fichier
                vector<int> rien;
                resources_image_map_nom.clear();
                for (unsigned int j=0;j<i.i;j++) //recuperation du nom des images
                {
                     fread(&img,sizeof(img),1,Fichier);
                     if (string(img.nom)=="./resources/image/rien.png")
                        rien.push_back(j);
                     resources_image_map_nom.push_back(img.nom);

                }
               charger_resources_map();

                fread(&i,sizeof(i),1,Fichier); //recuperation du nombre de sprite dans le fichier
                Sprite sprite;
                for (unsigned int j=0;j<i.i;j++) //recuperation des sprites
                {

                     fread(&s,sizeof(s),1,Fichier);

                     if (s.img >= resources_image_map.size())
                     s.img=resources_image_map.size()-1;

                    bool X=false;
                    for (unsigned int a=0;a<rien.size() && !X;a++)
                        if (s.img == rien[a])
                            X=true;
                    if (X)
                        sprite.SetImage(affiche->image_mur);
                    else
                        sprite.SetImage(resources_image_map[s.img]);

                     sprite.SetPosition(s.x,s.y);
                     int n=(int)(s.y/taille_objets_pix)*taille_carte_case_x+(int)(s.x/taille_objets_pix);

                     grille_map.resize(taille_carte_case_x*taille_carte_case_y);
                     sprite.SetSubRect(IntRect(s.d,s.h,s.g,s.b));

                     grille_map[n]=sprite; //faire ici le push_back du sprite

                     if (s.colision)
                        affiche->grille_collision[n].SetImage(affiche->image_colision_no);
                    else
                        affiche->grille_collision[n].SetImage(affiche->Image_vide);
                }
                affiche->Set_grille(grille_map);


                /**********************************
                *       IMAGES DES PNJ ici inutle *
                **********************************/

                fread(&i,sizeof(i),1,Fichier);

                for (unsigned int j=0;j<i.i;j++)
                {
                    fread(&img,sizeof(img),1,Fichier);
                }

                /***************************
                *   recuperation des PNJ   *
                ***************************/
                fread(&i,sizeof(i),1,Fichier); //recuperation du nombre de sprite dans le fichier

                for (unsigned int j=0;j<i.i;j++) //recuperation des sprites
                {
                    fread(&pnj,sizeof(_PNJ_),1,Fichier);
                    int c=PNJEditeur->add_image(pnj.nom,pnj.dossier,pnj.ty,pnj.direction);
                    affiche->Set_PNJ(pnj,&PNJEditeur->resources_image[c]);
                    affiche->add_PNJ();
                }


                /**********************************
                *       IMAGES DES OBJ ici inutle *
                **********************************/

                fread(&i,sizeof(i),1,Fichier);

                for (unsigned int j=0;j<i.i;j++)
                {
                    fread(&img,sizeof(img),1,Fichier);
                }

                /********************************
                *       RECUPERATION DES OBJ    *
                ********************************/

                fread(&i,sizeof(i),1,Fichier); //recuperation du nombre de sprite dans le fichier
                for (unsigned int j=0;j<i.i;j++) //recuperation des sprites
                {
                    fread(&obj,sizeof(_OBJ_),1,Fichier);
                    int c=OBJEditeur->add_image(obj.nom,obj.dossier,obj.ty,obj.direction);
                    affiche->Set_OBJ(obj,&OBJEditeur->resources_image[c]);
                    affiche->add_OBJ();

                }

                /*******************************
                *   RECUPERATION DU HERO       *
                *******************************/
                fread(&i,sizeof(i),1,Fichier);

                fread(&affiche->curent_Heros,sizeof(heros),1,Fichier);
                affiche->Set_Heros_coord(affiche->curent_Heros.x,affiche->curent_Heros.y);

                /*******************************
                *       OPTION DE LA CARTE     *
                *******************************/

                options_map map;
                fread(&map,sizeof(options_map),1,Fichier);

                affiche->Set_Map(map);

                /*********************************
                *   Sons spacialisés             *
                *********************************/
                affiche->tab_SONS_struct.clear();
                fread(&i,sizeof(i),1,Fichier);

                if (feof(Fichier)>0)    //au cas ou il n'y a pas de sons
                    goto end;

                for (unsigned int j=0;j<i.i;j++)
                {
                    Sons_Spacialise son;
                    fread(&son,sizeof(Sons_Spacialise),1,Fichier);
                    affiche->tab_SONS_struct.push_back(son);
                }

                end:
                fclose(Fichier);
        }
        else
        {
            Erreur.SetText("Impossible d'ouvrir le fichier ");
        }
    }

 }

 void EDITEUR_MAP::charger_resources_map()
 {

    /*for (unsigned int i=0;i<resources_image_map_nom.size();i++)
    {
        for (unsigned int j=0;j<resources_image_map_nom.size();j++)
        {
            if (i != j)
                if (resources_image_map_nom[i]==resources_image_map_nom[j])
                {
                    resources_image_map_nom.erase(resources_image_map_nom.begin()+i);
                    i--;
                    j--;
                }

        }
    }
    for (unsigned int i=0;i<resources_image_map_nom.size();i++)
        cout<<resources_image_map_nom[i]<<endl;*/
    if (resources_image_map_nom.size()>=2)
    {
     if (resources_image_map_nom[resources_image_map_nom.size()-1]==resources_image_map_nom[resources_image_map_nom.size()-2]) //eviter les boublon d'image quand on charge une map
        resources_image_map_nom.resize(resources_image_map_nom.size()-1);
    }

     resources_image_map.clear();
     resources_image_map.resize(resources_image_map_nom.size()*2);
     resources_image_map.clear();
     resources_sprite_map.clear();


     for (unsigned int i=0;i<resources_image_map_nom.size();i++)
     {
        Image img;
        if (i==0 && curent_map!="lvl")
            AutoEnabled=(img.LoadFromFile(curent_map+"/Auto.png"))? true:false;
       else if (curent_map!="lvl")
            img.LoadFromFile(curent_map+"/"+resources_image_map_nom[i].c_str());
       else
            img.LoadFromFile(resources_image_map_nom[i].c_str());

        resources_image_map.push_back(img);
        if (i==0)
            affiche->SetAutoImage(&resources_image_map[0]);
        if (i>0)
        {
            Sprite spr(resources_image_map[i],Vector2f((i-1)%4*taille_objets_pix,taille_objets_pix+(i-1)/4*taille_objets_pix));
            resources_sprite_map.push_back(spr);
        }
     }

if (AutoEnabled)
     affiche->MajAuto();
 }

 void EDITEUR_MAP::afficher_resources_images()
 {
     for (unsigned int i=0;i<resources_sprite_map.size();i++)
     {
       Draw(resources_sprite_map[i]);
     }
     Draw(curent_selection);

 }

void EDITEUR_MAP::ecrire_fichier(bool deja_fait)
{

        /****************************************
        *           STRUCTURE DU FICHIER:       *
        *                                       *
        *       -i nombre d'images pour sprite  *
        *       -i*img  chemin relatif du sprite*
        *                                       *
        *       -i nombre de sprite             *
        *       -i*spr  sprite avec "pointer"   *
        *               sur le tableau d'image  *
        *               precedent et coodonées  *
        *                                       *
        *       -i nombre d'images pour les PNJ *
        *       -i*img chemin relatif des images*
        *                                       *
        *       -i nombre de PNJ                *
        *       -i _PNJ_    PNJ avec pointer sur*
        *                   le tableau d'images *
        *                   precedent           *
        *                                       *
        *       -i nombre d'images pour les OBJ *
        *       -i img  chemin relatif des image*
        *                                       *
        *       -i nombre d'OBJ                 *
        *       -i _OBJ_ OBJ avec pointer sur le*
        *                pointeur su le tableau *
        *                d'image precedent      *
        *                                       *
        *       -i nombre d'Hero                *
        *       -i _PNJ_ PNJ avec pointer sur le*
        *                pointeur su le tableau *
        *                d'image precedent      *
        *                                       *
        *                                       *
        *       -i =1                           *
        *       -option de la carte             *
        *       -i = nombre de sons spacialisés *
        *       -i SONS_SPACIALISE              *
        ****************************************/
         FILE* Fichier=NULL;
         bool existe=false;



         #ifdef DEBUGING
         FILE* Fichier_fr=NULL;
         #endif

         string str="./lvl/";
         str+=string(NomFichier->text().toAscii().data());


        if (str==string("./lvl/"))
        {
            Erreur.SetText("Nom de niveau vide");
        }
        else
        {
         Fichier=fopen(str.c_str(),"rb"); //ouvertur du fichier en lecture

         if (Fichier != NULL && !deja_fait)
            {

                int res=QMessageBox::critical(this, "Erreur", "Fichier deja existant, enregistrement impossible, forcer l'enregistrement?",QMessageBox::Yes | QMessageBox::No);

                if ( res ==QMessageBox::Yes )
                existe=false;
                else if (res== QMessageBox::No)
                existe =true;

            }
            if (Fichier != NULL)
            fclose(Fichier);

            if (!existe) //ouvertur du fichier en ecriture
            {
                 Fichier=fopen(str.c_str(),"wb+");
                 #ifdef DEBUGING
                 Fichier_fr=fopen(string(str+"_txt").c_str(),"wb+");
                 #endif

                 if (Fichier != NULL)
                 {
                     spr s; //srites
                     img im; //images
                     unsigned_int i; //nombre

                     /*******************************************
                     *          ECRITURE DES IMAGES SOURCES     *
                     *******************************************/

                     i.i=resources_image_map.size()+1;
                     fwrite(&i,sizeof(i),1,Fichier); //ecriture du nombre d'image presente dans le fichier
                     #ifdef DEBUGING
                     fprintf(Fichier_fr,"%u  <unsigned int nombre d'images differentes>\n",i.i);
                     #endif

                     //enregistrement des noms d'images sources

                     for (unsigned int j=0;j<resources_image_map.size()+1;j++) //pour toutes les images du tableau
                     {
                         string stri;
                         if (j==resources_image_map.size())
                            stri="./resources/image/rien.png";
                        else
                        {
                            if (curent_map!="lvl")
                                stri=curent_map+string("/")+resources_image_map_nom[j];
                            else
                                stri=resources_image_map_nom[j];
                        }

                        strncpy(im.nom,stri.c_str(),99); //mise en place du nom complet du chemin de l'image
                        fwrite(&im,sizeof(im),1,Fichier);      //ecriture dans le fichier
                        #ifdef DEBUGING
                        fprintf(Fichier_fr,"%s  <nom de l'image %d>\n",im.nom,j);
                        #endif
                     }


                     /*******************************************
                     *          ECRITURE DES SPRITE DE DECORS   *
                     *******************************************/

                     i.i=0;  //le nombre de srites
                     for (unsigned int j=0;j<affiche->grille.size();j++) //pour tous les sprites du tableau
                     {
                        if (affiche->grille[j].GetImage() != &(affiche->image_grille)) //verifier si ce n'est pas une grille
                            i.i++;
                     }

                     fwrite(&i,sizeof(i),1,Fichier); //ecriture du nombre de srite
                     #ifdef DEBUGING
                     fprintf(Fichier_fr,"\n%u  <unsigned int nombre de sprites differentes>\n",i.i);
                     #endif

                     for (unsigned int j=0;j<affiche->grille.size();j++) //pour touts les sprites du tableau
                     {

                        bool trouve=false;
                        unsigned int num_image=0;

                        if (affiche->grille[j].GetImage() != &(affiche->image_grille))
                        {
                            while (!trouve && num_image < resources_image_map.size()+1) //on recherche son image source dans le tableau d'images sources
                            {
                                if (affiche->grille[j].GetImage()== &resources_image_map[num_image])
                                {
                                    trouve=true;
                                    s.img=num_image;
                                }
                                else if (affiche->grille[j].GetImage()== affiche->sprite_mur.GetImage())
                                {
                                    trouve=true;
                                    s.img=resources_image_map.size();
                                }
                                num_image++;
                            }

                            s.x=affiche->grille[j].GetPosition().x; //on met ses coordonnées
                            s.y=affiche->grille[j].GetPosition().y;
                            s.d=affiche->grille[j].GetSubRect().Right;
                            s.h=affiche->grille[j].GetSubRect().Top;
                            s.b=affiche->grille[j].GetSubRect().Bottom;
                            s.g=affiche->grille[j].GetSubRect().Left;

                            if (affiche->grille_collision[j].GetImage()== &(affiche->image_colision_no))
                                s.colision=true;
                            else
                                s.colision=false;

                            fwrite(&s,sizeof(s),1,Fichier); //ecriture de la structure dans le fichier
                            #ifdef DEBUGING
                            fprintf(Fichier_fr,"<\t %d sprite> <%u reference sur le tab d'image> <%d position x> <%d position y> <%d droit ><%d gauche><%d haut ><%d bas> <rect>\n",j,s.img,s.x,s.y,s.d,s.g,s.h,s.b);
                            #endif
                        }

                     }

                    /*******************************************
                    *           ECRITURE DES PNJ               *
                    *******************************************/
                        /**************************
                        *   IMAGES                *
                        **************************/
                    vector<img> tab_nom_img_pnj;

                    for (unsigned int i=0;i<affiche->tab_PNJ_struct.size();i++) //pour tous les perso du tableau
                    {
                        bool OK=false;
                        unsigned int j=0;
                        string chemin_image_perso=string(string(affiche->tab_PNJ_struct[i].dossier)+"/"+string(affiche->tab_PNJ_struct[i].nom));

                        while (!OK && j<tab_nom_img_pnj.size() ) //on parcour le tableau des images
                        {
                            if (string(tab_nom_img_pnj[j].nom) == chemin_image_perso) //si le nom de l'images est le meme que celui du perso, on lui met sa place dans le tableau
                            {
                                OK=true;
                                affiche->tab_PNJ_struct[i].img=j;
                            }
                            j++;
                        }
                        if(!OK && j==tab_nom_img_pnj.size()) //si a la fin du tableau, on a pas trouvé l'image corespondante
                        {
                            img newimag;
                            strncpy(newimag.nom,chemin_image_perso.c_str(),99);
                            tab_nom_img_pnj.push_back(newimag); //on ajoute le chemin de l'image initialiser avec celui de l'image du perso
                            affiche->tab_PNJ_struct[i].img=j;
                        }
                    }

                    i.i=tab_nom_img_pnj.size(); //ecriture du nombre d-images de PNJ

                    fwrite(&i,sizeof(i),1,Fichier);
                    #ifdef DEBUGING
                    fprintf(Fichier_fr,"%u  <unsigned int nombre d'image de PNJ differentes>\n",i.i);
                    #endif

                    for(unsigned int j=0;j<i.i;j++) //ecriture de toutes les images des pnj
                    {
                        fwrite(&tab_nom_img_pnj[j],sizeof(img),1,Fichier);
                        #ifdef DEBUGING
                        fprintf(Fichier_fr,"%s  <nom de l'image %d>\n",tab_nom_img_pnj[j].nom,j);
                        #endif
                    }

                    /*******************************
                    *       PERSO PNJ              *
                    *******************************/

                    i.i=affiche->tab_PNJ_struct.size(); //ecriture du nombre
                    fwrite(&i,sizeof(i),1,Fichier);
                    #ifdef DEBUGING
                    fprintf(Fichier_fr,"%u  <unsigned int nombre de sprite de PNJ differentes>\n",i.i);
                    #endif


                    for(unsigned int j=0;j<affiche->tab_PNJ_struct.size();j++) //ecriture de tous les PNJ
                    {
                        fwrite(&affiche->tab_PNJ_struct[j],sizeof(_PNJ_),1,Fichier);
                        #ifdef DEBUGING
                        fprintf (Fichier_fr,"\t<%u img> <%d vue> <%d min %d max> <%d tx %d ty> ",affiche->tab_PNJ_struct[j].img,affiche->tab_PNJ_struct[j].vue,affiche->tab_PNJ_struct[j].min,affiche->tab_PNJ_struct[j].max,affiche->tab_PNJ_struct[j].tx,affiche->tab_PNJ_struct[j].ty);
                        fprintf (Fichier_fr,"<%d degats>\n",affiche->tab_PNJ_struct[j].degats);
                        #endif
                    }

                    /************************************
                    *       ECRITURE DES OBJ            *
                    ************************************/
                    /**************************
                    *   IMAGES                *
                    **************************/
                    tab_nom_img_pnj.clear();

                    for (unsigned int i=0;i<affiche->tab_OBJ_struct.size();i++) //pour tous les perso du tableau
                    {
                        bool OK=false;
                        unsigned int j=0;

                        string chemin_image=string(string(affiche->tab_OBJ_struct[i].dossier)+"/"+string(affiche->tab_OBJ_struct[i].nom));

                        while (!OK && j<tab_nom_img_pnj.size()) //on parcour le tableau des images
                        {
                            if (string(tab_nom_img_pnj[j].nom) ==chemin_image) //si le nom de l'images est le meme que celui du perso, on lui met sa place dans le tableau
                            {
                                OK=true;
                                affiche->tab_OBJ_struct[i].img=j;
                            }
                            j++;
                        }

                        if(!OK && j==tab_nom_img_pnj.size()) //si a la fin du tableau, on a pas trouvé l'image corespondante
                        {
                            img newimag;
                            strncpy(newimag.nom,chemin_image.c_str(),99);
                            tab_nom_img_pnj.push_back(newimag); //on ajoute le chemein de l'image initialiser avec celui de l'image du perso
                            affiche->tab_OBJ_struct[i].img=j;
                        }

                    }

                    i.i=tab_nom_img_pnj.size(); //ecriture du nombre d'image d'objets
                    fwrite(&i,sizeof(i),1,Fichier);
                    #ifdef DEBUGING
                    fprintf(Fichier_fr,"%u  <unsigned int nombre d'image de OBJ differentes>\n",i.i);
                    #endif

                    for(unsigned int j=0;j<tab_nom_img_pnj.size();j++) //ecriture de toutes les images des pnj
                    {
                        fwrite(&tab_nom_img_pnj[j],sizeof(img),1,Fichier);
                        #ifdef DEBUGING
                        fprintf(Fichier_fr,"%s  <nom de l'image %d>\n",tab_nom_img_pnj[j].nom,j);
                        #endif
                    }
                    /***************************
                    *   sprite OBJ             *
                    ***************************/

                    i.i=affiche->tab_OBJ_struct.size(); //ecriture du nombre;
                    fwrite(&i,sizeof(i),1,Fichier);
                    #ifdef DEBUGING
                    fprintf(Fichier_fr,"%u  <unsigned int nombre d'obj differents>\n",i.i);
                    #endif

                    for(unsigned int j=0;j<affiche->tab_OBJ_struct.size();j++)  //ecriture de tous les OBJ
                    {
                        fwrite(&affiche->tab_OBJ_struct[j],sizeof(_OBJ_),1,Fichier);
                        #ifdef DEBUGING
                        fprintf(Fichier_fr,"<tx %d ><ty %d> <dir %d> <type %d> <dossier %s > <fichier %s> \n",affiche->tab_OBJ_struct[j].tx,affiche->tab_OBJ_struct[j].ty,(int)affiche->tab_OBJ_struct[j].direction,(int)affiche->tab_OBJ_struct[j].type,affiche->tab_OBJ_struct[j].dossier,affiche->tab_OBJ_struct[j].nom);
                        #endif
                    }

                    /**********************************
                    *           ECRITURE DU HEROS     *
                    **********************************/
                    i.i=1;
                    fwrite(&i,sizeof(i),1,Fichier);

                    fwrite(&affiche->curent_Heros,sizeof(heros),1,Fichier);


                    /*******************************
                    *       OPTION DE LA CARTE     *
                    *******************************/

                    options_map map=affiche->getMapOption();
                    strncpy(map.nomVerif,NomFichier->text().toStdString ().c_str(),15);

                    fwrite(&map,sizeof(options_map),1,Fichier);
                    #ifdef DEBUGING
                    fprintf(Fichier_fr,"\n\n<%d gravitee> <%d frottement> <%s sons> <%s fond> <%d chono> <%s nom> <%s nomVerif> <%s type> <%s mythologie>\n",map.gravitee,map.frottement,map.sons,map.fond,map.chrono,map.nomMap,map.nomVerif,map.Type,map.mythologie);
                    fclose(Fichier_fr);
                    #endif


                    /********************************
                    *   Sons Spacialisés            *
                    ********************************/

                    i.i=affiche->tab_SONS_struct.size();
                    fwrite(&i,sizeof(i),1,Fichier);

                    Sons_Spacialise son;

                    for (unsigned int j=0;j<i.i;j++)
                    {
                        son=affiche->tab_SONS_struct[j];
                        fwrite(&son,sizeof(Sons_Spacialise),1,Fichier);
                    }

                    fclose(Fichier);
                     Erreur.SetText("Fichier Enregiste");
                 }
                 else
                    Erreur.SetText("Fichier NON Enregiste");
            }
        }
        // resolution de bug de merde
        Set_map_load();
      if(!deja_fait) ecrire_fichier(true);
}

void EDITEUR_MAP::initPNJ(_PNJ* PNJ)
{
    strncpy(PNJ->nom,PNJEditeur->resources_image_nom[PNJEditeur->Get_curent()].c_str(),49);
    strncpy(PNJ->dossier,PNJEditeur->Get_dossier().c_str(),49);
    PNJ->vue=5;
    PNJ->min=5;
    PNJ->max=5;
    PNJ->direction=(_type_direction) PNJEditeur->Get_ligne();
    PNJ->tx=PNJEditeur->Get_x();
    PNJ->ty=PNJEditeur->Get_y();
    PNJ->x=0;
    PNJ->y=0;
    PNJ->agressif=true;
    PNJ->boss=false;
    PNJ->degats=10;
    PNJ->vie=100;
    PNJ->vitesse=150;
    PNJ->lvl=1;
    PNJ->exp=5;
    PNJ->resize=1.0;
    strcpy(PNJ->dialogue,"");
    PNJ->element=NEUTRE;
    PNJ->quette=0;
    PNJ->resize=10;

};

bool EDITEUR_MAP::VerifAuto()
 {
    bool res;

    if(AutoEnabled && AutoIs)
        res= true;
    else
    {
         Erreur.SetText("Mode Automatique impossible: image introuvable ou mode desactive");
         res=false;
    }
    affiche->SetAuto(res);
    return res;
};
void EDITEUR_MAP::keyPressEvent (QKeyEvent * event)
{

    switch (event->key())
        {
            case Qt::Key_A : edition_auto->setCheckState ((Qt::CheckState)(2*(!edition_auto->checkState())));break;
            case Qt::Key_1 : Set_type_edition(MAP);break;
            case Qt::Key_2 : Set_type_edition(PNJ);break;
            case Qt::Key_3 : Set_type_edition(OBJ);break;
            case Qt::Key_4 : Set_type_edition(HEROS);break;
            case Qt::Key_5 : Set_type_edition(OPTIONS);break;
            case Qt::Key_6 : Set_type_edition(SKILLS);break;
            case Qt::Key_7 : Set_type_edition(QUETTE);break;
            default : affiche->keyPressEvent(event);break;
        }
}
