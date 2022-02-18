#include <iostream>
#include <SFML/Graphics.hpp>
#include "classes2.0.h"
#include "maps.h"
#include <vector>
#include <list>

#include <sstream>


///проверочка
int main() {
    //смещение
    float map_displacement_x = 0,map_displacement_y = 0;

    int game_speed = 250;

    //анимации(создание)
    sf::RenderWindow window(sf::VideoMode(600,350),"PLATFORMER");
    window.setFramerateLimit(120);
    window.setVerticalSyncEnabled(true);
    sf::Texture soldier;
    sf::Texture enemy;
    sf::Texture bullet;
    sf::Texture fon;
    sf::Texture block;

    fon.loadFromFile("fon.png");
    sf::Sprite FON;
    FON.setTexture(fon);

    enemy.loadFromFile("bot.png");
    soldier.loadFromFile("soldier.png");
    bullet.loadFromFile("bullet.png");
    block.loadFromFile("block.png");
    sf::Texture* BLOCK = &block;

    Animation_Manager anim_M;
    Animation_Manager anim_E1;
    Animation_Manager anim_B;


    anim_B.create("flying",bullet,0,0,11,11,0.005,1,10);


    anim_E1.create("chilling",enemy,0,0,55,51,0.008,11,55);
    anim_E1.set("chilling");

    anim_M.create("walking",soldier,0,0,43,58,0.005,18,43);
    anim_M.create("staying",soldier,0,58,43,58,0.005,18,43);
    anim_M.create("jumping",soldier,0,58,43,58,0.005,18,43);

    //TEST

    OBJECT* P1 = new PLAYER(anim_M,300,175);
    OBJECT* E1 = new ENEMY(anim_E1,300,176);
    std::list<OBJECT*> OBJECT_LIST;
    auto OBJECT_IT = OBJECT_LIST.begin();

    OBJECT_LIST.push_back(P1);
    OBJECT_LIST.push_back(E1);



    //проверка

    ///exp
    sf::RectangleShape rectangle;
    sf::Vector2f rectangle_size(32.0f, 32.0f);
    rectangle.setFillColor(sf::Color::Black);
    ///

    sf::Clock clock;

    MAP MAP1;


    sf::Font font;
    font.loadFromFile("a_AssuanTitulStrDs.ttf");
    sf::Text textB("B",font,40);
    sf::Text textHP("B",font,40);



    sf::Text GAME_OVER(" ",font,80);
    GAME_OVER.setString("GAME_OVER");
    GAME_OVER.setPosition(75,100);
    bool GAME_OVER_FLAG = false;



    while(window.isOpen()){
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        //регулируем скорость
        time = time/game_speed;

        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) and ! P1->isShoot() and *dynamic_cast<PLAYER*>(P1)->getBulletsInGun()>0){
            if(!(P1->isFlip())){OBJECT_LIST.push_back(new BULLET(anim_B,P1->getX()+43,P1->getY()+29));
                *dynamic_cast<PLAYER*>(P1)->getVx()-=0.24;
            }
            OBJECT* B = new BULLET(anim_B,P1->getX(),P1->getY()+29);
            B->setFlip(true);
            BULLET *element = dynamic_cast<BULLET*>(B);
            element->revers();
            if(P1->isFlip()) {OBJECT_LIST.push_back(B);
                *dynamic_cast<PLAYER*>(P1)->getVx()+=0.24;
            }
            (*dynamic_cast<PLAYER*>(P1)->getBulletsInGun())--;
        }

        std::ostringstream P_B_string;
        std::ostringstream H_P_string;
        P_B_string << "B:" << *dynamic_cast<PLAYER*>(P1)->getBulletsInGun() << "\\" << *dynamic_cast<PLAYER*>(P1)->getBullets();
        H_P_string << "HP:" << dynamic_cast<PLAYER*>(P1)->getHealth();
        textB.setString(P_B_string.str());
        textHP.setString(H_P_string.str());
        textB.setPosition(20,20);
        textHP.setPosition(20,70);

        for(OBJECT_IT = OBJECT_LIST.begin();OBJECT_IT!=OBJECT_LIST.end();OBJECT_IT++){
            (*OBJECT_IT)->update(time,MAP1);
            if( !(*OBJECT_IT)->isAlive()) {
                if((*OBJECT_IT)->getType() == "player") GAME_OVER_FLAG = true;
                //OBJECT_LIST.erase(OBJECT_IT);
            }
        }




        map_displacement_x = P1->getX() - 600/2;
        map_displacement_y = P1->getY() - 350/2;

        window.clear();
        window.draw(FON);

        for(auto & element: OBJECT_LIST){
            element->draw(window,map_displacement_x,map_displacement_y);
        }




        for(auto OBJECT_IT0 = OBJECT_LIST.begin();OBJECT_IT0!=OBJECT_LIST.end();OBJECT_IT0++){
            if((*OBJECT_IT0)->getType() == "player"){
                OBJECT *PLAYER = *OBJECT_IT0;
                if(PLAYER->getHealth()<=0) {
                    PLAYER->setAlive(false);
                    continue;
                }
                else{
                    for(auto OBJECT_IT2 = OBJECT_LIST.begin();OBJECT_IT2!=OBJECT_LIST.end();OBJECT_IT2++){
                        if((*OBJECT_IT2)->getType() == "enemy"){
                            OBJECT *ENEMY = *OBJECT_IT2;
                            if(ENEMY->getHealth()<=0) {
                                ENEMY->setAlive(false);
                                continue;
                            }

                            if((PLAYER->RectGetter()).intersects(ENEMY->RectGetter())){
                                PLAYER->setHealth(PLAYER->getHealth()-5);
                                std::cout << "auch" << std::endl;
                            }

                            for(auto OBJECT_IT3 = OBJECT_LIST.begin();OBJECT_IT3!=OBJECT_LIST.end();OBJECT_IT3++){
                                if((*OBJECT_IT3)->getType() == "bullet"){
                                    OBJECT *BULLET = *OBJECT_IT3;
                                    if((ENEMY->RectGetter()).intersects(BULLET->RectGetter())){
                                        ENEMY->setHealth(ENEMY->getHealth()-5);
                                        std::cout << "bonk" << std::endl;
                                        BULLET->setAlive(false);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        ///exp
        for(int i = 0;i<16;i++){
            for(int j = 0;j<40;j++){
                if(MAP1.map[i][j] == '#'){
                    rectangle.setTexture(BLOCK);
                    rectangle.setFillColor(sf::Color::White);
                }
                if(MAP1.map[i][j] == ' '){
                    continue;
                }
                if(MAP1.map[i][j] == '@'){
                    rectangle.setFillColor(sf::Color::Yellow);
                }
                if(MAP1.map[i][j] == 'D'){
                    rectangle.setFillColor(sf::Color::Red);
                }
                rectangle.setPosition(float(j)*32 - map_displacement_x,float(i)*32 - map_displacement_y);
                rectangle.setSize(rectangle_size);
                window.draw(rectangle);
            }
        }
        if(GAME_OVER_FLAG) {window.draw(GAME_OVER);
        FON.setColor(sf::Color::Red);}
        window.draw(textB);
        window.draw(textHP);
        ///
        window.display();
    }
}
