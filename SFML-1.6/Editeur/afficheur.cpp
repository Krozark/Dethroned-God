#include "afficheur.hpp"
#include <sstream>

using namespace std;
using namespace sf;


void AFFICHEUR_GRILLE::OnInit()
{
};


void AFFICHEUR_GRILLE::OnUpdate()
{
    Clear();

    if ( sprite_fond.GetImage()!=NULL)
    {
        sprite_fond.SetPosition(point.x,point.y);
        Draw(sprite_fond);
    }
//affichage de tous els prites necesaires
for(int j=point.y/32;j<point.y/32+23;j++)
  for (int i=point.x/32;i<point.x/32+45;i++)
  {
      Draw(grille[j*200+i]);
      Draw(grille_collision[j*200+i]);
  }

  for (unsigned int i=0;i<tab_SONS_struct.size();i++)
  {
        sprite_son.SetPosition(tab_SONS_struct[i].x,tab_SONS_struct[i].y);
        Draw(Shape::Circle(tab_SONS_struct[i].x,tab_SONS_struct[i].y,tab_SONS_struct[i].MinDistance/10.0,Color(255,255,0,127)));
        Draw(sprite_son);
  }

  grille_iter=tab_PNJ.begin();
  int i=0;
  stringstream out;
  String niveau;
  niveau.SetSize(20);
  niveau.SetColor(Color::Red);

  while(grille_iter!=tab_PNJ.end())
  {
      Draw(sf::Shape::Rectangle(tab_PNJ_struct[i].x- tab_PNJ_struct[i].min*taille_objets_pix,tab_PNJ_struct[i].y+taille_objets_pix,tab_PNJ_struct[i].x + tab_PNJ_struct[i].max*taille_objets_pix,tab_PNJ_struct[i].y+taille_objets_pix - 10,sf::Color(0,255,0,127) ));
      Draw(*grille_iter++);
      out.str("");
      out<<tab_PNJ_struct[i].lvl;
      niveau.SetText(out.str().c_str());
      niveau.SetPosition(tab_PNJ_struct[i].x,tab_PNJ_struct[i].y);
      Draw(niveau);

      if (tab_PNJ_struct[i].boss)
      {
        etoile_boss_S.SetPosition(tab_PNJ_struct[i].x+16,tab_PNJ_struct[i].y);
        Draw(etoile_boss_S);
      }
      i++;
  }


  grille_iter=tab_OBJ.begin();
  for (unsigned int i=0;i<tab_OBJ.size();i++)
  {
      Draw(tab_OBJ[i]);
      if (tab_OBJ_struct[i].type== TELEPORTEUR or tab_OBJ_struct[i].type==TELEPORTEUR_QUETTE)
      {
          telepor.SetPosition(tab_OBJ_struct[i].destx,tab_OBJ_struct[i].desty);
          Draw(telepor);
      }

      if (tab_OBJ_struct[i].quette)
      {
        etoile_boss_S.SetPosition(tab_OBJ_struct[i].x+16,tab_OBJ_struct[i].y);
        Draw(etoile_boss_S);
      }
  }


    Draw(*heros_obj);

    if (DestTP)
        Draw(TP);

//    Aide.Draw(this);


    //Display();
};

void AFFICHEUR_GRILLE::MajAuto()
{
    unsigned int _size=grille.size();

    for (unsigned int i=0;i<_size;i++)
        if(grille[i].GetImage() != &image_grille && grille[i].GetImage() != &image_mur)
        {
            Vector2f xy=grille[i].GetPosition();
            MajSpriteCourant(i,true);
            grille[i].SetPosition(xy);
        }
};

