//
// Created by Stepan on 08.02.2022.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>



///MAP///

class MAP{
public:
    std::vector<std::string> map;
    MAP();

    void change(int i,int j, char element);

};

///ANIMATION///

class Animation{
    friend class Animation_Manager;
public:

//CONSTRUCTOR
    Animation() = default;
    Animation(sf::Texture &t,int x,int y,int w,int h,float speed,int capacity,int step);
//FUNCTIONS
    void tick(float time);
private:
    std::vector<sf::Rect<int>> frames;
    std::vector<sf::Rect<int>> flip_frames;
    float currentFrame{}, playing_speed{};
    bool is_Playing{},flipped{};
    sf::Sprite sprite;
};


///ANIMATION_MANAGER///

class Animation_Manager{

public:

    Animation_Manager()= default;;

    void create(std::string name, sf::Texture &t,int x,int y,int w,int h,float speed,int capacity,int step);
    void draw(sf::RenderWindow &window,float x = 0,float y = 0);
    void set(std::string name);
    void tick(float time);
    void pause();
    void play();
    void flip(bool value);

private:

    std::string current_animation;
    std::map<std::string,Animation> animation_map;

};

///OBJECT///

class OBJECT {
public:
    OBJECT(Animation_Manager &A,float X,float Y);
    virtual void update(float time,MAP& MAP) = 0;
    virtual void draw(sf::RenderWindow &window,float mapDisplacement_x, float mapDisplacement_y) = 0;

    [[nodiscard]] float getX() const;

    [[nodiscard]] float getY() const;

    [[nodiscard]] bool isAlive() const;

    [[nodiscard]] bool isShoot() const;

    [[nodiscard]] bool isFlip() const;

    void setFlip(bool flip);

    sf::FloatRect RectGetter();

    [[nodiscard]] const std::string &getType() const;

    int getHealth() const;

    void setAlive(bool alive);

    void setHealth(int health);

protected:
    float x,y,Vx,Vy,w,h;
    Animation_Manager N_Anim;
    bool alive,flip;
    bool shoot;
    int health;
    std::string type;
};

///PLAYER///

class PLAYER: public OBJECT{
private:
    int bullets,bullets_in_gun;
    bool hit,sit,onGround;
    enum {
        stay,
        walk,
        dash,
        jump,
        climb,
        reloading,
    } state;

public:
    PLAYER(Animation_Manager &a, float X,float Y);
    void check_condition();
    void update(float time,MAP& MAP) override;
    void draw(sf::RenderWindow &window,float mapDisplacement_x, float mapDisplacement_y) override;
    void CollisionX(MAP& MAP);
    void CollisionY(MAP& MAP);

    int* getBullets();

    int* getBulletsInGun();

    float* getVx();
};

///ENEMY///

class ENEMY: public OBJECT{
public:
    ENEMY(Animation_Manager &a,float X,float Y);
    void check_condition(MAP& MAP);
    void update(float time,MAP& MAPa) override;
    void draw(sf::RenderWindow &window,float mapDisplacement_x, float mapDisplacement_y) override;
    void CollisionX(MAP& MAP);
    void CollisionY(MAP& MAP);
};

///BULLET///

class BULLET: public OBJECT{
public:
    BULLET(Animation_Manager &a, float X, float Y);

    void update(float time,MAP& MAP) override;
    void draw(sf::RenderWindow &window,float mapDisplacement_x, float mapDisplacement_y) override;
    void CollisionX(MAP& MAP);
    void CollisionY(MAP& MAP);
    void revers();
};