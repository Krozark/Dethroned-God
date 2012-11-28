#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "WindowGame.hpp"
#include "Taille_fenetre.hpp"
#include "Global_taille_fenetre.hpp"

int FENETREX, FENETREY;

using namespace std;
using namespace sf;

int main()
{
    //ouverture des options
    _options settings;
    _touches_heros touchesHeros;
    _touches_sys touchesSys;

    //lecture dans le fichier
    FILE* file = NULL;
    file = fopen("./data/settings","rb");
    if (file != NULL)
    {
        fseek(file,0,SEEK_END);
        int taille = ftell(file);
        fseek(file,0,SEEK_SET);

        if (taille>0)
        {
            //options
            fread(&settings,sizeof(_options),1,file);

            //touches héros
            fread(&touchesHeros,sizeof(_touches_heros),1,file);

            //touches sys
            fread(&touchesSys,sizeof(_touches_sys),1,file);
        }

        fclose(file);
    }

    istringstream in(settings.resolutionX);
    in >> FENETREX;
    istringstream in2(settings.resolutionY);
    in2 >> FENETREY;

    // Création de la fenêtre principale
    RenderWindow *App;
    if (settings.fullscreen)
        App = new RenderWindow(VideoMode(FENETREX, FENETREY, 32), "Dethroned God", Style::Fullscreen);
    else
        App = new RenderWindow(VideoMode(FENETREX, FENETREY, 32), "Dethroned God");
    App->SetFramerateLimit(settings.fps);
    App->UseVerticalSync(settings.verticalSync);
    App->ShowMouseCursor(false);

    //Déclaration de la classe maîtresse du jeu
    WindowGame window(App,&settings,&touchesHeros,&touchesSys);

    // Start game loop
    while (App->IsOpened())
    {
        Event Event;
        while (App->GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed) {
                App->Close();
            }

            if (Event.Type == Event::KeyPressed || Event.Type == Event::TextEntered)
            {
                window.eventKey(Event);
            }
        }

        window.run();
    }

    return EXIT_SUCCESS;
}