void AFFICHEUR_GRILLE::init()
{
    Aide="Test";
    Aide.SetFondColor(Color(255,255,0,255));
    Aide.SetTextColor(Color(0,17,255,255));
    Auto=false;
    collision=false;
    point=Vector2i(0,0);
    //initialisation de toutes les variables de la classe
    image_grille.LoadFromFile("./resources/image/32_32_1.png");
    image_mur.LoadFromFile("./resources/image/rien.png");
    image_colision_no.LoadFromFile("./resources/image/v.png");
    Image_vide.LoadFromFile("./resources/image/vide.png");
    Image_TP.LoadFromFile("./resources/image/TP.png");
    telepor_vert.LoadFromFile("./resources/image/TP2.png");
    image_son.LoadFromFile("./resources/image/son.png");
    etoile_boss_I.LoadFromFile("./resources/image/etoile.png");


    gr.SetImage(image_grille);
    vide.SetImage(Image_vide);
    sprite_mur.SetImage(image_mur);
    TP.SetImage(Image_TP);
    TP.SetCenter(16,16);
    telepor.SetImage(telepor_vert);
    telepor.SetCenter(16,16);
    sprite_son.SetImage(image_son);
    sprite_son.Resize(taille_objets_pix,taille_objets_pix);
    etoile_boss_S.SetImage(etoile_boss_I);

    type_edition=MAP;
    currentType=currentTypeReal=POTION_VIE;

    show_options=false;
    Options=new AFFICHEUR_OPTION();
    QObject::connect(Options,SIGNAL(Choix_DestinationPressed()),this,SLOT(ChangeDestTp()));

    hero_img.LoadFromFile("./resources/image/Heros/Walking.png");
    image_all_items.LoadFromFile("./resources/image/allItems.png");
    sprite_all_items.SetImage(image_all_items);
    heros_obj=new OBJECT(hero_img,7,1,1);

    option_perso=new AFFICHEUR_OPTION_PERSO(&curent_Heros,*heros_obj);
    option_perso->hide();

    Fenettre_skill=new Editeur_Skill();

    Fenettre_quette=new Editeur_Quette();
    Fenettre_quette->SetPnjObj(&tab_OBJ_struct,&tab_PNJ_struct);

    curentLvl=1;
    Fenettre_quette->hide();

    reset();
    map_option=new MAP_OPTION();
    QObject::connect(map_option,SIGNAL(FondChanged()),this,SLOT(modifFond()));

    Options->set_sprite_all_items(&sprite_all_items);

    sons_option=new SONS_OPTION();

    Aide.SetPosition(taille_objets_pix*4,fenettre_taille_y-taille_objets_pix+5);
}

void AFFICHEUR_GRILLE::reset()
{
    //remise à zero de toutes les varialbles
    grille.clear();
    grille_collision.clear();

    for ( int i=0;i<taille_x*taille_y;i++)
    {
        int x=(i%taille_x);
        int y=(i/taille_x);

        if (x==0 || x==taille_x-1 || y==taille_y-1)
            grille.push_back(sprite_mur);
        else
            grille.push_back(gr);
        grille_collision.push_back(vide);
        grille_collision[i].SetPosition(x*taille_objets_pix,taille_objets_pix*y);
        grille[i].SetPosition(x*taille_objets_pix,taille_objets_pix*y);
    }

    tab_PNJ.clear();
    tab_PNJ_struct.clear();

    tab_OBJ.clear();
    tab_OBJ_struct.clear();

    tab_SONS_struct.clear();


    curent_Heros.acceleration=1000;
    curent_Heros.direction=DROITE;
    curent_Heros.vitesse=200;
    heros_obj->Resize(taille_objets_pix,taille_objets_pix);
    Set_Heros_coord(2,taille_carte_case_y-9);

    //sprite_fond.SetImage(Image_vide);



    PressedRight=PressedLeft=DestTP=false;



};

void AFFICHEUR_GRILLE::UpLevelPnj(int u)
{
    for (unsigned int i=0;i<tab_PNJ_struct.size();i++)
        if (tab_PNJ_struct[i].lvl + u >1 )
        {
            Options->set_options(&tab_PNJ_struct[i],&tab_PNJ[i]);
            Options->change_lvl(tab_PNJ_struct[i].lvl + u);
        }

}

void AFFICHEUR_GRILLE::keyPressEvent (QKeyEvent * event)
{
    switch (type_edition)
    {
        case MAP :
            Aide="Edition de la carte";
            switch (event->key())
            {
                case Qt::Key_C :
                    collision=!collision;
                    if (collision) Aide+=" Sans collisions";
                    break;
                default : break;
            }
        break;

        case PNJ :
        {
            switch (event->key())
            {
                case Qt::Key_Plus: SetLvl(curentLvl+1); break;
                case Qt::Key_Minus: SetLvl(curentLvl-1); break;
                case Qt::Key_Asterisk:UpLevelPnj(1);break;
                case Qt::Key_Slash: UpLevelPnj(-1);break;
                default: break;
            }
        }
        break;

        case OBJ : Aide="Edition des Objets";
            switch (event->key())
            {
                case Qt::Key_F1 :
                    currentType=POTION_VIE;
                    Aide+=" Potion Vie";
                break;

                case Qt::Key_F2 :
                    currentType=POTION_MANA;
                    Aide+=" Potion Mana";
                break;

                case Qt::Key_F3 :
                    currentType=COFFRE;
                    Aide+=" Coffre";
                break;

                case Qt::Key_F4 :
                    currentType=PARCHEMIN;
                    Aide+=" Parchemin";
                break;

                case Qt::Key_F5 :
                    currentType=SAUVEGARDE;
                    Aide+=" Sauvegarde";
                break;

                case Qt::Key_F6 :
                    currentType=SAUVEGARDE_FIX;
                    Aide+=" Sauvegarde Fix";
                break;

                case Qt::Key_F7 :
                    currentType=TELEPORTEUR;
                    Aide+=" Teleporteur";
                break;

                case Qt::Key_F8 :
                    currentType=OR;
                    Aide+=" Or";
                break;

                case Qt::Key_F9 :
                    currentType=TRAMPOLINE;
                    Aide+=" Trempling";
                break;
                case Qt::Key_F10 :
                    currentType=ACCELERATEUR;
                    Aide+=" Accelerateur";
                break;
                case Qt::Key_F11 :
                    currentType=PIEGE;
                    Aide+=" Piege";
                break;
                case Qt::Key_A :
                    currentType=PORTAIL;
                    Aide+=" Portail";
                break;
                case Qt::Key_Z:
                    currentType=MARCHAND;
                    Aide+=" Marchand";
                break;
                case Qt::Key_E:
                    currentType=OBJ_QUETTE;
                    Aide+=" Objet Quette";
                break;
                case Qt::Key_R:
                    currentType=RIEN;
                    Aide+=" Rien";
                break;
               case Qt::Key_T:
                    currentType=TELEPORTEUR_QUETTE;
                    Aide+=" Teleporteur de quette";
                break;
                default : /*SetType(currentTypeReal);*/break;
            }
        break;

        case SONS :
        {
            Aide="Edition des sons spacialises";
        }
        break;

        case HEROS: Aide="Edition du Hero";
        break;

        case OPTIONS:
        case SKILLS:
        case QUETTE:
        default : Aide="Edition dans une fenettre differente";
            break;
    }

};

