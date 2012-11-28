#include "Pnj.hpp"
#include <sstream>

using namespace std;
using namespace sf;

/*Pnj::Pnj(Image& skin, int frameX, int frameY, _type_direction dirDepart, Vector2f pos, int x1, int x2, int speed) : Entite_non_jouable(skin, frameX, frameY, dirDepart, pos)
{
    min = x1;
    max = x2;
    vue = 5*TAILLEBLOC;
    viemax=200;
    vie=200;
    this->speed=speed;
    possede_anim_tapper=false;
    direction=dirDepart;

    SetLoop(true);
    Play();

    chargementAnimations();

}*/


Pnj::Pnj(Image &image,_PNJ_ &pnj) : Entite_non_jouable(image,pnj.tx,pnj.ty,pnj.direction,Vector2f(pnj.x,pnj.y+32))
{
    min=min_origin=pnj.x-pnj.min*TAILLEBLOC;
    max=max_origin=pnj.x+pnj.max*TAILLEBLOC;
    vue=pnj.vue*TAILLEBLOC;
    Resize(pnj.resize/10.0*image.GetWidth()/pnj.tx,pnj.resize/10.0*image.GetHeight()/pnj.ty);
    enable=true;
    agressif=pnj.agressif;
    speed=pnj.vitesse;//pnj.vitesse;
    degats=pnj.degats;
    viemax=pnj.vie;
    vie=pnj.vie;
    element=pnj.element;
    quette=pnj.quette;
    dialogue=pnj.dialogue;
    exp=pnj.exp;
    boss=pnj.boss;
    lvl=pnj.lvl;
    chargementAnimations();
    SetLoop(true);
    volant=pnj.volant;
    berzerk_force=3+boss;

    possede_anim_tapper= (string(pnj.nom).find("_T_")< string(pnj.nom).size());


    Etat=ETAT_RIEN;

    Play();
    m_bloque=m_bloque_last=false;

    clock.SetTime((350/((speed>0)? speed:116)));
    temps_agro.SetTime(lvl+boss*3);

    if (speed> 350)
        speed=350;

    speedMin = speed;
    speedMax = speed + 100;

    puissance_saut=speed;
    nb_case_saut=((vector_saut.y/TAILLEBLOC)*9.80)-0.85;


    clock.Stop();
    temps_agro.Stop();
};

