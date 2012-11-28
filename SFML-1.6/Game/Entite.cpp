#include "Entite.hpp"
#include <string>
#include <iostream>

using namespace sf;
using namespace std;
//************************************************************************
//      Constructeur
//************************************************************************
Entite::Entite(sf::Image &image,int x,int y, _type_direction dirDepart, sf::Vector2f pos)
{
    //image
    charger_image_sprite(image,x,y);
    Resize(1.5,1.5);

    //direction de départ
    SetDirection(dirDepart);
    //position
    SetPosition(pos);

    m_vector.x = 0;
    m_vector.y = 0;

    Pause();
    SetFrameTime(0.2);

    gravitee=9.81;
    frottement=2000;
}

//constructeur sans animations pour le heros
Entite::Entite(Vector2f pos)
{
    //position
    SetPosition(pos);

    m_vector.x = 0;
    m_vector.y = 0;
    gravitee=9.81;

    SetFrameTime(0.2);
}



//************************************************************************
//      Chargement d'une image (découpage par frame)
//************************************************************************
void Entite::charger_image_sprite(Image &image,int x,int y)
{
    int perso_image_taille_x=(image.GetWidth())/x;
    int perso_image_taille_y=(image.GetHeight())/y;

    for (int i=1;i<=x;++i)
    {
        GoDown.PushFrame(Frame(&image,Rect<int>(perso_image_taille_x*(i-1), 0, perso_image_taille_x*i, perso_image_taille_y)));
        if(y>1)
        GoLeft.PushFrame(Frame(&image,Rect<int>(perso_image_taille_x*(i-1),perso_image_taille_y*1, perso_image_taille_x*i, perso_image_taille_y*2)));
        if(y>2)
        GoRight.PushFrame(Frame(&image,Rect<int>(perso_image_taille_x*(i-1),perso_image_taille_y*2, perso_image_taille_x*i, perso_image_taille_y*3)));
        if(y>3)
        GoUp.PushFrame(Frame(&image,Rect<int>(perso_image_taille_x*(i-1),perso_image_taille_y*3, perso_image_taille_x*i, perso_image_taille_y*4)));
    }

    SetCenter(perso_image_taille_x/2,perso_image_taille_y);
}


//************************************************************************
//      Mise à jour de l'entite
//************************************************************************
void Entite::Update(float time,int nombre_division,bool tab_collision[MAPSIZECARTEY][MAPSIZECARTEX])
{
    //booleen qui dit si on touche ou pas le sol
    m_floor_collision = false;

    //int nombre_division=1;
    float final_Time=0.0;

    //position du perso avant le traitement
    Vector2f oldPos = GetPosition();

    while (nombre_division > 0 and  !m_floor_collision)
    {
        final_Time+=time;
    //si la force en y est inférieure à 600, on la met à jour en fonction de la gravité
        m_vector.y += (m_vector.y <600)? time *gravitee*96 : 0; //sinon on ne la modifie pas
        //nombre de cases à vérifier en y selon la vitesse du perso
        int nb_case_verif_y= (m_vector.y>0)? m_vector.y/(TAILLEBLOC+96):0;  //TAILLEBLOC = taille d'une case (32 px)
        //détection de la position future du perso
        int y=(oldPos.y + m_vector.y * time)/TAILLEBLOC;
        int x=(oldPos.x + m_vector.x * time)/TAILLEBLOC;

        if (x >0 and y >0 and x<MAPSIZECARTEX and y<MAPSIZECARTEY)
        {
             //colision avec le mur de droite ou gauche
            m_vector.x=(tab_collision[y-1][x+ ((m_vector.x < 0)? -1 : 1)]or tab_collision[y-1][x]) ? 0 : m_vector.x;

            //collisions avec le bas
            if (m_vector.y>0)       //si le perso retombe
            {
                if (tab_collision[y-nb_case_verif_y][x]or tab_collision[y][x])     //on verifi les collisions avec le sol
                {
                    m_vector.y =0;
                    m_floor_collision = true;
                }
            }
            //on change la position du perso en fonction de son ancienne position et de la force en x et en y modifiée

        }
        --nombre_division;
    }

     SetPosition(oldPos.x + m_vector.x * final_Time, oldPos.y + m_vector.y * final_Time);
};
//************************************************************************
//      Arret du perso au contact du sol
//************************************************************************
void Entite::Stopping_Frotement(float time)
{
    if(m_vector.x > 0)
    {
        m_vector.x -= time * frottement; //coef de frottements
        if(m_vector.x < 0)
            m_vector.x = 0;
    }
    else if(m_vector.x < 0)
    {
        m_vector.x += time * frottement;
        if(m_vector.x > 0)
            m_vector.x = 0;
    }
}


//************************************************************************
//      Ajout d'une force au vecteur d'accélération
//************************************************************************
void Entite::AddForce(Vector2f force)
{
    m_vector += force;
}

void Entite::SetDirection(_type_direction d)
{
    if (d<BAS)
        d=BAS;

    switch(d)
    {
        case BAS: SetAnim(&GoDown);
        break;

        case GAUCHE: SetAnim(&GoLeft);
        break;

        case DROITE: SetAnim(&GoRight);
        break;

        case HAUT: SetAnim(&GoUp);
        break;
        default: SetAnim(&GoLeft);
    }
    direction=d;
}


void Entite::loadAnim(Image &image, int x, int y, Anim *animRight, Anim *animLeft)
{
    int perso_image_taille_x=(image.GetWidth())/x;
    int perso_image_taille_y=(image.GetHeight())/y;

    for (int i=1;i<=x;++i)
    {
        animRight->PushFrame(Frame(&image,Rect<int>(perso_image_taille_x*(i-1), 0, perso_image_taille_x*i, perso_image_taille_y)));
        if(y>1)
        animLeft->PushFrame(Frame(&image,Rect<int>(perso_image_taille_x*(i-1),perso_image_taille_y*1, perso_image_taille_x*i, perso_image_taille_y*2)));
    }
}

 bool Entite::operator ^(Entite &e)
 {
   Vector2f Pos=this->GetPosition(),
            EPos=e.GetPosition(),
            size=this->GetSize(),
            Esize=e.GetSize();
            Esize.x/=2.3;
            Esize.y/=1.2;
            size.y/=1.2;
            size.x/=2.3;

    //int Portee=2*TAILLEBLOC;

    bool res=true;

    //verifiction droite gauche
    if (this->direction==DROITE) res=(((Pos.x + size.x) >= (EPos.x - Esize.x)) and  (EPos.x +0) >= Pos.x); //x en droit
    else res=(((Pos.x - size.x) <= (EPos.x + Esize.x)) and  (EPos.x) <= Pos.x); //en x gauche

    //verification hauteur
    res=res and  ((EPos.y >= (Pos.y - size.y)) and  ((EPos.y - Esize.y) <= (Pos.y)));


    return res;
};