void AFFICHEUR_GRILLE::keyReleaseEvent ( QKeyEvent * event )
{
    switch (event ->key())
    {
        case Qt::Key_F1 :
        case Qt::Key_F2 :
        case Qt::Key_F3 :
        case Qt::Key_F4 :
        case Qt::Key_F5 :
        case Qt::Key_F6 :
        case Qt::Key_F7 :
        case Qt::Key_F8 :
        case Qt::Key_F9 :
        case Qt::Key_F10:
        case Qt::Key_F11:
        case Qt::Key_A:
        case Qt::Key_Z:
        case Qt::Key_E:
        case Qt::Key_R:
        case Qt::Key_T:
            if (type_edition == OBJ && !DestTP)
            {
                SetType(currentTypeReal);
            }break;
        default : break;
    }

    if((currentType == TELEPORTEUR  or currentType == TELEPORTEUR_QUETTE) && !DestTP)
        {
            DestTP=true;
            //Telep=&(tab_OBJ_struct[tab_OBJ_struct.size()-1]);
            TP.SetPosition(Telep->destx,Telep->desty);
        }

};
void AFFICHEUR_GRILLE::mouseReleaseEvent (QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
        PressedLeft=false;
    if (event->button()==Qt::RightButton)
        PressedRight=false;
     if (event->button()==Qt::MiddleButton)
     {
        setCursor(Qt::ArrowCursor);
        Relased();
     }

    if (type_edition==OBJ)
    {
        if((currentType == TELEPORTEUR  or currentType== TELEPORTEUR_QUETTE )&& !DestTP)
        {
            DestTP=true;
            //Telep=&(tab_OBJ_struct[tab_OBJ_struct.size()-1]);
            TP.SetPosition(Telep->destx,Telep->desty);
        }
       /* else if ((currentType == TELEPORTEUR or currentType== TELEPORTEUR_QUETTE) && DestTP)
        {
            int x=event->pos().x();
            int y=event->pos().y();

            Telep->destx=x;
            Telep->desty=y;
            DestTP=false;
        }*/
        else if (DestTP)
        {
            int x=event->pos().x();
            int y=event->pos().y();

            Telep->destx=x;
            Telep->desty=y;
            DestTP=false;

        }
    }
};

 void AFFICHEUR_GRILLE::mouseMoveEvent ( QMouseEvent * event )
 {

    if (type_edition==MAP)
    {
        int x=event->pos().x()/taille_objets_pix;
        int y=event->pos().y()/taille_objets_pix;

        unsigned int c=convertToCase(Vector2i(x,y));

        if (PressedLeft)
        {
            if (Auto)
            {
                grille[c].SetImage(*AutoImage);
                MajSpriteCourant(c,true);
                //MajSpriteCourant(c,true);
            }
            else
                grille[c]=sprite_courant;

            grille[c].SetPosition(x*taille_objets_pix,y*taille_objets_pix);
            if (collision)
            {
                if (grille_collision[c].GetImage() == &Image_vide)
                    grille_collision[c].SetImage(image_colision_no);
            }
             else
             {
                if (grille_collision[c].GetImage() == &image_colision_no)
                    grille_collision[c].SetImage(Image_vide);
             }
        }
        else if (PressedRight)
        {
            if (x==0 || x==taille_x-1 || y==taille_y-1)
                grille[c]=sprite_mur;
            else
                grille[c]=gr;
            grille[c].SetPosition(x*taille_objets_pix,y*taille_objets_pix);
            MajSpriteCourantDel(c);
        }
    }
    else if (type_edition == OBJ)
    {
        if (DestTP)
        {
            int x=event->pos().x();
            int y=event->pos().y();
            TP.SetPosition(x,y);
            Telep->destx=x;
            Telep->desty=y;
        }
    }
    else if (type_edition==PNJ)
    {

        if (PressedLeft)    //si le bouton gauche est pressé, on doit redimentioné une des 2 borne de déplacement
        {
            unsigned int x=event->x()/taille_objets_pix,
            pos=tab_PNJ_struct[ref_pnj.i].x/taille_objets_pix;

            if ((tab_PNJ_struct[ref_pnj.i].x - tab_PNJ_struct[ref_pnj.i].min*taille_objets_pix >= (x-1)*taille_objets_pix or tab_PNJ_struct[ref_pnj.i].x - tab_PNJ_struct[ref_pnj.i].min*taille_objets_pix >= (x+1)*taille_objets_pix) and x< pos  ) // si c'est celle de gauche
                tab_PNJ_struct[ref_pnj.i].min=pos-x;//on lui attribu une valeure
            else if(((tab_PNJ_struct[ref_pnj.i].x + tab_PNJ_struct[ref_pnj.i].max*taille_objets_pix >= (x-1)*taille_objets_pix) or (tab_PNJ_struct[ref_pnj.i].x + tab_PNJ_struct[ref_pnj.i].max*taille_objets_pix>= (x+1)*taille_objets_pix )) and x>pos+1) // si c'est celle de droite
                tab_PNJ_struct[ref_pnj.i].max=x-pos;//on lui attribu une valeure

        }
    }
        clickRelase->x=event->x();
        clickRelase->y=event->y();

 };

