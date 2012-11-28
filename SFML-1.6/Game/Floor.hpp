#ifndef FLOOR_HPP_INCLUDED
#define FLOOR_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Taille_fenetre.hpp"

#define HAUTEUR 5

class Floor : public sf::Sprite
{

    public:
        Floor(sf::Vector2f pos=sf::Vector2f(0,0), float size=HAUTEUR,sf::Color color=sf::Color::White,float rotate=0)
        {

            m_size=size;
            Resize(m_size,HAUTEUR);
            SetPosition(pos);
            if(rotate)
                SetRotation (rotate);
            SetColor(color);

        }

        float GetSize() {return m_size;}
        void AgrandirDroit(float size) {m_size+=size;Resize(m_size,HAUTEUR);};
        void AgrandirGauche(float size) {SetX(GetPosition().x-size);m_size+=size;Resize(m_size,HAUTEUR);};
        void AgrandirHaut(float size){SetY(GetPosition().y-size);m_size+=size;Resize(m_size,HAUTEUR);};
        void AgrandirBas(float size) {m_size+=size;Resize(m_size,HAUTEUR);};
        bool CouvrePosition(sf::Vector2f pos);


    private:
        float       m_size;
};

/*class Floor : public Shape
{

    public:
        Floor(Vector2f pos, float size) //:  Shape::Line(pos.x,pos.y,pos.x+size,pos.y,5,Color::Blue)
        {
            m_size=size;
            AddPoint(pos.x,pos.y,Color::Blue,Color::Red);
            AddPoint(pos.x+size,pos.y,Color::Blue,Color::Red);

            //m_image.Create(m_size,16,Color(0,160,0));
            //SetImage(m_image);
            //SetPosition(pos);
        }

        float GetSize() {return m_size;}

    private:
        float       m_size;
        //Image       m_image;
};*/

#endif
