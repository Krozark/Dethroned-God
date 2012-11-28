#ifndef DG_AFFICHAGETEXTE_HPP
#define DG_AFFICHAGETEXTE_HPP

#include <iostream>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Animated.hpp"
#include "Sons.hpp"

class AffichageTexte
{
    public:

        ////////////////////////////////////////////////////////////
        /// Default constructor
        ///
        /// \param app : window in which the string will be displayed
        ///
        ////////////////////////////////////////////////////////////
        AffichageTexte(sf::RenderWindow *app);

        ////////////////////////////////////////////////////////////
        /// Draw a string on the screen letter by letter
        ///
        /// \param s : string to draw
        /// \param msg_box : draw the message box if true (true by default)
        ///
        ////////////////////////////////////////////////////////////
        void ShowText(std::string s, bool msg_box = true);

        ////////////////////////////////////////////////////////////
        /// Set the message box and head box positions
        ///
        /// \param x : x position to draw the message box
        /// \param y : y position to draw the message box
        ///
        ////////////////////////////////////////////////////////////
        void SetPosition(float x, float y, bool haut);

        ////////////////////////////////////////////////////////////
        ///
        /// \param image : image of the talking character
        ///
        ////////////////////////////////////////////////////////////
        void SetHeadImage(sf::Image &image);

        void SetResize(float coaf_x ,float coef_y);

        std::string getTextFollowing() { return nextString; };
        bool getSuite() { return suite; };


    private:
        inline void loadImages();

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        sf::RenderWindow*   App;

        sf::String          DisplayedString;

        std::string nextString,
                    special_chars;

        sf::Image   imgFond,
                    imgFondHead,
                    imgCursor;

        sf::Sprite  sprFond,
                    sprFondHead,
                    *sprHead,
                    sprCursor;

        sf::Font font;

        Sons son;

        int     line,
                line_width,
                spaces;

        bool    suite;

        float   x, y;
};

#endif // DG_AFFICHAGETEXTE_HPP