void AFFICHEUR_GRILLE::mousePressEvent ( QMouseEvent * event )
{

    int x=event->pos().x()/taille_objets_pix;
    int y=event->pos().y()/taille_objets_pix;

    unsigned int c=convertToCase(Vector2i(x,y));
    unsigned int i=0;

    show_options=false;

    if (type_edition==MAP)
    {
        if (event->button()==Qt::LeftButton)
        {
            PressedLeft=true;
            if (Auto)
            {
                MajSpriteCourant(c,true);
                MajSpriteCourant(c,true);
            }

            else
                grille[c]=sprite_courant;

            grille[c].SetPosition(x*taille_objets_pix,y*taille_objets_pix);
            if (collision)
            {
                if (grille_collision[c].GetImage() == &Image_vide)
                    grille_collision[c].SetImage(image_colision_no);
            }
             else
             {
                if (grille_collision[c].GetImage() == &image_colision_no)
                    grille_collision[c].SetImage(Image_vide);
             }
        }
        else if (event->button()==Qt::RightButton)
        {
            PressedRight=true;
           // if (grille[c].GetImage()==AutoImage)


            if (x==0 || x==taille_x-1 || y==taille_y-1)
                grille[c]=sprite_mur;

            else
                grille[c]=gr;

             MajSpriteCourantDel(c);
            grille[c].SetPosition(x*taille_objets_pix,y*taille_objets_pix);
        }
    }
    else if (type_edition==PNJ)
    {
        i=0;
         if (event->button()==Qt::LeftButton)
        {
            bool existe=false;
            bool modif_dir=false;

            while ( i < tab_PNJ.size() && !existe)
            {
                 if (tab_PNJ[i].GetPosition().x==x*taille_objets_pix and  tab_PNJ[i].GetPosition().y==y*taille_objets_pix)//verification si l'ajout n'en pas par dessu un PNJ existant déjà
                    existe=true;
                else if  (tab_PNJ[i].GetPosition().x-tab_PNJ_struct[i].min*taille_objets_pix==x*taille_objets_pix and tab_PNJ[i].GetPosition().y==y*taille_objets_pix)
                {
                    ref_pnj.i=i;
                    ref_pnj.dir=ref_pnj.MIN;
                    modif_dir=existe=PressedLeft=true;

                }
                else if (tab_PNJ[i].GetPosition().x+(tab_PNJ_struct[i].max-1)*taille_objets_pix==x*taille_objets_pix and tab_PNJ[i].GetPosition().y==y*taille_objets_pix)
                {
                    ref_pnj.i=i;
                    ref_pnj.dir=ref_pnj.MAX;
                    modif_dir=existe=PressedLeft=true;
                }
                 else
                    i++;
            }

            if (existe and !modif_dir) //si il existe on montre les options
            {
                //ajouter l'ouverture d'une fenettre d'options
                show_options=true;

            }
            else if (!modif_dir and !existe)//sinon on l'ajoute
            {
                curent_PNJ.x=x*taille_objets_pix;
                curent_PNJ.y=y*taille_objets_pix;
                curent_PNJ.element=NEUTRE;
                curent_PNJ.lvl=curentLvl;
                curent_PNJ.quette=0;
                curent_PNJ.resize=10;
                add_PNJ();
                Options->set_options(&tab_PNJ_struct[i],&tab_PNJ[i]);

            }
        }
        else if (event->button()==Qt::RightButton)
        {
           unsigned int i=0;
            bool existe=false;

            while ( i < tab_PNJ.size() && !existe)
            {
                 if (tab_PNJ[i].GetPosition().x==x*taille_objets_pix && tab_PNJ[i].GetPosition().y==y*taille_objets_pix)
                 {
                     existe=true;
                     tab_PNJ.erase(tab_PNJ.begin()+i);
                     tab_PNJ_struct.erase(tab_PNJ_struct.begin()+i);
                 }
             i++;
            }
        }
    }
    else if (type_edition==OBJ)
    {
        if (DestTP)
        {
            Telep->destx=x*taille_objets_pix;
            Telep->desty=y*taille_objets_pix;
        }
        else if (event->button()==Qt::LeftButton)
        {
            bool existe=false;

            while ( i < tab_OBJ.size() && !existe)
            {
                 if (tab_OBJ[i].GetPosition().x==x*taille_objets_pix && tab_OBJ[i].GetPosition().y==y*taille_objets_pix) //verification si l'ajout n'en pas par dessu un OBJ existant déjà
                    existe=true;
                else
                    i++;
            }

            if (existe) //si il existe on montre les options
            {
                //ajouter l'ouverture d'une fenettre d'options
                currentType=tab_OBJ_struct[i].type;
                if( (currentType == TELEPORTEUR) or (currentType==TELEPORTEUR_QUETTE))
                {
                    Telep=&(tab_OBJ_struct[i]);
                    TP.SetPosition(Telep->destx,Telep->desty);
                }
                show_options=true;
            }
            else //sinon on l'ajoute
            {
                curent_OBJ.x=x*taille_objets_pix;
                curent_OBJ.y=y*taille_objets_pix;
                curent_OBJ.destx=curent_OBJ.desty=0;
                curent_OBJ.quette=0;
                curent_OBJ.idSkill=0;
                curent_OBJ.resize=10;
                curent_OBJ.tx=4;
                curent_OBJ.ty=4;
                curent_OBJ.type=currentType;
                strncpy(curent_OBJ.dialogue,"",TAILLE_DIALOGUE_PNJ-1);

                switch (currentType)
                {
                    case POTION_VIE :
                        break;
                    case POTION_MANA:
                        break;
                    case COFFRE:
                      break;
                    case PARCHEMIN:
                       curent_OBJ.quette=1;
                        curent_OBJ.desty=5; //exp
                       break;
                    case SAUVEGARDE:
                       break;
                    case SAUVEGARDE_FIX:
                        break;
                   case TELEPORTEUR_QUETTE:
                        curent_OBJ.quette=1;
                        curent_OBJ.destx=x*taille_objets_pix;
                        curent_OBJ.desty=y*taille_objets_pix;
                    case TELEPORTEUR:
                        curent_OBJ.destx=x*taille_objets_pix;
                        curent_OBJ.desty=y*taille_objets_pix;
                       break;
                    case OR:
                        curent_OBJ.destx=20;    //quantitée d'or
                        break;
                    case TRAMPOLINE:
                        curent_OBJ.destx=600;   //force de saut
                       break;
                    case ACCELERATEUR:
                      curent_OBJ.destx=600;   //force suplémentaire
                        break;
                    case PIEGE:
                        curent_OBJ.destx=25;    //degats
                        break;
                    case OBJ_QUETTE :
                        curent_OBJ.quette=1;
                        curent_OBJ.destx=5;
                        break;
                    case RIEN:
                        break;
                    default:
                    break;
                }

                if (curent_OBJ.type <PIEGE)
                {
                     //nombre d'items different sur l'image
                    if ( curent_OBJ.type==TELEPORTEUR)
                    {
                        Telep=&(tab_OBJ_struct[i]);
                    }
                    curent_OBJ.ty=11;
                    sprite_courant_OBJ=sprite_all_items;
                    curent_OBJ.direction=(_type_direction)curent_OBJ.type;
                    strcpy(curent_OBJ.dossier,"./resources/image");
                    strcpy(curent_OBJ.nom,"allItems.png");
                }
                else if (curent_OBJ.type==TELEPORTEUR_QUETTE)
                {
                    Telep=&(tab_OBJ_struct[i]);
                }

                add_OBJ();
                Options->set_options(&tab_OBJ_struct[i],&tab_OBJ[i]);

            }
        }
        else if (event->button()==Qt::RightButton)
        {
            unsigned int i=0;
            bool existe=false;

            while ( i < tab_OBJ.size() && !existe)
            {
                 if (tab_OBJ[i].GetPosition().x==x*taille_objets_pix && tab_OBJ[i].GetPosition().y==y*taille_objets_pix)
                 {
                     existe=true;
                     tab_OBJ.erase(tab_OBJ.begin()+i);
                     tab_OBJ_struct.erase(tab_OBJ_struct.begin()+i);
                     Fenettre_quette->reload();
                 }
             i++;
            }
        }
    }
    else if (type_edition==HEROS)
    {
        if (curent_Heros.x!=x || curent_Heros.y!=y)
        {
            Set_Heros_coord(x,y);
            option_perso->set_infos(curent_Heros.direction,curent_Heros.vitesse,curent_Heros.acceleration);
        }
        else
        show_options=true;
    }
    else if (type_edition==OPTIONS)
    {
        show_options=true;
    }
    else if (type_edition==SKILLS)
    {
        Fenettre_skill->show();
    }
    else if (type_edition==QUETTE)
    {

        Fenettre_quette->show();
    }
    else if (type_edition==SONS)
    {
        bool existe=false;
        i=0;


            if (event->button()==Qt::LeftButton)
            {
                while ( i < tab_SONS_struct.size() && !existe)
                {
                     if (tab_SONS_struct[i].x==x*taille_objets_pix and  tab_SONS_struct[i].y==y*taille_objets_pix)//verification si l'ajout n'en pas par dessu un PNJ existant déjà
                        existe=true;
                    else
                    i++;
                }

                if (existe)
                {
                    sons_option->SetSon(&(tab_SONS_struct[i]));
                    show_options=true;
                }
                else
                {
                    Sons_Spacialise nouveau_son;
                    nouveau_son.x=x*taille_objets_pix;
                    nouveau_son.y=y*taille_objets_pix;
                    nouveau_son.attenuation=10;
                    nouveau_son.MinDistance=1000;
                    nouveau_son.Volume=1000;
                    strcpy(nouveau_son.nom," ");
                    strcpy(nouveau_son.dossier," ");
                    tab_SONS_struct.push_back(nouveau_son);
                }
            }
            else if (event->button()==Qt::RightButton)
            {
                while ( i < tab_SONS_struct.size() && !existe)
                {
                     if (tab_SONS_struct[i].x==x*taille_objets_pix and  tab_SONS_struct[i].y==y*taille_objets_pix)//verification si l'ajout n'en pas par dessu un PNJ existant déjà
                    {
                        existe=true;
                        tab_SONS_struct.erase(tab_SONS_struct.begin()+i);
                    }
                    i++;
                }
            }
    }

    if (show_options)
    {
        switch (type_edition)
        {
             case PNJ:
             {
                    Options->set_options(&tab_PNJ_struct[i],&tab_PNJ[i]);
                    Options->show();
             } break;

             case OBJ:
             {
                    Options->set_options(&tab_OBJ_struct[i],&tab_OBJ[i]);
                    Options->show();
             } break;

             case HEROS:
             {
                 option_perso->show();
             }break;
             case OPTIONS:
             {
                 map_option->show();

             };break;
             case SONS:
             {
                 sons_option->show();
             }

             default: break;
        }
    }

    if (event->button()==Qt::MiddleButton)
    {
        clickPush->x=clickRelase->x=event->x();
        clickPush->y=clickRelase->y=event->y();
        setCursor(Qt::ClosedHandCursor);

        Clicked();
    }


};

