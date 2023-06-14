#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TETRIS_HEADER.hpp>


using namespace std;

const int M = 20; //height of the grid
const int N = 10; //width of the grid
int field[M][N] = {{0}}; //declaring grid
int figures[7][4] = // creating figures from a single tile
{
    {1,3,5,7},
    {2,4,5,7},
    {3,5,4,6},
    {3,5,4,7},
    {2,3,5,7},
    {3,5,7,6},
    {2,3,4,5},
};
Point a[4], b[4];

Menu::Menu(float width, float height)
{
    if (!font.loadFromFile("Modern.otf")){
    }

    menu[0].setFont(font);
    menu[0].setCharacterSize(60);
    menu[0].setColor(sf::Color::Red);
    menu[0].setString("Play");
    menu[0].setPosition(sf::Vector2f(width /2.65, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

    menu[1].setFont(font);
    menu[1].setCharacterSize(60);
    menu[1].setColor(sf::Color::White);
    menu[1].setString("Quit");
    menu[1].setPosition(sf::Vector2f(width/2.65, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

    selectedItemIndex = 0;
}
Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow &window)
{
    for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
    {
        window.draw(menu[i]);
    }
}

void Menu::MoveUp()
{
    if (selectedItemIndex - 1 >= 0)
    {
        menu[selectedItemIndex].setColor(sf::Color::White);
        selectedItemIndex--;
        menu[selectedItemIndex].setColor(sf::Color::Red);
    }
}

void Menu::MoveDown()
{
    if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
    {
        menu[selectedItemIndex].setColor(sf::Color::White);
        selectedItemIndex++;
        menu[selectedItemIndex].setColor(sf::Color::Red);
    }
}
Background::Background(string file){
    texture_.loadFromFile(file);
    sprite_.setTexture(texture_);
}

bool check()
{
   for (int i=0;i<4;i++)
      if (a[i].x<0 || a[i].x>=N || a[i].y>=M) return 0;
      else if (field[a[i].y][a[i].x]) return 0;

   return 1;
};

void Background::Draw_Background(sf::RenderWindow &window){
    window.draw(sprite_);
}
vector<unique_ptr<Background>> Make_vector(Background &ob1, Background &ob2){
    vector<unique_ptr<Background>>screen;
    auto s1 = make_unique<Background>(ob1);
    auto s2 = make_unique<Background>(ob2);
    screen.emplace_back(move(s1));
    screen.emplace_back(move(s2));
    return screen;
}

void Game(){


        sf::Music music;
        if(!music.openFromFile("game_music.ogg"))
        {
            std::cout << "ERROR" << std::endl;
        }
        music.setLoop(true);
        music.play();

        srand(time(0));

        sf::RenderWindow window(sf::VideoMode(320.0, 480.0), "Tetris");

        sf::Texture t1;
        t1.loadFromFile("textures/tiles.png");
        Background background("textures/background.png");
        Background frame("textures/frame.png");


        sf::Sprite s(t1);
        vector<unique_ptr<Background>> screen = Make_vector(background, frame);

        sf::Text Score;
        sf::Font font;
        font.loadFromFile("Modern.otf");
        Score.setFont(font);
        Score.setCharacterSize(30);
        Score.setColor(sf::Color::Black);
        Score.setPosition(sf::Vector2f(160, 400));

        int dx=0; bool rotate=0; int colorNum=1;
        float timer=0,delay=0.3;

        sf::Clock clock;
        sf::Clock time_elapsed;
        sf::Text Time_elapsed;
        Time_elapsed.setFont(font);
        Time_elapsed.setCharacterSize(30);
        Time_elapsed.setColor(sf::Color::Black);
        Time_elapsed.setPosition(sf::Vector2f(160, 435));
        int score = 0;

        bool game_over = false;
        while (window.isOpen())
        {
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer+=time;

            sf::Event e;
            while (window.pollEvent(e))
            {
                if (e.type == sf::Event::Closed)
                    window.close();

                if (e.type == sf::Event::KeyPressed)
                  if (e.key.code==sf::Keyboard::Up) rotate=true;
                  else if (e.key.code==sf::Keyboard::Left) dx=-1;
                  else if (e.key.code==sf::Keyboard::Right) dx=1;
            }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay=0.05;

        for (int i=0;i<4;i++)  { b[i]=a[i]; a[i].x+=dx; }
        if (!check()) for (int i=0;i<4;i++) a[i]=b[i];

        if (rotate)
          {
            Point p = a[1]; //center of rotation
            for (int i=0;i<4;i++)
              {
                int x = a[i].y-p.y;
                int y = a[i].x-p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
               }
               if (!check()) for (int i=0;i<4;i++) a[i]=b[i];
          }

        if (timer>delay)
          {
            for (int i=0;i<4;i++) { b[i]=a[i]; a[i].y+=1; }

            if (!check())
            {

             for (int i=0;i<4;i++) field[b[i].y][b[i].x]=colorNum;

             colorNum=1+rand()%7;
             int n = rand() % 7;
             for (int i = 0; i < 4; i++)
             {
                 a[i].x = figures[n][i] % 2;
                 a[i].y = figures[n][i] / 2;
             }
            }
            for (int i=0;i<4;i++){
                  if(field[a[i].y][a[i].x]) game_over=true;
                }

             timer=0;
          }
        int k=M-1;
        int completedLines = 0;
        for (int i = M - 1; i > 0; i--) {
            int count = 0;
            for (int j = 0; j < N; j++) {
                if (field[i][j]) count++;
                field[k][j] = field[i][j];
            }
            if (count == N) {
                // Line completed!
                completedLines++;
                // Increase the score
                score += 10;
            }
            if (count < N) k--;
        }


        dx=0; rotate=0; delay=0.3;

        window.clear(sf::Color::White);
        for(int i = 0; i < 2; i++){
            screen[i]->Draw_Background(window);
        }
        string points = "Score:" + to_string(score);
        Score.setString(points);
        int game_time = time_elapsed.getElapsedTime().asSeconds();
        string q = "Time:" + to_string(game_time);
        Time_elapsed.setString(q);

        for (int i=0;i<M;i++)
         for (int j=0;j<N;j++)
           {
             if (field[i][j]==0) continue;
             s.setTextureRect(sf::IntRect(field[i][j]*18,0,18,18));
             s.setPosition(j*18,i*18);
             s.move(28,31);
             window.draw(s);
           }

        for (int i=0;i<4;i++)
          {
            s.setTextureRect(sf::IntRect(colorNum*18,0,18,18));
            s.setPosition(a[i].x*18,a[i].y*18);
            s.move(28,31);
            window.draw(s);
          }
        window.draw(Time_elapsed);
        window.draw(Score);
        window.display();
        if(game_over == true){
            window.clear(sf::Color::White);
            window.close();
        }
        }
};
