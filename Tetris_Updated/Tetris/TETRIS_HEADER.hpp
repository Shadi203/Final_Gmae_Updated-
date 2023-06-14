#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define MAX_NUMBER_OF_ITEMS 2
using namespace std;

struct Point{
int x,y;
};
bool check();

class Menu
{
public:
    Menu(float width, float height);
    ~Menu();

    void draw(sf::RenderWindow &window);
    void MoveUp();
    void MoveDown();
    int GetPressedItem() { return selectedItemIndex; }

private:
    int selectedItemIndex;
    sf::Font font;
    sf::Text menu[MAX_NUMBER_OF_ITEMS];

};
class Background : public sf::Sprite{
protected:
    sf::Texture texture_;
    sf::Sprite sprite_;
public:
    Background(string file);
    void Draw_Background(sf::RenderWindow &window);
};
vector<unique_ptr<Background>> Make_vector(Background &ob1, Background &ob2);


void Game();