void AFFICHEUR_GRILLE::Set_Heros_coord(int x,int y)
{
    curent_Heros.x=x;
    curent_Heros.y=y;
    heros_obj->SetPosition(x*taille_objets_pix,y*taille_objets_pix);

};

void AFFICHEUR_GRILLE::Set_grille( std::vector<sf::Sprite> g)
{
    grille=g;

    for (unsigned int i=0;i<grille.size();i++)
    {
        if ( grille[i].GetImage()==0)
        {
            grille[i]= gr;
            grille[i].SetPosition( ( i%taille_carte_case_x) *taille_objets_pix, ((int)(i / taille_carte_case_x))*taille_objets_pix);
        }
    }
};

void AFFICHEUR_GRILLE::Set_PNJ(_PNJ p,Image* i)
{
    curent_PNJ=p;

    int taillex=i->GetWidth()/p.tx;
    int tailley= i->GetHeight()/p.ty;

    Sprite spr;

    spr.SetImage(*i);
    spr.SetSubRect(IntRect(0,(int)p.direction*tailley,taillex,(int)p.direction*tailley+tailley));
    spr.Resize(taille_objets_pix,taille_objets_pix);

    sprite_courant_PNJ=spr;

}

void AFFICHEUR_GRILLE::Set_OBJ(_OBJ p,Image* i)
{
    curent_OBJ=p;

    int taillex=i->GetWidth()/p.tx;
    int tailley= i->GetHeight()/p.ty;

    Sprite spr;

    spr.SetImage(*i);
    spr.SetSubRect(IntRect(0,(int)p.direction*tailley,taillex,(int)p.direction*tailley+tailley));
    spr.Resize(taille_objets_pix,taille_objets_pix);

    sprite_courant_OBJ=spr;
};

