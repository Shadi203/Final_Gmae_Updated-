#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TETRIS_HEADER.hpp>

using namespace std;

int main()
{

    sf::RenderWindow window(sf::VideoMode(600.0, 400.0), "Tetris");
    sf::Texture texture_background;
    if(!texture_background.loadFromFile("textures/Background.jpg")) {
        std::cout << "ERROR" << std::endl;
        return 1; };
    sf::Sprite background;
            texture_background.setRepeated(true);
            background.setTexture(texture_background);
            background.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));
            background.setScale(static_cast<float>(window.getSize().x) / texture_background.getSize().x, static_cast<float>(window.getSize().y) / texture_background.getSize().y);

    sf::Music music;
    if(!music.openFromFile("start_game.ogg"))
    {
        std::cout << "ERROR" << std::endl;
    }
    music.play();

    Menu menu(window.getSize().x, window.getSize().y);
    while (window.isOpen()){
        sf::Event menu_case;

        while (window.pollEvent(menu_case)){
            switch (menu_case.type)
            {
            case sf::Event::KeyReleased:
                switch (menu_case.key.code){
                case sf::Keyboard::Up:
                    menu.MoveUp();
                    break;

                case sf::Keyboard::Down:
                    menu.MoveDown();
                    break;

                case sf::Keyboard::Return:
                    switch (menu.GetPressedItem())
                    {
                    case 0:
                        Game();
                        break;
                    case 1:
                        window.close();
                        break;
                    }

                    break;
                }
                break;
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

        window.clear();

        window.draw(background);

        menu.draw(window);

        window.display();
    }
};
