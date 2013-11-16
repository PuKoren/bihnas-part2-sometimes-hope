#include <SDL.h>
#include "Vector2.h"
#include "Rectangle.h"

class Enemy: public Rectangle{
    private:
        SDL_Color color;
        int currentLife;
        int lifeTime;
        int currentLifeTime;
    public:
        Enemy(Vector2, Vector2, Vector2, int, SDL_Color);
        ~Enemy();
        Vector2 Direction;
        void Update(Uint32);
        void Draw(SDL_Surface*);
        bool IsColored(SDL_Color);
        bool IsDead();
        void Kill();
        void Hit();
};