void Pnj::Update(float time,int nombre_division, bool tab_collision[MAPSIZECARTEY][MAPSIZECARTEX])
{
        m_floor_collision = false;
        m_bloque_last=m_bloque;
        m_bloque=false;
        saut=BAS;

        Vector2f oldPos = GetPosition();
        float final_Time=0.0;
        Change_puissance_saut_max();

        static float Recharge_vie=0;

       // cout<<"puissance: "<<puissance_saut<<" vector: "<<vector_saut.y<<endl;

        while (nombre_division > 0 and !m_floor_collision)  //on subdivise pour verifier tous les blocs
        {
             final_Time+=time;
             m_vector.y += (m_vector.y <600)? time *gravitee*96 : 0;

            int nb_case_verif_y= (m_vector.y<0)? m_vector.y/(TAILLEBLOC+96):0;

            const int y=(oldPos.y + m_vector.y * time)/TAILLEBLOC;     //case future du perso
            const int x=(oldPos.x + m_vector.x * time)/TAILLEBLOC;

            if (x >0 and y >0 and x<MAPSIZECARTEX and y<MAPSIZECARTEY)
            {
                //bas milieu
                if (m_vector.y>0)       //si le perso retombe
                {
                    if (tab_collision[y-nb_case_verif_y][x]or tab_collision[y][x])     //on verifi les collisions avec le sol
                    {
                        m_vector.y =0;
                        m_floor_collision = true;
                    }
                }

                int dir= (m_vector.x < 0)? -1:1;

                nb_case_saut=((vector_saut.y/TAILLEBLOC)*time *gravitee)-1;


                if (tab_collision[y-1][x+dir])//verif droite gauche collision
                {
                    m_vector.x=0;
                    int Y=y+nb_case_saut;    // nombre de case en heuteur

                    for (int i= y;i>=Y and saut!=HAUT;i--)
                    {
                        if (!tab_collision[i][x+dir]) //si il n'y a pas de colision en hauteur
                        {
                            Change_puissance_saut((y-i)*2*TAILLEBLOC); //on gegle la puissance du saut
                            saut=HAUT;  //en peu sotter
                        }

                    }
                        m_bloque=saut != HAUT;

                }
                bool peu_sauter=false;
                if (!tab_collision[y][x+dir] and saut != HAUT) //si il y a un trou
                {
                    int y_min=y+nb_case_saut,
                        y_max=y -nb_case_saut,
                        x_max=x+dir*(-nb_case_saut)+1;

                    if (y_max >MAPSIZECARTEY-3)
                        y_max=MAPSIZECARTEY-3;

                    if (y_min<1)
                        y_min=1;

                    if (x_max < 1)
                        x_max= 1;
                    else if (x_max>MAPSIZECARTEX)
                        x_max=MAPSIZECARTEX;


                   for(int _y=y_max;_y>=y_min and !peu_sauter; _y--) //on verifi les trou si on peu les sauter
                    {
                        if (dir >0) // droite
                        {
                            for(int _x=x_max; (_x>x) and (!peu_sauter);_x--)
                            {
                                if ( (!tab_collision[_y][_x]) and tab_collision[_y+1][_x])
                                {

                                    if (_y<=y)
                                    {
                                        Change_puissance_saut(((_x-x+1)*2 + (y-_y+1)*2)*TAILLEBLOC);
                                        saut=HAUT;

                                    }
                                    else
                                    {
                                        if (_x - x > 3)
                                        {
                                            Change_puissance_saut(((_x-x -3))*TAILLEBLOC);
                                            saut=HAUT;
                                        }
                                    }

                                    peu_sauter=true;
                                }
                            }
                        }
                        else//gauche
                        {
                            for(int _x=x_max-1; (_x<=x) and (!peu_sauter);_x++)
                            {
                                if ( (!tab_collision[_y][_x]) and tab_collision[_y+1][_x])
                                {
                                    if (_y<=y)
                                    {

                                        Change_puissance_saut(((x-_x+1)*2 + (y-_y+1)*2)*TAILLEBLOC);
                                        saut=HAUT;
                                    }
                                    else
                                    {
                                       //cout<<_y-y<<endl;
                                       if (x - _x > 3)
                                        {
                                            Change_puissance_saut(((x-_x -3)*2)*TAILLEBLOC);
                                            saut=HAUT;
                                        }

                                    }
                                    peu_sauter=true;

                                }
                            }
                        }
                    }

                    if (peu_sauter)
                    {
                         m_bloque=false;
                    }
                    else //si on ne saute pas et qu'il y a un trou, on ragarde sa profondeure
                    {
                        Toggle();
                        saut=BAS;
                        m_bloque=false;
                    }
                }
            }
            //on change la position du perso en fonction de son ancienne position et de la force en x et en y modifiée
            --nombre_division;
        }

        Recharge_vie+=final_Time;
        if (Recharge_vie>1)
        {
            Recharge_vie-=1;
            vie+=(lvl/1.5);
            if (vie>viemax)
                vie = viemax;
        }

         SetPosition(oldPos.x + m_vector.x * final_Time, oldPos.y + m_vector.y * final_Time);

         if(GetPosition().y>=MAPSIZEY-TAILLEBLOC-10)    //si on va plus bas que la map
         {
            if (!boss)
                modifieVie(-vie,true);
            else
            {
                SetPosition(Position_Enemie);
                clock.Replay();
                Etat=ETAT_FRAPPE;
            }
         }
};
//**************************************************************
//      Bouger les mobs
//**************************************************************
void Pnj::bouger_mob(RenderWindow* Window,float time,int nombre_division,bool tab_collision[MAPSIZECARTEY][MAPSIZECARTEX])
{
    Update(time,nombre_division,tab_collision);


    if (saut == HAUT) // si le perso dois sauter
    {
         Sauter(); //on le fait sauter
    }
    else if ((m_bloque and m_bloque_last) or (Etat==ETAT_RIEN and (((GetPosition().x< min_origin) and (direction== GAUCHE)) or ((GetPosition().x>max_origin) and (direction == DROITE))))) //ou pas trouvé le hero et que l'on est au bornes ou que le perso est bloqué
    {
        Toggle();   //on se retourne*/
        m_bloque=false;
    }


    if(direction==GAUCHE)
    {
        if(getForce().x > -speedMax)
        {
            if(getFloorCollision() and  getForce().x > -speedMin)
                m_vector.x = -speedMin;
            else if (getFloorCollision())
                AddForce(sf::Vector2f (-time * speed*2, 0));
            else
                AddForce(sf::Vector2f (-time * speed, 0));
        }
        else
            m_vector.x = -speedMax;
    }
    else if (direction==DROITE)
    {
            if(getForce().x < speedMax)
            {
                if (getFloorCollision() and  getForce().x < speedMin)
                    m_vector.x = speedMin;
                else if(getFloorCollision())
                    AddForce(sf::Vector2f (time * speed*2, 0));
                else
                    AddForce(sf::Vector2f (time * speed, 0));
            }
            else
                m_vector.x = speedMax;
    }


    anim(Window->GetFrameTime());
};

