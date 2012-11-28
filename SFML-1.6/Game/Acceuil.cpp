#include "Acceuil.hpp"

using namespace std;
using namespace sf;


//**************************************************************
//      Constructeur
//**************************************************************
Acceuil::Acceuil(RenderWindow* app)
{
    App=app;
    x=fenettre_taille_x;
    y=fenettre_taille_y;
    app->ShowMouseCursor(false);
    init();
}


//**************************************************************
//      Initialisation
//**************************************************************
void Acceuil::init()
{
    running=false;
    gamed=false;

    //sons buffert
    SB_Bouger.LoadFromFile("musique/002-System02.ogg");
    SB_Validation.LoadFromFile("musique/open3.ogg");
    //sons
    M_Bouger.SetBuffer(SB_Bouger); //son pour les touches haut et bas
    M_Validation.SetBuffer(SB_Validation); //son pour la touche entrée

    //musique
    musicMenu.OpenFromFile("musique/home.ogg");
    musicMenu.SetLoop(true);
    musicMenu.Play(); //on joue la musique dès l'ouverture

    //definition du curseur
    Image_Icon.LoadFromFile("image/Icone_Menu.png");        //image du curseur
    Sprite_Icon.SetImage(Image_Icon);   //Sprite
    Sprite_Icon.Resize(40,40);  //taille

    F_Font.LoadFromFile("fonts/FairyDustB.ttf");
    F_Font_Titre.LoadFromFile("fonts/BlackCastleMF.ttf");
    //chargement des variables de Acceuil
    Image_Fond.LoadFromFile("image/Acceuil.jpg");
    Sprite_Fond.SetImage(Image_Fond);
    Sprite_Fond.Resize(x,y);


    //nouvelle partie
    initial_position_menu(Unicode::Text("Nouvelle Partie"));
    //charger
    initial_position_menu(Unicode::Text("Charger une Partie"));
    //Quitter
    initial_position_menu(Unicode::Text("Quitter"));


    //animation sous le choix
    Image_Animation_Choix.LoadFromFile("image/animation2.png");
    Animation_Choix.charger_anim(Image_Animation_Choix,5,1,1);
    Animation_Choix.SetCenter(90,96);
    Move_Sprite_Icon(0);

    touche_haut.Key.Code=Key::Up; //touche pour le haut
    touche_bas.Key.Code=Key::Down; //pour le bas
    touche_valide.Key.Code=Key::Return; //pour valider
}


//**************************************************************
//      Initialise le texte du menu
//**************************************************************
void Acceuil::initial_position_menu(Unicode::Text text)
{
    int2 pos;
    String string;

    string.SetText(text); //texte
    string.SetFont(F_Font); //plice
    string.SetSize(40); //taille police
    string.SetCenter(string.GetRect().Right/2,string.GetRect().Top-string.GetRect().Bottom); //centre
    string.Move(X()/2,Y()/10*(tab_emplacement_boutons.size()+3)); //emplacement
    string.SetColor(Color(0,255,255)); //couleur

    pos.x=(int)string.GetRect().Left;
    pos.y=((int)string.GetRect().Top+(int)string.GetRect().Bottom)/2;

    tab_emplacement_boutons.push_back(pos);             //coordonées
    tab_String.push_back(string); //ajout dans tab string
}



//**************************************************************
//      Actualisation
//**************************************************************
void Acceuil::OnUpdate()
{
    if (IsRunning())
    {
        Run();
        if (App->GetEvent(*event)) {
            if (event->Type == Event::KeyPressed)
                clavier_action();
            if (event->Type == Event::MouseMoved)
                mouse_move();
            if (event->Type == Event::MouseButtonPressed)
                mouse_action();
        }
    }
    else if (IsGamed())
    {
        Run_Game();

        while (App->GetEvent(*event)) {
            if (event->Type == Event::KeyPressed)
                game->clavier_action(event);
            if (event->Type == Event::MouseMoved)
                game->mouse_move(event);
            if (event->Type == Event::MouseButtonPressed)
                game->mouse_action(event);
        }


        //on teste si on doit arrêter le jeu
        if (game->GetStopped())
            Stop_Game();
    }
}


//**************************************************************
//      Lance le menu d'acceuil
//**************************************************************
 void Acceuil::Run()
 {
     running=true;
     gamed=false;
     App->Clear();
     Afficher();
     App->Display();
 }


