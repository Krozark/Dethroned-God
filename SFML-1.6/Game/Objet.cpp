#include "Objet.hpp"

using namespace std;
using namespace sf;

/*Objet::Objet(Image& skin, int frameX, int frameY, _type_direction dirDepart, Vector2f pos, _type_objets type) : Entite_non_jouable(skin, frameX, frameY, dirDepart, pos)
{
    SetLoop(true);
    Play();

    typeObjet=type;
}*/


Objet::Objet(Image& skin,_OBJ_ &obj) : Entite_non_jouable(skin,obj.tx,obj.ty,obj.direction,Vector2f(obj.x,obj.y+32))
{
    quette=obj.quette;
    typeObjet=obj.type;
    dialogue = obj.dialogue;
    idSkill = obj.idSkill;
    if (typeObjet==TELEPORTEUR)
        destination=Vector2f(obj.destx,obj.desty);
    else
    {
        destination.x=obj.destx;
        destination.y=obj.desty;
    }
    //Resize(obj.resize,obj.resize);

    //objets spéciaux
    if (obj.type <= PIEGE) {
        loadOneAnim(skin,obj.ty,obj.direction);
    }

    SetLoop(true);
    Play();
};

void Objet::Update(float time,int nombre_division,bool tab_collision[MAPSIZECARTEY][MAPSIZECARTEX])
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
        --nombre_division;
    }

    if (m_floor_collision) //si le perso touche/vient de toucher le sol
        Stopping_Frotement(time);

     SetPosition(oldPos.x + m_vector.x * final_Time, oldPos.y + m_vector.y * final_Time);
};

void Objet::loadOneAnim(Image &image,int nbLignes,int y,int x)
{
    int perso_image_taille_x=(image.GetWidth())/x;
    int perso_image_taille_y=(image.GetHeight())/nbLignes;
    GoLeft.clear();

   for (int i=0;i<x;++i)
    {
        GoLeft.PushFrame(Frame(&image,Rect<int>(perso_image_taille_x*i, perso_image_taille_y*y, perso_image_taille_x*(i+1), perso_image_taille_y*(y+1))));
    }
   //GoDown.PushFrame(Frame(&image,Rect<int>(0, 0, image.GetWidth(), image.GetHeight())));

    SetAnim(&GoLeft);
}

 bool Objet::operator ^(Entite &e)
 {
   Vector2f Pos=this->GetPosition(),
            EPos=e.GetPosition();
    return ((Pos.x - TAILLEBLOC/1.75 <= EPos.x)  and  (Pos.x + TAILLEBLOC/1.75 >= EPos.x)  and  ((Pos.y - TAILLEBLOC <= EPos.y)  and  (Pos.y + TAILLEBLOC >= EPos.y)));
};
