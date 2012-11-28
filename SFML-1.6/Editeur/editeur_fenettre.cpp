#include "editeur_fenettre.hpp"
#include <sstream>

using namespace std;
using namespace sf;

void EDITEUR_FENETTRE::mousePressEvent ( QMouseEvent * event )
{
    int x=(event->pos().x()/(2*taille_objets_pix));
    int y=((event->pos().y()-taille_objets_pix)/(2*taille_objets_pix));

    unsigned int c = y*(size().width()/(taille_objets_pix*2))+x;

    if (c <resources_sprite.size() && curent_case!=c )
    {
        passed_case=curent_case;
        curent_case=c;
        S_Selection.SetPosition(x*2*taille_objets_pix,y*2*taille_objets_pix+taille_objets_pix);
    }

}

void EDITEUR_FENETTRE::OnInit()
{
     ChargerImages();
}

void EDITEUR_FENETTRE::OnUpdate()
{
    Clear();
    for (unsigned int i=0;i<resources_sprite.size();i++)
    {
        Draw(resources_sprite[i]);
    }
    Draw(S_Selection);
    //Display();
}

void EDITEUR_FENETTRE::init()
{
    ligne=0;
    B_Choix_ligne=new QComboBox(this);
    for (int i=0;i<y;i++)
    {
        std::ostringstream oss;
        oss <<"Ligne "<<i+1;
        B_Choix_ligne->addItem(oss.str().c_str());
    }
    B_Choix_ligne->move(0,0);


    I_Selection.LoadFromFile("./resources/image/selection.png");
    S_Selection.SetImage(I_Selection);
    S_Selection.Resize(taille_objets_pix*2,taille_objets_pix*2);
    S_Selection.SetPosition(0,taille_objets_pix);

    B_Type=new QComboBox(this);
    B_Type->addItem("Potion Vie");
    B_Type->addItem("Potion Mana");
    B_Type->addItem("Coffre");
    B_Type->addItem("Parchemin");
    B_Type->addItem("Sauvegarde");
    B_Type->addItem("Sauvgarde Fix");
    B_Type->addItem("Teleporteur");
    B_Type->addItem("Or");
    B_Type->addItem("Trempoling");
    B_Type->addItem("Accelerateur");
    B_Type->addItem("Piege");
    B_Type->addItem("Portail");
    B_Type->addItem("Marchand");
    B_Type->addItem("Quette");
    B_Type->addItem("Rien");
    B_Type->addItem("Telep.Quette");
    QObject::connect(B_Type,SIGNAL(currentIndexChanged(int)),this,SLOT(changeType(int)));
    B_Type->move(100,0);

    B_lvl=new QSpinBox(this);
    B_lvl->setRange(1,60);
    B_lvl->move(250,0);
    B_lvl->setValue(1);
    QObject::connect(B_lvl,SIGNAL(valueChanged(int)),this,SLOT(changeLvl(int)));


    ChargerImages();

    curent_case=0;
    passed_case=99;

}

void EDITEUR_FENETTRE::ChargerSprites()
{
    for (unsigned int i=0;i<resources_sprite.size();i++)
    {
        int image_taille_x=resources_image[i].GetWidth()/x;
        int image_taille_y=resources_image[i].GetHeight()/y;

        resources_sprite[i].SetSubRect(IntRect(image_taille_x,image_taille_y*ligne,image_taille_x*2,image_taille_y*ligne+image_taille_y));
    }
}

void EDITEUR_FENETTRE::ChargerImages()
{
    resources_image_nom.clear();
    resources_image.clear();
    resources_sprite.clear();

    resources_image_nom=resources.lister(dossier.c_str());

    int maxx=size().width()/(2*taille_objets_pix);

    for (unsigned int i=0;i<resources_image_nom.size();i++)
    {
        Image img;
        img.LoadFromFile( string(dossier+"/"+resources_image_nom[i]).c_str());

        resources_image.push_back(img);

        Sprite spr(resources_image[i],Vector2f((i%maxx)*(2*taille_objets_pix),(int(i/maxx))*(2*taille_objets_pix)+taille_objets_pix));

        int image_taille_x=img.GetWidth()/x;
        int image_taille_y=img.GetHeight()/y;

        spr.SetSubRect(IntRect(image_taille_x,image_taille_y*ligne,image_taille_x*2,image_taille_y*ligne+image_taille_y));
        spr.Resize(taille_objets_pix*2,taille_objets_pix*2);

        resources_sprite.push_back(spr);
    }


}
int EDITEUR_FENETTRE::add_image(std::string s,std::string dos,int y,int ligne)
{
    bool ok=false;
    unsigned int i=0;

    if (y<=0)
        y=this->y;
    if(ligne<0)
        ligne=this->ligne;




    while (i<resources_image_nom.size() && !ok )
    {
        if (dossier=="." || dossier==".")
        {
             if (resources_image_nom[i]==dossier+s)
            {
                ok=true;
            }
            else
                i++;
        }
        else
        {
            if (resources_image_nom[i]==dos+"/"+s)
                ok=true;
            else
                i++;
        }

    }

    if(!ok)
    {
        Image img;

        if (dossier=="." || dossier==".")
        {
            resources_image_nom.push_back(dossier+"/"+s);
            img.LoadFromFile( string(dossier+"/"+s).c_str());
        }
        else
        {
            (resources_image_nom.push_back(dos+"/"+s));
            img.LoadFromFile( string(dos+"/"+s).c_str());
        }


        resources_image.push_back(img);

        Sprite spr(resources_image[i],Vector2f((i%10)*(2*taille_objets_pix),(int(i/10))*(2*taille_objets_pix)+taille_objets_pix));

        int image_taille_x=img.GetWidth()/x;
        int image_taille_y=img.GetHeight()/y;

        spr.SetSubRect(IntRect(image_taille_x,image_taille_y*ligne,image_taille_x*2,image_taille_y*ligne+image_taille_y));
        spr.Resize(taille_objets_pix*2,taille_objets_pix*2);

        resources_sprite.push_back(spr);
    }

    return i;
};


void EDITEUR_FENETTRE::changeLvl(int l)
{
        emit LvlChanged(l);
};

void EDITEUR_FENETTRE::changeType(int t)
{
    emit TypeChanged( t);
};
