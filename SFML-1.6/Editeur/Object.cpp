#include "Object.hpp"

OBJECT::OBJECT()
{

}

OBJECT::OBJECT(Image &image,int x, int y,int z, int w)
{
  rectangle(image,x,y,z,w);
}

OBJECT::OBJECT(Image &image,int x,int y, int color)
{
  charger_anim(image,x,y,color);
}

void OBJECT::rectangle(Image &image,int x, int y,int z, int w)
{
  SetImage(image);
  image_taille_x=image.GetWidth();
  image_taille_y=image.GetHeight();
  object_image_taille_x=image_taille_x/taille_objets_pix;
  object_image_taille_y=image_taille_y/taille_objets_pix;

  IntRect a(x*taille_objets_pix,y*taille_objets_pix,z*taille_objets_pix,w*taille_objets_pix);
  SetLoop(false);
  SetSubRect(a);
}



void OBJECT::charger_anim(Image &image,int x,int y, int color)
{
  charger_image_sprite(image,x,y);

  //détection de la couleur
  switch (color)
    {
    case 1:
      SetAnim(&GoDown);
      break;
    case 2:
      SetAnim(&GoLeft);
      break;
    case 3:
      SetAnim(&GoRight);
      break;
    case 4:
      SetAnim(&GoUp);
      break;
    default:
      SetAnim(&GoDown);
      break;
    }
  Pause();
  SetLoop(true);
  SetFrameTime(0.2f);
}


void OBJECT::charger_image_sprite(Image &image,int x,int y)
{
    GoDown.clear();
    GoLeft.clear();
    GoRight.clear();
    GoUp.clear();

  image_taille_x=image.GetWidth();
  image_taille_y=image.GetHeight();
  object_image_taille_x=image_taille_x/x;
  object_image_taille_y=image_taille_y/y;

  //initialisation des animations de marche


  for (int i=1;i<=x;i++)
    {
      GoDown.PushFrame(Frame(&image,sf::Rect<int>(object_image_taille_x*(i-1),object_image_taille_y*0, object_image_taille_x*i, object_image_taille_y*1)));
      if (y>1)
	GoLeft.PushFrame(Frame(&image,Rect<int>(object_image_taille_x*(i-1),object_image_taille_y*1, object_image_taille_x*i, object_image_taille_y*2)));
      if (y>2)
	GoRight.PushFrame(Frame(&image,Rect<int>(object_image_taille_x*(i-1),object_image_taille_y*2, object_image_taille_x*i, object_image_taille_y*3)));
      if (y>3)
	GoUp.PushFrame(Frame(&image,Rect<int>(object_image_taille_x*(i-1),object_image_taille_y*3, object_image_taille_x*i, object_image_taille_y*4)));
      else
	y=4;
    }
}

void OBJECT::animation()
{
  if(IsPaused())
    Play();

    // On appelle la fonction d'animation a chaque tours en donnant le temps écoulé
    anim(0.1f);

}



OBJECT::~OBJECT()
{

}
