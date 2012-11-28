#ifndef TEXT_ENCADRE_HPP
#define TEXT_ENCADRE_HPP

#include <SFML/Graphics.hpp>

class TEXT_ENCADRE
{
  public:

     enum _POSITION{HAUT_DROIT=0,HAUT_GAUCHE,BAS_DROIT,BAS_GAUCHE,CENTRE};  //enumeration permetant de positioner le centre du texte

     TEXT_ENCADRE(const char* txt="");  //constructeur on l'on spécifie le texte ) afficher

     void Draw(sf::RenderTarget* App); //methode à appeler pour afficher l'objet

     void SetText(const char * txt=""); //permet de definir le texte à afficher

     void SetPosition(float x,float y); //permet de modifier la position d'affihage
     void SetPosition(sf::Vector2f pos);    // idem

     void SetCentre(_POSITION pos=CENTRE); //permet de redéfinir le centre d'affichage de l'objet
     void SetSize(int size=10); //permet de modifier la taille du texte
     void SetTextColor(sf::Color color=sf::Color(255, 255, 255, 255)) { couleur_Text=color; MAJ();}; // permet de modifier lacouleur du texte
     void SetFondColor(sf::Color color=sf::Color(0, 0, 0, 255)) {couleur_Fond=color;MAJ();};//permet de modifier la couleur de fond

     void operator+=(const char*); //permet de concaténer au texe existant la chaine entréé en parametre
     void operator=(const char* txt) {SetText(txt);};   //permet d'associer le texte à afficher (idem que setText)

  private:
      sf::String text;  //texte qui est affiché
      sf::Sprite sprite;    // sprite de fond
      sf::Color couleur_Fond,couleur_Text; // couleur du fond et du text
      _POSITION centre; //position du centre relatif à l'objet
      void MAJ();   //apelé pour mettre à jour l'affichage lors de la mdification d'un atribut
      void init();  //ce qui est fait dans le constructeur
};
#endif
