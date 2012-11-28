#include "Menu.hpp"

using namespace std;
using namespace sf;

Menu::Menu(RenderWindow* Window, _options* setting, Sons* so, _touches_sys* touchesSy)
{
    App = Window;
    settings = setting;
    son = so;
    touchesSys = touchesSy;

    init();
}

//*****************************************
//  Initialisation des composants
//*****************************************
void Menu::init()
{
    launchGameAllowed = false;
    launchLoadAllowed = false;

    //musique
    musicMenu.OpenFromFile("musique/home.ogg");
    musicMenu.SetVolume(settings->volumeMusiqueMenus);
    musicMenu.Stop();

    /*
     * Chargements des images de choix
     */
    //curseur
    cursor.LoadFromFile("images/Icone_Menu.png");
    spr_cursor.SetImage(cursor);
    spr_cursor.Rotate(-135);
    spr_cursor.Resize(40*FENETREX/800.0,40*FENETREY/600.0);

    //titre
    title.LoadFromFile("images/menu/titre.png");
    spr_title.SetImage(title);
    spr_title.SetCenter(spr_title.GetSize().x/2.0,0);
    spr_title.Resize(spr_title.GetSize().x/1.5 * FENETREX/800.0, spr_title.GetSize().y/1.5 * FENETREY/600.0);
    spr_title.SetPosition(FENETREX/2.0,0);

    //nouveau
    newGame.LoadFromFile("images/menu/new.png");
    spr_new.SetImage(newGame);
    spr_new.SetCenter(spr_new.GetSize().x/2,spr_new.GetSize().y/2);
    spr_new.Resize(spr_new.GetSize().x/1.5 * FENETREX/800.0, spr_new.GetSize().y/1.5 * FENETREY/600.0);
    spr_new.SetPosition(FENETREX/2.0,spr_title.GetPosition().y+spr_title.GetSize().y);
    tab_choice.push_back(spr_new);

    //charger
    load.LoadFromFile("images/menu/load.png");
    spr_load.SetImage(load);
    spr_load.SetCenter(spr_load.GetSize().x/2,spr_load.GetSize().y/2);
    spr_load.Resize(spr_load.GetSize().x/1.5 * FENETREX/800.0, spr_load.GetSize().y/1.5 * FENETREY/600.0);
    spr_load.SetPosition(FENETREX/2.0,spr_new.GetPosition().y+spr_new.GetSize().y/1.2);
    tab_choice.push_back(spr_load);

    //exit
    exit.LoadFromFile("images/menu/exit.png");
    spr_exit.SetImage(exit);
    spr_exit.SetCenter(spr_exit.GetSize().x/2,spr_exit.GetSize().y/2);
    spr_exit.Resize(spr_exit.GetSize().x/1.5 * FENETREX/800.0, spr_exit.GetSize().y/1.5 * FENETREY/600.0);
    spr_exit.SetPosition(FENETREX/2.0,spr_load.GetPosition().y+spr_load.GetSize().y/1.2);
    tab_choice.push_back(spr_exit);

    //fond
    background.LoadFromFile("./resources/Backgrounds/Background.jpg");
    spr_background.SetImage(background);
    spr_background.Resize(FENETREX,FENETREY);

    moveSpriteIcon(0);
}


//*****************************************
//  Bouge l'icone de selection
//*****************************************
void Menu::moveSpriteIcon(int choix)
{
    int choixActuel=Choix;

    if (choix==-1)
        Choix=tab_choice.size()-1;
    else
        Choix=choix % tab_choice.size(); // calcule de déplacement avec le nombre de choix possible

    //animChoix->SetPosition(FENETREX/2,tab_choice[Choix].GetPosition().y+300); //deplacement de l'animation sous le texte

    spr_cursor.SetPosition(tab_choice[Choix].GetPosition().x-(tab_choice[Choix].GetSize().x/2-50),tab_choice[Choix].GetPosition().y); //deplacement du curseur

    if (choixActuel!=Choix)
        son->playSon(MENUBOUGER);
}


//*****************************************
//  Traitement du choix
//*****************************************
void Menu::traitementChoix()
{
    son->playSon(MENUVALIDER);
    if (Get_Choix() == 0)
    {
        musicMenu.Stop();
        launchGameAllowed=true;
    }
    else if (Get_Choix() == 1)
    {
        musicMenu.Stop();
        launchLoadAllowed=true;
    }
    else if (Get_Choix() == tab_choice.size()-1)
        App->Close();
}


//*****************************************
//  Mise à jour
//*****************************************
void Menu::run()
{
    if (musicMenu.GetStatus() == 0  and  !launchGameAllowed) //si la musique est stoppée
        musicMenu.Play();

    //affichage
    display();
}


//*****************************************
//  Affichage
//*****************************************
void Menu::display()
{
    //fond
    App->Draw(spr_background);

    //titre
    App->Draw(spr_title);

    //animation
//    App->Draw(*animChoix);
    //animChoix->anim(App->GetFrameTime());

    //choix
    for (unsigned int i=0; i<tab_choice.size(); ++i)
        App->Draw(tab_choice[i]);

    //curseur
    App->Draw(spr_cursor);
}


//*****************************************
//  Event
//*****************************************
void Menu::eventKey(Event event2)
{
    if (event2.Key.Code == touchesSys->haut)
        moveSpriteIcon(Get_Choix()-1);
    else if (event2.Key.Code == touchesSys->bas)
        moveSpriteIcon(Get_Choix()+1);
    else if (event2.Key.Code == touchesSys->entree)
        traitementChoix();
}
