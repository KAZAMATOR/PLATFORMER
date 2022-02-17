//
// Created by Stepan on 08.02.2022.
//

#include "classes2.0.h"
#include "cmath"

///MAP METHODS
MAP::MAP() {
    map = { "########################################",
            "#                                      #",
            "#                @ @ @ @ @ @ @ @ @ @   #",
            "#                                      #",
            "#                                      #",
            "#              @ @ @ @ @               #",
            "#                                      #",
            "#         ##############################",
            "#                        D             #",
            "#            @   @       D         @   #",
            "#                        D             #",
            "#############################          #",
            "#                                      #",
            "#            D D D D D D D D D D       #",
            "#                                      #",
            "########################################",};
}


void MAP::change(int i, int j, char element) {
    map[i][j] = element;
}


///ANIMATION METHODS

Animation::Animation(sf::Texture &t,int x,int y,int w,int h,float speed,int capacity,int step) {
    playing_speed = speed;
    sprite.setTexture(t);
    currentFrame = 0;
    is_Playing = true;
    flipped = false;

    for(int i = 0;i<capacity;i++){
        frames.push_back(sf::IntRect(x+i*step,y,w,h));
        flip_frames.push_back(sf::IntRect(x+i*step+w,y,-w,h));
    }
}

void Animation::tick(float time){
    if(!is_Playing){
        return;
    }
    currentFrame += playing_speed * time * 1.5f;
    if(currentFrame > frames.size()){
        currentFrame -= frames.size();
    }
    int i = int(currentFrame);

    if(flipped) sprite.setTextureRect(flip_frames[i]);
    else sprite.setTextureRect(frames[i]);
}




///ANIMATION_MANAGER METHODS

void Animation_Manager::create(std::string name, sf::Texture &t, int x, int y, int w, int h, float speed, int capacity, int step){
    animation_map[name] = Animation(t,x,y,w,h,speed,capacity,step);
}

void Animation_Manager::draw(sf::RenderWindow &window, float x, float y){
    animation_map[current_animation].sprite.setPosition(x,y);
    window.draw(animation_map[current_animation].sprite);
}

void Animation_Manager::set(std::string name){current_animation = name;}

void Animation_Manager::tick(float time){animation_map[current_animation].tick(time);}

void Animation_Manager::pause(){animation_map[current_animation].is_Playing = false;}

void Animation_Manager::play(){animation_map[current_animation].is_Playing = true;}

void Animation_Manager::flip(bool value){animation_map[current_animation].flipped = value;}






///╔══╗╔══╗──╔══╗╔═══╗╔══╗╔════╗╔══╗//////╔══╗╔══╗──╔══╗╔═══╗╔══╗╔════╗╔══╗///
///║╔╗║║╔╗║──╚╗╔╝║╔══╝║╔═╝╚═╗╔═╝║╔═╝//////║╔╗║║╔╗║──╚╗╔╝║╔══╝║╔═╝╚═╗╔═╝║╔═╝///
///║║║║║╚╝╚╗──║║─║╚══╗║║────║║──║╚═╗//////║║║║║╚╝╚╗──║║─║╚══╗║║────║║──║╚═╗///
///║║║║║╔═╗║╔╗║║─║╔══╝║║────║║──╚═╗║//////║║║║║╔═╗║╔╗║║─║╔══╝║║────║║──╚═╗║///
///║╚╝║║╚═╝║║╚╝╚╗║╚══╗║╚═╗──║║──╔═╝║//////║╚╝║║╚═╝║║╚╝╚╗║╚══╗║╚═╗──║║──╔═╝║///
///╚══╝╚═══╝╚═══╝╚═══╝╚══╝──╚╝──╚══╝//////╚══╝╚═══╝╚═══╝╚═══╝╚══╝──╚╝──╚══╝///



///OBJECT///

OBJECT::OBJECT(Animation_Manager &A, float X, float Y) {
    N_Anim = A;
    x = X;
    y = Y;
    alive = true;
    Vx = 0;
    Vy = 0;
}

float OBJECT::getX() const {
    return x;
}

float OBJECT::getY() const {
    return y;
}

bool OBJECT::isAlive() const {
    return alive;
}

bool OBJECT::isShoot() const {
    return shoot;
}

bool OBJECT::isFlip() const {
    return flip;
}

