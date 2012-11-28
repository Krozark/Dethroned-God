#include "Parchemin.hpp"

using namespace std;
using namespace sf;

Parchemin::Parchemin(sf::Image *fond, Sons* son, _touches_heros* touchesHeros, _touches_sys* touches_Sys, vector<_skills>* skills)
{
    this->son = son;
    touches = touchesHeros;
    touchesSys = touches_Sys;

    //variables
    currentParchemin=0;
    tailleChar = 18;

    //polices
    font.LoadFromFile("./fonts/BlackCastleMF.ttf");

    //initialisation des images
    initImages(fond);

    nbCharParLigne = spr_parchemin.GetSize().x/((tailleChar * FENETREX/800.0)*0.65);

    int _size_skills=skills->size();
    for (int cpt=0; cpt<_size_skills; cpt++)
    {
        //texte du skill
        texteSkill[cpt]=convertText((*skills)[cpt].description);

        //titre du skill
        titreSkill[cpt]=convertText((*skills)[cpt].nom);

        //image du skill
        imageSkill[cpt].LoadFromFile((*skills)[cpt].img);
        spriteSkill[cpt].SetImage(imageSkill[cpt]);
    }

    //initialisation des textes
    initText();
};

void Parchemin::initText()
{
    //quêtes
    quete.SetText("Quete");
    quete.SetFont(font);
    quete.SetSize((tailleChar+5) * FENETREX/800.0);
    quete.SetStyle(4);
    quete.SetColor(Color(0,0,0));
    quete.SetPosition(Vector2f(150*FENETREX/800.0,120*FENETREY/600.0));

    //texte de quette
    txt.SetFont(font);
    txt.SetSize(tailleChar * FENETREX/800.0);
    txt.SetColor(Color(0,0,0));
    txt.SetPosition(Vector2f(quete.GetPosition().x, quete.GetPosition().y+quete.GetRect().GetHeight()+20));

    //skills
    skills.SetText("Skills");
    skills.SetFont(font);
    skills.SetSize((tailleChar+5) * FENETREX/800.0);
    skills.SetStyle(4);
    skills.SetColor(Color(0,0,0));
    skillsTitre.SetFont(font);
    skillsTitre.SetSize(tailleChar * FENETREX/800.0);
    skillsTitre.SetStyle(4);
    skillsTitre.SetColor(Color(0,0,0));
    skillsContain.SetFont(font);
    skillsContain.SetSize(tailleChar * FENETREX/800.0);
    skillsContain.SetColor(Color(0,0,0));
}

void Parchemin::initImages(Image *fond)
{
    //initialisation de l'image de fond
    img_parchemin.LoadFromFile("./resources/quetes/parchemin.png");
    spr_parchemin.SetImage(img_parchemin);
    spr_parchemin.Resize(FENETREX-150,FENETREY-25);
    spr_parchemin.SetPosition(75,25);

    //fond de la map en cours
    spr_fond.SetImage(*fond);
    spr_fond.Resize(FENETREX,FENETREY);

    //parchemin précédent
    img_previous.LoadFromFile("./resources/quetes/previous.png");
    spr_previous.SetImage(img_previous);
    spr_previous.SetPosition(spr_parchemin.GetSize().x/2.0,spr_parchemin.GetPosition().y+(15*FENETREY/600.0));

    //texte haut
    txtHaut.SetPosition(spr_previous.GetPosition().x+40,spr_previous.GetPosition().y);
    txtHaut.SetFont(font);
    txtHaut.SetColor(Color(0,0,0));

    //parchemin suivant
    img_next.LoadFromFile("./resources/quetes/next.png");
    spr_next.SetImage(img_next);
    spr_next.SetPosition(txtHaut.GetPosition().x + 90, spr_previous.GetPosition().y);
}

