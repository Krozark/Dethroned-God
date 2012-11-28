#include "afficheur_sprite.hpp"

void AFFICHEUR_SPRITE::OnUpdate()
{
    Clear();

    spr.animation();

    Draw(spr);

    //Display();

};

void AFFICHEUR_SPRITE::Set_sprite(OBJECT s)
{
    spr=s;
    spr.Resize(size().width ()/4,size().height()/4);
    spr.SetPosition(size().width()/2-110,0);
    spr.SetFrameTime(0.3);
};


void AFFICHEUR_SPRITE::OnInit()
{

};
