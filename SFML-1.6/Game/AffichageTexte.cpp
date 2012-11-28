#include "AffichageTexte.hpp"

using namespace sf;
using namespace std;


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
AffichageTexte::AffichageTexte(RenderWindow* app)
{
    this->App = app;
    line = 1;
    line_width = 45;
    special_chars = "{#}[<@|";

    font.LoadFromFile("./fonts/BlackCastleMF.ttf");
    loadImages();

    DisplayedString.SetFont(font);
    DisplayedString.SetSize(20);
};


inline void AffichageTexte::loadImages()
{
    imgFond.LoadFromFile("./resources/GUI/fondDial.png");
    sprFond.SetImage(imgFond);
    sprFond.Resize(App->GetView().GetRect().GetWidth()-10, 100);

    imgCursor.LoadFromFile("./resources/GUI/cursor.png");
    sprCursor.SetImage(imgCursor);

    imgFondHead.LoadFromFile("./resources/GUI/fondDialPortrait.png");
    sprFondHead.SetImage(imgFondHead);

    sprHead = NULL;
}

void AffichageTexte::SetHeadImage(Image &image)
{
    sprHead->SetImage(image);
}

////////////////////////////////////////////////////////////
/// Draw a string on a the screen letter by letter
////////////////////////////////////////////////////////////
void AffichageTexte::ShowText(string s, bool msg_box)
{
    bool new_line = false;
    int cpt = 0;
    suite = false;
    int taille=0;
    int pos_x=15;

    //Draw the message box
    if (msg_box)
    {
        App->Draw(sprFond);
        if (sprHead != NULL)
        {
            //Draw the head of the speaking character
            App->Draw(sprFondHead);
            App->Draw(*sprHead);
        }
    }

    for (unsigned int i = 0 ; i < s.length() ; ++i)
    {
        switch (s[i])
        {
            case '\n':
            case '#' : //Go to the next line
            {
                ++line;
                new_line = true;
            }
            case '}' : //Change the colour to white
            {
                DisplayedString.SetColor(Color::White);
                break;
            }
            case '[' : //Change the colour to blue
            {
                DisplayedString.SetColor(Color::Blue);
                break;
            }
            case '<' : //Change the colour to yellow
            {
                DisplayedString.SetColor(Color::Yellow);
                break;
            }
            case '{' : //Change the colour to red
            {
                DisplayedString.SetColor(Color::Red);
                break;
            }
            case '@' : //Change the colour to green
            {
                DisplayedString.SetColor(Color::Green);
                break;
            }
            case '|' : //Wait a few moments
            {
                sf::Sleep(0.5);
                break;
            }
            case ' ' : //Check wether there is enough space in the current line to put the next word
            {
                //int _k = s.length() - 1;
                spaces = 1;
                string mot_suivant="";
                for (int j = 1 ; s[i+j]!=' ' ; ++j)
                {
                    if (special_chars.find(s[i+j]) >= special_chars.size()) //si il y a un caractere spécial
                    {
                        mot_suivant+=s[i+j];
                    }
                }
                String futur_display(mot_suivant);
                if (pos_x + futur_display.GetRect().GetWidth() > line_width)
                {
                    ++line;
                    new_line = true;
                    break;
                }
            }
            default :
            {
                //Draw the character on the screen
                taille=DisplayedString.GetRect().GetWidth();
                DisplayedString.SetText(s.substr(i,1));
                DisplayedString.SetPosition(x+pos_x+taille, y+(line-1)*sprFond.GetSize().y/4+12);
                App->Draw(DisplayedString);
                pos_x+=taille;
                App->Display();
                sf::Sleep(0.01);
                ++cpt;
            }
        }

        //If the system has to start a new line
        if (new_line)
        {
            cpt = 0;
            taille=0;
            pos_x=0;

            if (line < 4)
            {
                //Create a new string made with the end of the precedent line and draw it
                string temp_s = s.substr(i+1, s.length() - i);
                ShowText(temp_s, false);
            }
            else
            {
                line = 1;
                //Wait until the user presses a key
                nextString = s.substr(i+1, s.length() - i);
                suite = true;
            }
            i = s.length();
        }
    }

    App->Draw(sprCursor);
    line = 1;
}

void AffichageTexte::SetResize(float coef_x,float coef_y)
{
    line_width=coef_x-15;
    sprFond.Resize(coef_x,imgFond.GetHeight()*coef_y);
    //sprCursor.Resize(imgCursor.GetWidth()*coef_x,imgCursor.GetHeight()*coef_y);
    sprFondHead.Resize(imgFondHead.GetWidth()*coef_x,imgFondHead.GetHeight()*coef_y);
};

////////////////////////////////////////////////////////////
/// Set the message box and head box positions
////////////////////////////////////////////////////////////
void AffichageTexte::SetPosition(float x, float y, bool haut)
{
    this->x = x;
    this->y = y;
    sprFond.SetPosition(x, y);
    sprCursor.SetPosition(x+sprFond.GetSize().x- sprCursor.GetSize().x - 12 , y+sprFond.GetSize().y- sprCursor.GetSize().y - 12);

    sprFondHead.SetX(sprFond.GetPosition().x);
    sprFondHead.SetY((haut)?sprFond.GetPosition().y+sprFond.GetSize().y:sprFond.GetPosition().y-sprFond.GetSize().y);

    if (sprHead != NULL)
        sprHead->SetPosition(sprFondHead.GetPosition());
}