void OBJECT::setFlip(bool flip) {
    OBJECT::flip = flip;
}

sf::FloatRect OBJECT::RectGetter() {
    return sf::FloatRect(x,y,w,h);
}

const std::string &OBJECT::getType() const {
    return type;
}

int OBJECT::getHealth() const {
    return health;
}

void OBJECT::setAlive(bool alive) {
    OBJECT::alive = alive;
}

void OBJECT::setHealth(int health) {
    OBJECT::health = health;
}



///PLAYER METHODS

PLAYER::PLAYER(Animation_Manager &a, float X,float Y): OBJECT(a,X,Y) {
    state = stay;
    bullets = 30;
    bullets_in_gun = 10;
    flip = false;
    shoot = false;
    sit = false;
    hit = false;
    onGround = false;
    type = "player";
    health = 100;
    h = 58;
    w = 43;
}

//сотояние

void PLAYER::check_condition() {

    //если кнопки нажать

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        if(!sit) Vx = -0.12;
        if(sit) Vx = -0.045;
        if(state == stay) state = walk;
        flip = true;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        if(!sit) Vx = 0.12;
        if(sit) Vx = 0.045;
        if(state == stay) state = walk;
        flip = false;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        if(state == stay or state == walk){Vy = -0.3; state = jump;}
        if(state == climb) Vy = -0.01;
        sit = false;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        if(state == stay or state == walk){sit = true;}
        if(state == climb){Vy = 0.01;}
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        if(!shoot and state != reloading and state != climb and state != dash) {
            shoot = true;

        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
        if(state == stay or state == walk){
            Vx = 0;
            while (bullets > 0 and bullets_in_gun < 10){
                bullets--;
                bullets_in_gun++;
            }
            state = reloading;
        }
    }



    //если кнопки отпустить

    if(!(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))){
        if(fabs(Vx)>0){
            if(fabs(Vx)<0.005) Vx -= Vx;

            else Vx -= Vx/fabs(Vx)*0.005;
        }
        if(state == walk) state = stay;
    }
    if(!(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))){
        if(state == climb) Vy = 0;
    }
    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if(sit) state = stay;
    }
    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        shoot = false;
    }
}

//хитбоксы

void PLAYER::CollisionX(MAP& MAP) {
    for(int i = y/32;i<(y+58)/32 and i<16 ;i++){
        for(int j = x/32;j<(x+43)/32 and j<40;j++){
            if(MAP.map[i][j] == '#'){
                if(Vx > 0){x = j*32-43;Vx = 0;}
                if(Vx < 0){x = j*32+32;Vx = 0;}
            }
            if(MAP.map[i][j] == '@'){MAP.map[i][j] = ' ';bullets+=5;}
            if(MAP.map[i][j] == 'D'){MAP.map[i][j] = ' ';
                if(health<=95)health +=5;
                else health = 100;
            }
        }
    }
}

void PLAYER::CollisionY(MAP& MAP) {
    for(int i = y/32;i<(y+58)/32 and i<16;i++){
        for(int j = x/32;j<(x+43)/32 and j<40;j++){
            if(MAP.map[i][j] == '#'){
                if(Vy > 0){y = i*32 - 58; Vy = 0;state = walk;}
                if(Vy < 0){y = i*32 +32; Vy = 0;}
            }
            if(MAP.map[i][j] == '@'){MAP.map[i][j] = ' ';bullets+=5;}
            if(MAP.map[i][j] == 'D'){MAP.map[i][j] = ' ';health+=5;}
        }
    }
}


void PLAYER::update(float time,MAP& MAP) {
    check_condition();

    if(state==stay) N_Anim.set("staying");
    if(state==walk) N_Anim.set("walking");
    if(state==jump) N_Anim.set("jumping");
    //if(state==dash) P_Anim.set();
    //if(state==climb) P_Anim.set("climbing");
    //if(state==reloading) P_Anim.set("reloading");

    if(shoot) {
        ///if(state==stay) P_Anim.set("staying_shoot");
        ///if(state==walk) P_Anim.set("walking_shoot");
        ///if(state==jump) P_Anim.set("jumping_shoot");
    }
    if(!shoot and sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) bullets_in_gun--;

    N_Anim.flip(flip);
    if(fabs(Vx)>=0.045) Vx -= Vx/fabs(Vx)*0.003;
    x += Vx*time;
    CollisionX(MAP);

    if(!onGround) Vy+= 0.0003f*time;
    y += Vy*time;
    CollisionY(MAP);
    onGround = false;

    N_Anim.tick(time);
}