void Parchemin::maj(std::vector<Objet> &liste, sf::Image *Screen)
{
    //fond
    img_fond = *Screen;
    spr_fond.SetImage(img_fond);
    spr_fond.Resize(FENETREX,FENETREY);

    //listeParchemins = liste;
    texteQuete.clear();
    idSkills.clear();

    //on remplit les dialogues correspondant aux texteQuete des parchemins
    for (unsigned int i=0; i<liste.size(); ++i)
    {
        texteQuete.push_back(convertText(liste[i].GetDialogue()));
        idSkills.push_back(liste[i].getIdSkill());
    }

    //parchemin courrant
    currentParchemin=liste.size()-1;

    //postionnement du texte
    positionneText();
}


void Parchemin::display(RenderTarget& app)
{
    //affichage de l'image de fond
    app.Draw(spr_fond);
    app.Draw(spr_parchemin);

    //affichage du titre et des flèches
    app.Draw(spr_previous);
    app.Draw(spr_next);

    //affichage du texte
    app.Draw(txtHaut);
    app.Draw(quete);
    app.Draw(txt);
    if (idSkills[currentParchemin]>0)
    {
        app.Draw(skills);
        app.Draw(skillsTitre);
        app.Draw(spriteSkill[idSkills[currentParchemin]-1]);
        app.Draw(skillsContain);
    }
}


void Parchemin::positionneText()
{
    stringstream out;
    out << currentParchemin+1 << " / " << texteQuete.size();
    txtHaut.SetText(out.str());

    //texte de quete
    txt.SetText(texteQuete[currentParchemin]);

    /*
     * SKILLS
    */
    int id = idSkills[currentParchemin]; //id du skill rattaché au parchemin
    if (id>0)
    {
        id--;

        //titre de la partie
        skills.SetPosition(txt.GetPosition().x, txt.GetPosition().y+txt.GetRect().GetHeight()+20);

        //image du skin
        spriteSkill[id].SetPosition(skills.GetPosition().x,skills.GetPosition().y + skills.GetRect().GetHeight()+15);
        spriteSkill[id].Resize(32,32);

        //titre du skin
        skillsTitre.SetPosition(spriteSkill[id].GetPosition().x + spriteSkill[id].GetSize().y + 20, spriteSkill[id].GetPosition().y);
        skillsTitre.SetText(titreSkill[id]);

        //texte du skin
        skillsContain.SetText(texteSkill[id]);
        skillsContain.SetPosition(Vector2f(skills.GetPosition().x, spriteSkill[id].GetPosition().y+spriteSkill[id].GetSize().y+10));
    }
}

void Parchemin::getEvents(Event event)
{
    if (event.Key.Code == Key::Left) changeCurrentParchemin(PARCH_GAUCHE);
    else if (event.Key.Code == Key::Right) changeCurrentParchemin(PARCH_DROITE);
}

void Parchemin::changeCurrentParchemin(_dir_change_ dir)
{
    if (dir==PARCH_GAUCHE)
        --currentParchemin;
    else
        ++currentParchemin;

    if (currentParchemin<=-1)
        currentParchemin=texteQuete.size()-1;
    else
        currentParchemin=currentParchemin%texteQuete.size();

    son->playSon(MENUBOUGER);
    positionneText();
}

string Parchemin::convertText(string res,bool appel)
{
    size_t cur=0;

    while ((cur=res.find("\\t"))<res.size() )
        res.replace(cur,2,"   ");
    while ((cur=res.find("\\n"))<res.size() )
        res.replace(cur,2,"\n");

    /////////////////////////////////////////////////////////////////////////
    /// Touches définies par l'utilisateur à afficher dans les parchemins
    /////////////////////////////////////////////////////////////////////////
    //saut
    while ((cur=res.find("\\jum")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touches->sauter) + "\""));
    //frappe
    while ((cur=res.find("\\hit")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touches->taper) + "\""));
    //courrir
    while ((cur=res.find("\\run")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touches->courrir) + "\""));
    //menu
    while ((cur=res.find("\\men")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touches->pause) + "\""));
    //retour
    while ((cur=res.find("\\esc")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touchesSys->retour) + "\""));
    //ramasser - utiliser
    while ((cur=res.find("\\use")-1)<res.size())
        res.replace(cur,5,string(" \"" + findKey(touches->ramasse) + "\""));

    //si on doit faire des retours à la ligne
    if (res.size() > nbCharParLigne and appel)
        insertRetourLigne(&res);

    return res;
}

