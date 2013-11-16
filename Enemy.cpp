#include "Enemy.h"

Enemy::Enemy(Vector2 _pos, Vector2 _size, Vector2 _dir, int _lifeTime, SDL_Color _color): Rectangle(_pos, (int)_size.X, (int)_size.Y){
    this->color = _color;
    this->Direction = _dir;
    this->lifeTime = _lifeTime;
    this->currentLifeTime = 0;
    this->currentLife = 10;
}

Enemy::~Enemy(){

}

void Enemy::Hit(){
	this->currentLife--;
}

void Enemy::Kill(){
	this->currentLife = 0;
}

bool Enemy::IsDead(){
	return ((this->currentLifeTime > this->lifeTime && this->lifeTime > 0) || this->currentLife <= 0);
}

void Enemy::Draw(SDL_Surface* viewport){
    Rectangle::Draw(viewport, this->color);
}

void Enemy::Update(Uint32 gameTime){
	this->Position = this->Position + (this->Direction * gameTime);
	this->currentLifeTime += gameTime;
}

bool Enemy::IsColored(SDL_Color _color){
    if(this->color.r == _color.r && this->color.g == _color.g && this->color.b == _color.b){
        return true;
    }else{
        return false;
    }
}