void PLAYER::draw(sf::RenderWindow &window,float mapDisplacement_x, float mapDisplacement_y) {
    N_Anim.draw(window,300,175);
}

int* PLAYER::getBullets() {
    return &bullets;
}

int* PLAYER::getBulletsInGun() {
    return &bullets_in_gun;
}

float* PLAYER::getVx() {
    return &Vx;
}



///ENEMY_METHODS

ENEMY::ENEMY(Animation_Manager &a, float X, float Y): OBJECT(a,X,Y) {
    Vx = 0.09;
    Vy = 0;
    alive = true;
    type = "enemy";
    health = 300 ;
    h = 29;
    w = 29;
}

void ENEMY::check_condition(MAP& MAP) {

}

//хитбоксы

void ENEMY::CollisionX(MAP& MAP) {
    for(int i = y/32;i<(y+51)/32 and i<16 ;i++){
        for(int j = x/32;j<(x+55)/32 and j<40;j++){
            if(MAP.map[i][j] == '#'){
                if(Vx > 0){x = j*32-55;}
                if(Vx < 0){x = j*32+32;}
            }
        }
    }

    int a = y/32;
    int b1 = (x+55)/32;
    int b2 = (x-1)/32;
    if(MAP.map[a][b1] == '#' and Vx>0) Vx*=-1;
    if(MAP.map[a][b2] == '#' and Vx<0) Vx*=-1;
}

void ENEMY::CollisionY(MAP& MAP) {
    for(int i = y/32;i<(y+51)/32 and i<16;i++){
        for(int j = x/32;j<(x+55)/32 and j<40;j++){
            if(MAP.map[i][j] == '#'){
                if(Vy > 0){y = i*32 - 51; Vy = 0;}
                if(Vy < 0){y = i*32 +32; Vy = 0;}
            }
        }
    }
}

void ENEMY::update(float time,MAP& MAP) {
    check_condition(MAP);

    if(Vx < 0)N_Anim.flip(true);
    if(Vx > 0)N_Anim.flip(false);


    x += Vx*time;
    CollisionX(MAP);
    Vy+=0.0003*time;
    y += Vy*time;
    CollisionY(MAP);
    N_Anim.tick(time);
}

void ENEMY::draw(sf::RenderWindow &window,float mapDisplacement_x, float mapDisplacement_y) {
    N_Anim.draw(window,x-mapDisplacement_x,y-mapDisplacement_y);
}

///BULLET_METHODS

BULLET::BULLET(Animation_Manager &a, float X, float Y): OBJECT(a,X,Y) {
    Vx = 0.25;
    Vy = -0.01;
    alive = true;
    N_Anim.set("flying");
    type = "bullet";
    h = 11;
    w = 11;
}


void BULLET::CollisionX(MAP& MAP) {
    for(int i = y/32;i<16 and i>= 0 and i<(y+11)/32;i++){
        for(int j = x/32;j<40 and j >=0 and j<(x+11)/32;j++){
            if(MAP.map[i][j] == '#'){
                this->alive = false;
            }
        }
    }
}

void BULLET::CollisionY(MAP& MAP) {
    for(int i = y/32;i<(y+11)/32 and i<16 and i>=0;i++){
        for(int j = x/32;j<(x+11)/32 and j<40 and j >=0;j++){
            if(MAP.map[i][j] == '#'){
                alive = false;
            }
        }
    }
}

void BULLET::update(float time,MAP& MAP) {

    if(Vx > 0)N_Anim.flip(true);
    if(Vx < 0)N_Anim.flip(false);


    x += Vx*time;
    CollisionX(MAP);
    Vy+= 0.000024f*time;
    y += Vy*time;
    //CollisionY(MAP);
    N_Anim.tick(time);
}

void BULLET::draw(sf::RenderWindow &window,float mapDisplacement_x, float mapDisplacement_y) {
    if(alive)N_Anim.draw(window,x-mapDisplacement_x,y-mapDisplacement_y);
}

void BULLET::revers() {
    Vx *= -1;
}