inline void Parchemin::insertRetourLigne(string *res)
{
    const unsigned int taille = res->size();
    unsigned int nbLettres = 0;
    unsigned int nbCharEcrisParLigne = 0;
    unsigned int curseur = 0;

    while (curseur < taille) //pour tous les mots...
    {
        //on va à la fin du mot courant
        unsigned int i=curseur;
        nbLettres=0; //va contenir le nombre de lettres du mot

        while (((*res)[i]!=' ' and (*res)[i]!='\n') and i<taille) //parcours du mot
        {
            ++nbLettres; //compter le nombre de lettres
            ++i;
        }

        if ((*res)[i]=='\n' and nbLettres>=1) //si il y a un retour à la ligne spécifié dans l'éditeur
        {
            nbCharEcrisParLigne=0;
        }

        if (i>=taille) //sécurité : en cas de fin de chaîne, on arrête
            break;

        nbCharEcrisParLigne+=nbLettres; //on rajoute les lettres du mot au nombre de caracères écrits dans la ligne

        if (nbCharEcrisParLigne >= nbCharParLigne) //si on a dépassé la longueur de la ligne
        { //on force un retour à la ligne
            (*res)[curseur-1]='\n';
            nbCharEcrisParLigne=0;
        }
        else //sinon on continue, on passe au mot suivant
            curseur += nbLettres+1;
    }
};



