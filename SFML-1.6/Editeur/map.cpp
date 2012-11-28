#include "map.hpp"

using namespace std;


MAP::MAP()
{
  tab_carte.resize(1);

}

void MAP::ajout_anim(Image &image,int taillex,int tailley,int color,unsigned int cx,unsigned int cy)
{
  OBJECT obj(image,taillex,tailley,color);
  obj.SetPosition(cx*taille_objets_pix,cy*taille_objets_pix);

  Charger_objet(obj);

}


void MAP::ajout_fix(Image &image,int x1, int y1,int x2, int y2,unsigned int cx,unsigned int cy)
{

  //caractérisique de l'objet créé
  OBJECT obj(image,x1,y1,x2,y2);
  obj.SetPosition(cx*taille_objets_pix,cy*taille_objets_pix);
  //chargement dans la map
  Charger_objet(obj);

}

//charger l'objet dans la cartes entiere
void MAP::Charger_objet(OBJECT &obj)
{
  tab_carte.push_back(obj);
}

