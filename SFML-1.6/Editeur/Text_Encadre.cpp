#include "Text_Encadre.hpp"
#include <iostream>

using namespace sf;
using namespace std;

TEXT_ENCADRE::TEXT_ENCADRE(const char* txt)
{
      text.SetText(txt);
      init();
};

void TEXT_ENCADRE::Draw(RenderTarget* App)
{
    if (string(text.GetText()) !="")
    {
        App->Draw(sprite);
        App->Draw(text);
    }
};

void TEXT_ENCADRE::SetSize(int size)
{
    text.SetSize(size);
    MAJ();
};

void TEXT_ENCADRE::SetText(const char * txt)
{
    text.SetText(txt);
    MAJ();
};

void TEXT_ENCADRE::SetPosition(float x,float y)
{
    SetPosition(Vector2f(x,y));
};

void TEXT_ENCADRE::SetPosition(sf::Vector2f pos)
{
    text.SetPosition(pos);
    sprite.SetPosition(pos);
};

void TEXT_ENCADRE::SetCentre(_POSITION POS)
{
     centre=POS;
     const FloatRect  Taille=text.GetRect();
     Vector2f TailleFond=sprite.GetSize();
     TailleFond.x/=Taille.GetWidth();
     TailleFond.y/=Taille.GetHeight();

     Vector2f pos(0,0),pos2(0,0);

     switch (centre)
     {
         case HAUT_DROIT:
            pos.y=2;
            pos.x=Taille.GetWidth();
            pos2.x=TailleFond.x;

            break;

         case HAUT_GAUCHE:
            pos.y=2;
            break;

         case BAS_DROIT:
            pos.y=Taille.GetHeight();
            pos.x=Taille.GetWidth();
            pos2.y=TailleFond.y;
            pos2.x=TailleFond.x;
            break;

         case BAS_GAUCHE:
            pos.y=Taille.GetHeight();
            pos2.y=TailleFond.y;

            break;
         default :
            pos.y=Taille.GetHeight()/1.8;
            pos.x=Taille.GetWidth()/2.05;
            pos2.y=TailleFond.y/2;
            pos2.x=TailleFond.x/2;
            break;
     }

    text.SetCenter(pos);
    sprite.SetCenter(pos2);
};

void TEXT_ENCADRE::MAJ()
{
    const FloatRect  Taille=text.GetRect();
    text.SetColor(couleur_Text);
    sprite.Resize(Taille.GetWidth(),Taille.GetHeight());
    sprite.SetColor(couleur_Fond);
    SetCentre(centre);
};

void TEXT_ENCADRE::init()
{
      couleur_Fond=Color(0, 0, 0, 255);
      couleur_Text=Color(255, 255, 255, 255);
      text.SetSize(20);
      SetCentre(HAUT_GAUCHE);
      sprite.SetColor(couleur_Fond);
      MAJ();
};

void TEXT_ENCADRE::operator+=(const char* a)
{
    SetText(string(string(text.GetText())+a).c_str());
};
