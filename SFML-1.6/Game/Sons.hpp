#ifndef SONS_HPP_INCLUDED
#define SONS_HPP_INCLUDED

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "Taille_fenetre.hpp"

class Sons
{
    public :
        Sons(_options* settings);
        Sons();
        void playSon(int son);
        void updateVolumeSons(_options *settings);

    private :
        inline void loadSon(std::string path);
        void initSons();

        std::vector<sf::SoundBuffer> tab_buffers;
        std::vector<sf::Sound> tab_sons;
        sf::SoundBuffer buffer;
        sf::Sound son;
        _options* settings;
};

#endif

