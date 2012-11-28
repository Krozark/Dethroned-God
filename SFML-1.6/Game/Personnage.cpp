#include "Personnage.hpp"
#include <string>


//************************************************************************
//      Initialisation du personnage
//************************************************************************
void Personnage::init()
{
  mouv=150;
  mouv_temp = mouv;
  dep_x=0;
  dep_y=0;
  dep_dir=0;
  dep_touche=1;
  touche_droite=3;
  touche_gauche=5;

  m_displacePerso=false;
  escape=false;

  //initialise les commandes
  SetDefaultCmd();
}


//************************************************************************
//      Actions du perso, events clavier
//************************************************************************
void Personnage::bouger_perso(RenderWindow* Window, float time)
{
  key_left=false;
  key_right=false;
  dep_dir=0;
  dep_x=0;
  dep_y=0;

  const sf::Input& Input = Window->GetInput();

  //echap
  if (Input.IsKeyDown(Key::Escape))
  {
      escape=true;
  }

  //verification de la direction
  //Gauche
  if (Input.IsKeyDown(Key::Left))
  {
      m_displacePerso = true;
      dep_dir+=touche_gauche;
  }

  //Droite
  if (Input.IsKeyDown(Key::Right))
  {
      m_displacePerso = true;
      dep_dir+=touche_droite;
  }

  //Saut
  if(Input.IsKeyDown(Key::Up))
  {
    if(getFloorCollision())
        AddForce(Vector2f(0,-500));
  }

  if(IsPaused())
    Play();

  switch (dep_dir)
    {
        //droite
        case 11:
        case 3: dep_x=mouv;dep_y=0;
            if (GetAnim() != &GoRight)
              SetAnim(&GoRight);
            if(getVector().x < 400)
            {
                if(getFloorCollision())
                {
                    AddForce(sf::Vector2f (time * 1000/3, 0));
                    if(getVector().x < 0)
                        AddForce(sf::Vector2f (time * 4000/3, 0));
                }
                else
                    AddForce(Vector2f (time * 500/3, 0));
            }
        break;

        //gauche
        case 13:
        case 5:dep_x=-mouv;dep_y=0;
            if (GetAnim() != &GoLeft)
              SetAnim(&GoLeft);
            if(getVector().x > -400)
            {
                if(getFloorCollision())
                {
                    AddForce(sf::Vector2f (-time * 1000/3, 0));
                    if(getVector().x > 0)
                        AddForce(sf::Vector2f (-time * 4000/3, 0));
                }
                else
                    AddForce(Vector2f (-time * 500/3, 0));
            }
        break;

        default: dep_x=0;dep_y=0;
              Pause();
        break;
    }

  anim(Window->GetFrameTime());
}

void Personnage::UpdateP(float time, std::vector<Floor> &floors)
{
    static int i=0;
    m_floor_collision = false;

    m_vector.y += time * 9.81 * NUMBER_PIXEL_PER_METER;

    Vector2f oldPos = GetPosition();

    m_floors_iter = floors.begin();
    while(m_floors_iter != floors.end())
    {
        if(GetPosition().x >= m_floors_iter->GetPosition().x
        && GetPosition().x <= m_floors_iter->GetPosition().x + m_floors_iter->GetSize()
        && oldPos.y                      <= m_floors_iter->GetPosition().y
        && oldPos.y + m_vector.y * time  >= m_floors_iter->GetPosition().y)
        {
            if(m_floors_iter->GetPosition().y < GetPosition().y && m_floor_collision || !m_floor_collision)
                SetPosition(GetPosition().x, m_floors_iter->GetPosition().y);

            m_vector.y = 0;
            m_floor_collision = true;
        }
        ++m_floors_iter;
    }


    SetPosition(GetPosition().x + m_vector.x * time,
                GetPosition().y + m_vector.y * time);
}

//************************************************************************
//      Touches par défaut
//************************************************************************
void Personnage::SetDefaultCmd()
{
  gauche.Key.Code = Key::Q;
  droite.Key.Code = Key::D;
  courrir.Key.Code = Key::Space;
  sauter.Key.Code = Key::Z;
  pause.Key.Code = Key::P;
}


//*********************************************************************************************************************

//************************************************************************
//      Getters & Setters
//************************************************************************
void Personnage::Set_mouv(int add)
{
  mouv =add;
}

int Personnage::Get_mouv()
{
  return mouv;
}

void Personnage::Set_mouv_temp(int add)
{
  mouv_temp =add;
}

int Personnage::Get_mouv_temp()
{
  return mouv_temp;
}

void Personnage::SetDisplacePerso(bool val)
{
    m_displacePerso=val;
}

bool Personnage::GetDisplacePerso()
{
    return m_displacePerso;
}

Personnage::~Personnage()
{

}