void Pnj::trouveEntite(Entite& E)
{
    if(! agressif)
        return;
    Position_Enemie =E.GetPosition();
    Vector2f Pos=GetPosition();

    //taille du monstre
   Vector2f _Size=GetSize();
    _Size.x/=1.7;
    _Size.y/=1.7;

   const int dist=TAILLEBLOC*3; //distance en plus a ajouter a celle de E

   bool Enemi_a_droite = ((direction==DROITE)and (Pos.x +vue + _Size.x >= Position_Enemie.x - _Size.x) and (Position_Enemie.x >= Pos.x - _Size.x)),
        Enemi_a_gauche = ((direction==GAUCHE)and (Pos.x -vue - _Size.x <= Position_Enemie.x + _Size.x) and (Position_Enemie.x <= Pos.x + _Size.x)),
        max_bas=(Pos.y + _Size.y >= Position_Enemie.y ),
        min_haut=(Pos.y -_Size.y <= Position_Enemie.y);

   if ((Etat==ETAT_RIEN or Etat==ETAT_GOTO) and agressif)
    {
        if ((min_haut)and (max_bas) //si l'enemie est entre un max et mini en hauteur
                 and ((Enemi_a_droite)  //si il est en vision droite
                 or (Enemi_a_gauche))) //si il est en vision vers la gauche
        {
                    Etat=ETAT_GOTO; //il faut alors aller vers lui
        }
        else
            Etat=ETAT_RIEN;
    }

    if (Etat==ETAT_GOTO)
    {
        if (Enemi_a_droite)
        {
            max=Position_Enemie.x;
            min=Pos.x;

            if (m_bloque and m_bloque_last and saut!=HAUT)
            {
                direction=GAUCHE;
                Etat=ETAT_RIEN;
            }
        }
        else if (Enemi_a_gauche)
        {
            max=Pos.x;
            min=Position_Enemie.x;

            if (m_bloque and m_bloque_last and saut!=HAUT)
            {
                direction=DROITE;
                Etat=ETAT_RIEN;
            }
        }
        else //if (m_floor_collision)
            Toggle();

        if (agressif)
            if ((*this)^E)  //il y a un enemie que l'on peu tapper a proximiter
            {
                temps_agro.Replay();
                Etat=ETAT_STOP;
            }
    }
    else if (Etat==ETAT_RIEN)
    {
        min=min_origin;
        max=max_origin;
    }
    else if (Etat==ETAT_FUI)
    {
        if (direction == DROITE)
        {
            min=Position_Enemie.x;
            max=Position_Enemie.x + dist;
            m_vector.x+=speed/2;
        }
        else
        {
            min=Position_Enemie.x - dist;
            max=Position_Enemie.x;
            m_vector.x-=speed/2;
        }
    }
    else if (Etat==ETAT_IS_FRAPPE)
    {
        if ((Enemi_a_droite) or (Enemi_a_gauche))
        {
            Etat=ETAT_GOTO;
        }
        else
        {
           Toggle();
            Etat=ETAT_GOTO;
        }
    }
    else if (Etat== ETAT_FRAPPE)
    {
        if (direction == DROITE)
        {
            m_vector.x=speed/6.0;
        }
        else
        {
            m_vector.x=-speed/6.0;
        }
    }
    else if (m_bloque and m_bloque_last)
        Etat=ETAT_RIEN;

    if (clock.GetRemainingTime()==-1)
    {
        if (Etat==ETAT_STOP)   //on peut taper
         {
                Etat=ETAT_FRAPPE;
                if (possede_anim_tapper)
                {
                    if (direction == DROITE)
                        SetAnim(&GoUp);
                    else
                        SetAnim(&GoDown);
                }

                if (Randomizer::Random(1,100) <= 80 + boss*10)
                {
                     int d=degats + Randomizer::Random(-degats/3,degats/3);
                    if (d<-1) d=-1;

                    E.modifieVie(-d);
                }
                else
                {
                    String txt("Miss");
                    txt.SetColor(Color(180,180,180));
                    txt.SetPosition(GetPosition().x,GetPosition().y-2*(GetSize().y)/3);
                    bonusMalus.push_back(txt);
                    bonusMalusPosDepartY.push_back(GetPosition().y);
                }

                clock.Replay();
         }
         else if(Etat!=ETAT_FUI)
            Etat=ETAT_GOTO;
    }

    if(max-min <= dist)  //au cas ou la distance serais trop petite pour marcher
    {
        max+=dist;
        min-=dist;
    }
    if (temps_agro.GetRemainingTime()>-1)   //si le monstre à encore l'agro su l'enemi on regarde ou est l'enemi par rapport à la direction dans laquelle on avence, et si elle ne conviennent pas, on se retourne
    {
        if ((direction == DROITE and !Enemi_a_droite) or (direction == GAUCHE and !Enemi_a_gauche))
            Toggle();
    }

};