std::string Parchemin::findKey(Key::Code &e)
{
    std::string touche;

    switch (e)
    {
        case Key::A :
            touche = "A";
            break;

        case Key::B :
            touche = "B";
            break;

        case Key::C :
            touche = "C";
            break;

        case Key::D :
            touche = "D";
            break;

        case Key::E :
            touche = "E";
            break;

        case Key::F :
            touche = "F";
            break;

        case Key::G :
            touche = "G";
            break;

        case Key::H :
            touche = "H";
            break;

        case Key::I :
            touche = "I";
            break;

        case Key::J :
            touche = "J";
            break;

        case Key::K :
            touche = "K";
            break;

        case Key::L :
            touche = "L";
            break;

        case Key::M :
            touche = "M";
            break;

        case Key::N :
            touche = "N";
            break;

        case Key::O :
            touche = "O";
            break;

        case Key::P :
            touche = "P";
            break;

        case Key::Q :
            touche = "Q";
            break;

        case Key::R :
            touche = "R";
            break;

        case Key::S :
            touche = "S";
            break;

        case Key::T :
            touche = "T";
            break;

        case Key::U :
            touche = "U";
            break;

        case Key::V :
            touche = "V";
            break;

        case Key::W :
            touche = "W";
            break;

        case Key::X :
            touche = "X";
            break;

        case Key::Y :
            touche = "Y";
            break;

        case Key::Z :
            touche = "Z";
            break;

        case Key::Num0 :
            touche = "0";
            break;

        case Key::Num1 :
            touche = "1";
            break;

        case Key::Num2 :
            touche = "2";
            break;

        case Key::Num3 :
            touche = "3";
            break;

        case Key::Num4 :
            touche = "4";
            break;

        case Key::Num5 :
            touche = "5";
            break;

        case Key::Num6 :
            touche = "6";
            break;

        case Key::Num7 :
            touche = "7";
            break;

        case Key::Num8 :
            touche = "8";
            break;

        case Key::Num9 :
            touche = "9";
            break;

        case Key::Escape :
            touche = "Echap";
            break;

        case Key::LControl :
            touche = "Ctrl gauche";
            break;

        case Key::LShift :
            touche = "Shift gauche";
            break;

        case Key::LAlt :
            touche = "Alt gauche";
            break;

        case Key::LSystem :
            touche = "System gauche";
            break;

        case Key::RControl :
            touche = "Ctrl droite";
            break;

        case Key::RShift :
            touche = "Shift droite";
            break;

        case Key::RAlt :
            touche = "Alt gr";
            break;

        case Key::RSystem :
            touche = "System droite";
            break;

        case Key::Menu :
            touche = "Menu";
            break;

        case Key::LBracket :
            touche = "Bracket gauche";
            break;

        case Key::RBracket :
            touche = "Bracket droite";
            break;

        case Key::SemiColon :
            touche = "SemiColon";
            break;

        case Key::Comma :
            touche = "Virgule";
            break;

        case Key::Period :
            touche = "Period";
            break;

        case Key::Quote :
            touche = "\"";
            break;

        case Key::Slash :
            touche = "/";
            break;

        case Key::BackSlash :
            touche = "\\";
            break;

        case Key::Tilde :
            touche = "~";
            break;

        case Key::Equal :
            touche = "=";
            break;

        case Key::Dash :
            touche = "Dash";
            break;

        case Key::Space :
            touche = "Espace";
            break;

        case Key::Return :
            touche = "Entree";
            break;

        case Key::Back :
            touche = "retour chariot";
            break;

        case Key::Tab :
            touche = "Tab";
            break;

        case Key::PageUp :
            touche = "Page Up";
            break;

        case Key::PageDown :
            touche = "Page Down";
            break;

        case Key::End :
            touche = "Fin";
            break;

        case Key::Home :
            touche = "Home";
            break;

        case Key::Insert :
            touche = "Inserer";
            break;

        case Key::Delete :
            touche = "Delete";
            break;

        case Key::Add :
            touche = "+";
            break;

        case Key::Subtract :
            touche = "-";
            break;

        case Key::Multiply :
            touche = "*";
            break;

        case Key::Divide :
            touche = "/";
            break;

        case Key::Left :
            touche = "Gauche";
            break;

        case Key::Right :
            touche = "Droite";
            break;

        case Key::Up :
            touche = "Haut";
            break;

        case Key::Down :
            touche = "Bas";
            break;

        case Key::Numpad0 :
            touche = "0";
            break;

        case Key::Numpad1 :
            touche = "1";
            break;

        case Key::Numpad2 :
            touche = "2";
            break;

        case Key::Numpad3 :
            touche = "3";
            break;

        case Key::Numpad4 :
            touche = "4";
            break;

        case Key::Numpad5 :
            touche = "5";
            break;

        case Key::Numpad6 :
            touche = "6";
            break;

        case Key::Numpad7 :
            touche = "7";
            break;

        case Key::Numpad8 :
            touche = "8";
            break;

        case Key::Numpad9 :
            touche = "9";
            break;

        case Key::F1 :
            touche = "F1";
            break;

        case Key::F2 :
            touche = "F2";
            break;

        case Key::F3 :
            touche = "F3";
            break;

        case Key::F4 :
            touche = "F4";
            break;

        case Key::F5 :
            touche = "F5";
            break;

        case Key::F6 :
            touche = "F6";
            break;

        case Key::F7 :
            touche = "F7";
            break;

        case Key::F8 :
            touche = "F8";
            break;

        case Key::F9 :
            touche = "F9";
            break;

        case Key::F10 :
            touche = "F10";
            break;

        case Key::F11 :
            touche = "F11";
            break;

        case Key::F12 :
            touche = "F12";
            break;

        case Key::F13 :
            touche = "F13";
            break;

        case Key::F14 :
            touche = "F14";
            break;

        case Key::F15 :
            touche = "F15";
            break;

        case Key::Pause :
            touche = "Pause";
            break;

        default :
            touche = "Inconnu";
            break;
    }

    return touche;
};