void AFFICHEUR_GRILLE::add_PNJ()
{
    sprite_courant_PNJ.SetPosition( curent_PNJ.x, curent_PNJ.y);

    tab_PNJ.push_back(sprite_courant_PNJ);
    tab_PNJ_struct.push_back(curent_PNJ);
};

void AFFICHEUR_GRILLE::add_OBJ()
{
    const int _sizeX=sprite_courant_OBJ.GetImage()->GetWidth();
    const int _sizeY=sprite_courant_OBJ.GetImage()->GetHeight();
    const int _size=curent_OBJ.ty;

    sprite_courant_OBJ.SetPosition(curent_OBJ.x,curent_OBJ.y);
    sprite_courant_OBJ.SetSubRect(IntRect(0,(_sizeY/_size)* curent_OBJ.direction,_sizeX/4,(_sizeY/_size)*( curent_OBJ.direction+1)));
    tab_OBJ.push_back(sprite_courant_OBJ);
    tab_OBJ_struct.push_back(curent_OBJ);
};

void AFFICHEUR_GRILLE::MajSpriteCourant(int c,bool first)
{

        grille[c].SetImage(*AutoImage);
        Sprite tmp=sprite_courant;
        sprite_courant=grille[c];
        sprite_courant.SetSubRect(IntRect(0,0,0,0));

        Vector2i V=convertToVector(c);
        bool haut,bas,droite,gauche,haut_gauche,haut_droit,bas_gauche,bas_droit;
        haut=bas=droite=gauche=haut_gauche=haut_droit=bas_gauche=bas_droit=false;    //bool qui servent a reperer les coter

        //puis on regarde dans les case ou il peu y avoir d'autres sprites
        gauche=(V.x > 0)? testProximite(V.x-1,V.y):false;
        haut=(V.y > 0)? testProximite(V.x,V.y-1):false;
        droite=(V.x < taille_x-1)? testProximite(V.x+1,V.y):false;
        bas=(V.y < taille_y-1)? testProximite(V.x,V.y+1):false;

        haut_gauche=(V.x>0 && V.y>0)? testProximite(V.x-1,V.y-1):false;
        haut_droit=(V.x< taille_x+1 && V.y>0)? testProximite(V.x+1,V.y-1):false;
        bas_gauche=(V.y< taille_x-1 && V.x>0)? testProximite(V.x-1,V.y+1):false;
        bas_droit=(V.y<taille_x-1 && V.x<taille_x-1)? testProximite(V.x+1,V.y+1):false;

        IntRect pos;

        int sum= gauche+haut+droite+bas;

        if (sum==1 && droite)               //platforme de droite
            pos=IntRect(0,5*32,32,6*32);
        else if (sum==1 && gauche)              //platforme de gauche
            pos=IntRect(2*32,5*32,3*32,6*32);
        else if (sum==2 && !bas && ! haut)          //platforme du milieu
            pos=IntRect(32,5*32,2*32,6*32);
        else if (droite && bas && !gauche && ! haut) //angle de haut gauche
            pos=IntRect(0,0,32,32);
        else if (bas && gauche && !droite && !haut) //angle de haut droite
            pos=IntRect(2*32,0,3*32,32);
        else if (haut && droite && !gauche && ! bas) //angle de bas gauche
            pos=IntRect(0,4*32,32,5*32);
        else if (haut && gauche && !bas && !droite) //angle de bas droite
            pos=IntRect(2*32,4*32,3*32,5*32);
        else if (droite && gauche && bas && !haut)  //haut
            pos=IntRect(32,0,2*32,32);
        else if (droite && gauche && haut &&!bas)      //bas
            pos=IntRect(32,4*32,2*32,5*32);
        else if (haut && bas && droite && !gauche)      //droite
            pos=IntRect(0,2*32,32,3*32);
        else if (haut && bas && gauche && !droite)      //gauche
            pos=IntRect(2*32,2*32,3*32,3*32);
        else if (sum == 4)
        {
            if (!haut_gauche && haut_droit && bas_droit && bas_gauche)      //petit angle haut gauche
                pos=IntRect(0,32,32,2*32);
            else if(haut_gauche && !haut_droit && bas_droit && bas_gauche)  //petit angle haut droit
                pos=IntRect(2*32,32,3*32,2*32);
            else if(haut_gauche && haut_droit && bas_droit && !bas_gauche)  //petit angle bas gauche
                pos=IntRect(0,3*32,32,4*32);
            else if(haut_gauche && haut_droit && !bas_droit && bas_gauche)  //petit angle bas droit
                pos=IntRect(2*32,3*32,3*32,4*32);
            else
                pos=IntRect(32,2*32,2*32,3*32);
        }
        else
            pos=IntRect(32,2*32,2*32,3*32);

        sprite_courant.SetSubRect(pos);
        grille[c]=sprite_courant;

        if (first)
        {
            if (gauche && IsImageAuto(c-1))          MajSpriteCourant(c-1);
            if (droite&& IsImageAuto(c+1))          MajSpriteCourant(c+1);
            if (bas&& IsImageAuto(V.x,V.y+1))             MajSpriteCourant(V.x,V.y+1);
            if (haut&& IsImageAuto(V.x,V.y-1))            MajSpriteCourant(V.x,V.y-1);
            if (haut_gauche&& IsImageAuto(V.x-1,V.y-1))     MajSpriteCourant(V.x-1,V.y-1);
            if (haut_droit&& IsImageAuto(V.x+1,V.y-1))      MajSpriteCourant(V.x+1,V.y-1);
            if (bas_gauche&& IsImageAuto(V.x-1,V.y+1))      MajSpriteCourant(V.x-1,V.y+1);
            if (bas_droit&& IsImageAuto(V.x+1,V.y+1))       MajSpriteCourant(V.x+1,V.y+1);
            sprite_courant=tmp;
        }

};