//**************************************************************
//      Lance le jeu
//**************************************************************
 void Acceuil::Click_Run_Game()
 {
     //avant de lancer le jeu, on stoppe la musique
     musicMenu.Stop();

     //on créer une nouvelle partie
    game=new Game(App);

    Run_Game();
 }


//**************************************************************
//      Maintient le jeu allumé
//**************************************************************
 void Acceuil::Run_Game()
 {

   running=false;
   gamed=true;

   game->On();
 }


//**************************************************************
//      Quitte le jeu
//**************************************************************
 void Acceuil::Stop_Game()
 {
     running=true;
     gamed=false;

     game->Off();
     delete game;

     //on remet la musique
     musicMenu.Play();
 }


//**************************************************************
//      Affichage du menu
//**************************************************************
 void Acceuil::Afficher()
 {
     //image du font
    App->Draw(Sprite_Fond);

    //animation sous le texte
    if (Animation_Choix.IsPaused())
    Animation_Choix.Play();

    Animation_Choix.anim(App->GetFrameTime());
    App->Draw(Animation_Choix);

    //texte acceuil
    App->Draw(String_Titre);

    //texte des boutons
    for (unsigned int i=0;i<tab_String.size();i++)
    App->Draw(tab_String[i]);

    //curseur
    Sprite_Icon.SetPosition(mousX,mousY);
    App->Draw(Sprite_Icon);
 }


//**************************************************************
//      Traitement des évènements clavier
//**************************************************************
 void Acceuil::clavier_action()
 {
     //event clavier
    if (event->Key.Code == Key::Up)
    {
        Move_Sprite_Icon(Get_Choix()-1);
    }
    else if (event->Key.Code ==Key::Down)
    {
        Move_Sprite_Icon(Get_Choix()+1);
    }
    else  if (event->Key.Code == Key::Return)
    {
        traitement_choix();
    }
 }


//**************************************************************
//      Traitement des mouvements de la souris
//**************************************************************
 void Acceuil::mouse_move()
 {
     mousX=event->MouseMove.X;
     mousY=event->MouseMove.Y;

    //on compare la position de la sourie aux positions des menus
    for (int i=0;i<3;i++)
    {

        if ((mousY > tab_String[i].GetRect().Top) && (mousY < tab_String[i].GetRect().Bottom)) //collisions en hauteur
            Move_Sprite_Icon(i);
    }
 }

//**************************************************************
//      Traitement des clics de la souris
//**************************************************************
void Acceuil::mouse_action()
{
    //on compare la position de la sourie aux positions des menus
    for (int i=0;i<3;i++)
    {
        if ((mousY > tab_String[i].GetRect().Top) && (mousY < tab_String[i].GetRect().Bottom)) //collisions en hauteur
        {
            if (event->MouseButton.Button == Mouse::Left)
                traitement_choix();
        }
    }
}


//**************************************************************
//      Traitement du choix des menus
//**************************************************************
void Acceuil::traitement_choix()
{
    M_Validation.Play();
    //si nouvelle partie
    if (Get_Choix()==0)
        Click_Run_Game();
    //si sur l'emplacemet de Quitter
    if (Get_Choix()==tab_String.size()-1)
        App->Close();
}


//**************************************************************
//      Bouge l'icône de selection de choix menu
//**************************************************************
void Acceuil::Move_Sprite_Icon(int choix)
{
  unsigned int choixActuel=Choix;

  if (choix==-1)
    Choix=tab_String.size()-1;
  else
    Choix= choix % tab_String.size(); // calcule de déplacement avec le nombre de choix possible

 Animation_Choix.SetPosition(fenettre_taille_x/2,tab_emplacement_boutons[Choix].y+60); //deplacement de l'animation sous le texte

 Sprite_Icon.SetPosition(tab_emplacement_boutons[Choix].x-10,tab_emplacement_boutons[Choix].y); //deplacement du curseur

 if (choixActuel!=Choix)
    M_Bouger.Play();

}


//**************************************************************
//      Met le menu en route / en pause
//**************************************************************
void Acceuil::mettre_menu()
{
    running=!running;
    gamed=!running;

}


Acceuil::~Acceuil()
{
}