void Pnj::modifieVie(int nb,bool critique,bool epicFail)
{
    bool affiche_degats = (nb!=1 and nb!=0);
    if (nb<0 && !critique)
    {
        nb+=lvl*(2.5f+0.5*boss);   //on calcule la defence
        if (nb>-1) //on verifie pour ne pas gagner de vie
            nb=-1;
    }
    vie+= nb;
    if (vie<=0)
    {
        vie=0;
        mustDead=true;
    }
    else if (vie>viemax) vie=viemax;

    if (affiche_degats)
    {
        Etat=ETAT_IS_FRAPPE;
        agressif=true;

        stringstream out;
        String txt;

        //bonus
        if (nb>0)
        {
            txt.SetColor(Color(0,255,0));
        }
        else //malux
        {
            if (critique)
                txt.SetColor(Color(255,0,0));
            else if (epicFail)
                txt.SetColor(Color(255,255,0));
            else
                txt.SetColor(Color(255,120,0));
        }
        out << nb;

        txt.SetText(out.str());
        txt.SetPosition(GetPosition().x,GetPosition().y-2*(GetSize().y)/3);
        txt.SetSize(35);
        bonusMalus.push_back(txt);
        bonusMalusPosDepartY.push_back(GetPosition().y);

        if (100*vie/viemax <=15) //teste de la fuite du monstre
        {
            if ((Randomizer::Random(1,100) <=(50-15*boss) ) or (nb > vie*2))
            {
                Etat=ETAT_FUI;
            }
            else if (boss)
            {
                ++berzerk_force;
                degats*=1.1;
            }
            else
                ++berzerk_force;

        }
    }
};


bool Pnj::operator ^(Entite &e)
{
    bool res=(Entite)(*this)^e;
    float add=degats*berzerk_force;
    if(res)
    {
        e.AddForce(Vector2f(-add,(direction == GAUCHE)? -add:add));
        e. SetColor(Color(255,50,50,190));
    }
    return res;

};

void Pnj::SetAgressif(bool v)
{
    agressif = v;
    if (!agressif)
    {
        Etat = ETAT_RIEN;
        temps_agro.Stop();
    }
}