void AFFICHEUR_GRILLE::MajSpriteCourantDel(int c)
{
    Vector2i V=convertToVector(c);
        bool haut,bas,droite,gauche;
        haut=bas=droite=gauche=false;    //bool qui servent a reperer les coter
        //puis on regarde dans les case ou il peu y avoir d'autres sprites
        gauche=(V.x > 0)? testProximite(V.x-1,V.y):false;
        haut=(V.y > 0)? testProximite(V.x,V.y-1):false;
        droite=(V.x < taille_x-1)? testProximite(V.x+1,V.y):false;
        bas=(V.y < taille_y-1)? testProximite(V.x,V.y+1):false;

        grille_collision[c].SetImage(Image_vide);

        if (gauche &&IsImageAuto(V.x-1,V.y)) MajSpriteCourant(V.x-1,V.y,true);
        if (droite && IsImageAuto(V.x+1,V.y)) MajSpriteCourant(V.x+1,V.y,true);
        if (haut&& IsImageAuto(V.x,V.y-1))   MajSpriteCourant(V.x,V.y-1,true);
        if (bas&& IsImageAuto(V.x,V.y+1))    MajSpriteCourant(V.x,V.y+1,true);

};

void AFFICHEUR_GRILLE::Set_type_edition(int c)
{
    type_edition=(_type_edition)c;


    switch (type_edition)
    {
        case MAP : Aide="Edition de la carte";
            break;

        case PNJ :
        {
             Aide="Edition des PNJ";
            stringstream out;
            out<<" lvl : "<<curentLvl;
            Aide+=out.str().c_str();
        }
            break;

        case OBJ : SetType(currentTypeReal);
            break;

        case HEROS: Aide="Edition du Heros";
            break;

        case SONS: Aide="Edition des sons spacialises";
            break;

        case OPTIONS:
        case SKILLS:
        case QUETTE:
        default : Aide="Edition dans une fenettre differente";
            break;
    }
};


void AFFICHEUR_GRILLE::SetType(_type_objets t)
{
    currentType=currentTypeReal=t;

    if (type_edition ==OBJ)
    {
        Aide="Edition des Objets";
        switch (currentType)
        {
            case POTION_VIE:
                Aide+=" Potion Vie";
            break;

            case POTION_MANA:
                Aide+=" Potion Mana";
            break;

            case COFFRE:
                Aide+=" Coffre";
            break;

            case PARCHEMIN:
                Aide+=" Parchemin";
            break;

            case SAUVEGARDE:
                Aide+=" Sauvgarde";
            break;

            case SAUVEGARDE_FIX:
                Aide+=" Sauvgarde Fix";
            break;

            case TELEPORTEUR:
                Aide+=" Teleporteur";
            break;

            case PORTAIL:
                Aide+=" Portail";
            break;

            case RIEN:
                Aide+=" Rien";
            break;
            case TELEPORTEUR_QUETTE:
                Aide+="Teleporteur de quette";
            break;
            default : break;
        }
    }
};

void AFFICHEUR_GRILLE::SetLvl(int l)
{
    if (l >0)
        curentLvl=l;
    else
        curentLvl=1;
    Aide="Edition des PNJ";
    stringstream out;
    out<<" lvl : "<<curentLvl;
    Aide+=out.str().c_str();
};

void AFFICHEUR_GRILLE::modifFond()
{
    image_fond.LoadFromFile(getMapOption().fond);
    sprite_fond.SetImage(image_fond);
    sprite_fond.Resize(45*taille_objets_pix,23*taille_objets_pix);
};
