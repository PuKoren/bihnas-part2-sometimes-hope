#include "Ingame.h"

Ingame::Ingame(){
    this->gameFont = TTF_OpenFont("visitor2.ttf", SCREEN_WIDTH/20);
    this->textCombo = NULL;
    this->textLife = NULL;
    this->textScore = NULL;
    this->Init();
	srand(10);
}

Ingame::~Ingame(){
	for(unsigned int i = 0; i < this->bullets.size(); i++){
        this->DeleteBullet(i);
        i--;
    }
    for(unsigned int i = 0; i < this->enemies.size(); i++){
        this->DeleteEnemy(i);
        i--;
    }
    if(this->textCombo != NULL){
        SDL_FreeSurface(this->textCombo);
        SDL_FreeSurface(this->textLife);
        SDL_FreeSurface(this->textScore);
    }
    TTF_CloseFont(this->gameFont);
}


void Ingame::DeleteBullet(int index){
	delete this->bullets[index];
    this->bullets.erase(this->bullets.begin() + index);
}

void Ingame::DeleteEnemy(int index){
	delete this->enemies[index];
    this->enemies.erase(this->enemies.begin() + index);
}

void Ingame::Init(){
    this->SetScore(0);
    this->SetCombo(1);
    this->lifes = 4;
    this->player.Position.X = SCREEN_WIDTH/2;
    this->player.Position.Y = SCREEN_HEIGHT - 100;
    
    int divided = 1;
    #if PSP
    divided = 2;
    #endif
    
    this->player.Width = 20/divided;
    this->player.Height = 20/divided;
    this->enemySpeed = 0.1f/divided;
    this->playerSpeed = 0.2f/divided;
    this->bulletSpeed = 3.0f/divided;   
    
    this->direction.X = 0.f;
    this->direction.Y = 0.f;
    this->fireRate = 10;
    this->currentFireRate = 0;

    this->lastDirection.X = 0.f;
    this->lastDirection.Y = this->bulletSpeed;
    this->currentWave = 0;
    
    this->canPause = false;
    this->RefreshTextSurface(&this->textLife, this->lifes);
    for(unsigned int i = 0; i < this->bullets.size(); i++){
        this->DeleteBullet(i);
        i--;
    }
    for(unsigned int i = 0; i < this->enemies.size(); i++){
        this->DeleteEnemy(i);
        i--;
    }
}

void Ingame::Event(SDL_Event* ev, GAME_STATE* gs){
    if(ev->type == SDL_KEYDOWN){
        if(ev->key.keysym.sym == SDLK_DOWN){
        	this->direction.Y += this->playerSpeed;
        }else if(ev->key.keysym.sym == SDLK_UP){
        	this->direction.Y -= this->playerSpeed;
        }else if(ev->key.keysym.sym == SDLK_RIGHT){
        	this->direction.X += this->playerSpeed;
        }else if(ev->key.keysym.sym == SDLK_LEFT){
        	this->direction.X -= this->playerSpeed;
        }else if(ev->key.keysym.sym == SDLK_ESCAPE){
            *gs = PAUSE;
        }
    }else if(ev->type == SDL_KEYUP){
        if(ev->key.keysym.sym == SDLK_DOWN){
        	this->direction.Y -= this->playerSpeed;
        }else if(ev->key.keysym.sym == SDLK_UP){
        	this->direction.Y += this->playerSpeed;
        }else if(ev->key.keysym.sym == SDLK_RIGHT){
        	this->direction.X -= this->playerSpeed;
        }else if(ev->key.keysym.sym == SDLK_LEFT){
        	this->direction.X += this->playerSpeed;
        }
    }
}

#if PSP
void Ingame::Event(SceCtrlData* pad, SDL_Event* ev, GAME_STATE* gs){
    if(pad->Buttons != 0)
    {
        if(this->canPause && (pad->Buttons & PSP_CTRL_START || pad->Buttons & PSP_CTRL_TRIANGLE || pad->Buttons & PSP_CTRL_SELECT)){
            *gs = PAUSE;
            this->canPause = false;
        }
        if(pad->Buttons & PSP_CTRL_UP){
            this->direction.Y = -this->playerSpeed;
        }else if(pad->Buttons & PSP_CTRL_DOWN){
            this->direction.Y = this->playerSpeed;
        }else{
        	this->direction.Y = 0.f;
        }
        if(pad->Buttons & PSP_CTRL_RIGHT){
            this->direction.X = this->playerSpeed;
        }else if(pad->Buttons & PSP_CTRL_LEFT){
            this->direction.X = -this->playerSpeed;
        }else{
        	this->direction.X = 0.f;
        }
    }else{
        this->direction.Y = 0.0f;
        this->direction.X = 0.0f;
        this->canPause = true;
    }
}
#endif

void Ingame::Update(Uint32 gameTime, GAME_STATE* gs){
	this->currentFireRate += gameTime;
	if(this->currentFireRate >= this->fireRate){
		this->Fire();
		this->currentFireRate -= this->fireRate;
	}
	
	for(unsigned int i = 0; i < this->bullets.size(); i++){
		this->bullets[i]->Update(gameTime);
		
		for(unsigned int j = 0; j < this->enemies.size(); j++){
			if(this->bullets[i]->CollideWith(this->enemies[j])){
				this->bullets[i]->Kill();
				this->enemies[j]->Hit();
				this->AddScore(100 * this->combo);
			}
		}
		
		if(this->bullets[i]->IsDead() || this->IsOutOfBounds(this->bullets[i])){
			this->DeleteBullet(i);
			i--;
		}
	}
	
	for(unsigned int i = 0; i < this->enemies.size(); i++){
		this->enemies[i]->Direction = (this->player.Position - this->enemies[i]->Position).Normalize() * this->enemySpeed;
		this->enemies[i]->Update(gameTime);
		
		if(this->enemies[i]->CollideWith(&this->player)){
			this->RemoveLife(gs);
			this->enemies[i]->Kill();
		}
		
		if(this->enemies[i]->IsDead()){
			this->DeleteEnemy(i);
			i--;
		}
	}
	
	Vector2 prevPos = this->player.Position;
	this->player.Position = this->player.Position + (this->direction * gameTime);
	if(this->IsOutOfBounds(&this->player)){
		this->player.Position = prevPos;
	}
	
	if(this->direction.X != 0.f || this->direction.Y != 0.f){
		this->lastDirection = this->direction;
	}
	
	if(this->enemies.size() == 0){
		this->GenerateWave();
	}
}

void Ingame::GenerateWave(){
	this->currentWave++;
	this->SetCombo(this->combo + 1);
	
	for(int i = 0; i < this->currentWave; i++){
		Vector2 pos;
		if(rand()%2 == 1){
			pos.X = (rand()%2 == 1)?-20:SCREEN_WIDTH;
			pos.Y = rand()%SCREEN_HEIGHT;
		}else{
			pos.X = rand()%SCREEN_WIDTH;
			pos.Y = (rand()%2 == 1)?-20:SCREEN_HEIGHT;
		}
		
		int divider = 1;
		#if PSP
		divider = 2;
		#endif
		
		this->enemies.push_back(new Enemy(pos, Vector2(15/divider, 15/divider), Vector2(0, 0), 0, BLUE));
	}
}

bool Ingame::IsOutOfBounds(Enemy* e){
    if(e->Position.X + e->Width < 0){
        return true;
    }else if(e->Position.X > SCREEN_WIDTH){
        return true;
    }
    if(e->Position.Y > SCREEN_HEIGHT){
        return true;
    }else if(e->Position.Y + e->Height < 0){
        return true;
    }

    return false;
}

bool Ingame::IsOutOfBounds(Rectangle* e){
    if(e->Position.X < 0){
        return true;
    }else if(e->Position.X + e->Width > SCREEN_WIDTH){
        return true;
    }
    if(e->Position.Y + e->Height > SCREEN_HEIGHT){
        return true;
    }else if(e->Position.Y < 0){
        return true;
    }

    return false;
}

void Ingame::Fire(){
	Vector2 dir = this->lastDirection;
	
	float neg = (rand()%2 == 1)?-1.f:1.f;
	dir.X = (this->lastDirection.X == 0.f)?this->playerSpeed*neg:this->lastDirection.X;
	dir.X = (float)this->GetRandom(25)/100 * dir.X;
	dir.Y = (this->lastDirection.Y == 0.f)?this->playerSpeed*neg:this->lastDirection.Y;
	dir.Y = (float)this->GetRandom(25)/100 * dir.Y;
	
	this->bullets.push_back(new Enemy(this->player.Position + Vector2(this->player.Width/2, this->player.Height/2), Vector2(5, 5), -dir * this->bulletSpeed, 700, RED));
}

void Ingame::AddScore(int score){
    this->score += score * this->combo;
    this->RefreshTextSurface(&this->textScore, this->score);
}

void Ingame::SetScore(int score){
    this->score = score;
    this->RefreshTextSurface(&this->textScore, this->score);
}

void Ingame::SetCombo(int combo){
    this->combo = combo;
    this->RefreshTextSurface(&this->textCombo, this->combo-1);
}

void Ingame::AddLife(){
    this->lifes++;
    this->RefreshTextSurface(&this->textLife, this->lifes);
}

void Ingame::RemoveLife(GAME_STATE* gs){
    this->lifes--;
    this->SetCombo(1);
    if(this->lifes < 0){
        *gs = GAMEOVER;
        this->RefreshTextSurface(&this->textLife, 0);
    }else{
        this->RefreshTextSurface(&this->textLife, this->lifes);
    }
}

void Ingame::RefreshTextSurface(SDL_Surface** surf, int value){
    if(*surf != NULL){
        SDL_FreeSurface(*surf);
    }
    *surf = this->GetIntTextSurface(value);
}

SDL_Surface* Ingame::GetIntTextSurface(int number){
    std::ostringstream s;
	s << number;
    return TTF_RenderUTF8_Solid(this->gameFont, s.str().c_str(), WHITE);
}

int Ingame::GetRandom(int limit){
    return rand()%limit+1;
}

void Ingame::Draw(SDL_Surface* viewport){
	this->player.Draw(viewport, RED);
	for(unsigned int i = 0; i < this->bullets.size(); i++){
		this->bullets[i]->Draw(viewport);
	}
	for(unsigned int i = 0; i < this->enemies.size(); i++){
		this->enemies[i]->Draw(viewport);
	}
    this->DrawUi(viewport);
}

void Ingame::DrawUi(SDL_Surface* viewport){
    SDL_Rect destR;
    destR.x = 5;
    destR.y = 0;
    destR.w = this->textLife->w;
    destR.h = this->textLife->h;
    SDL_BlitSurface(this->textLife, NULL, viewport, &destR);

    destR.x = SCREEN_WIDTH/2;
    destR.y = 0;
    destR.w = this->textScore->w;
    destR.h = this->textScore->h;
    SDL_BlitSurface(this->textScore, NULL, viewport, &destR);

    destR.x = SCREEN_WIDTH - 30;
    destR.y = 0;
    destR.w = this->textCombo->w;
    destR.h = this->textCombo->h;
    SDL_BlitSurface(this->textCombo, NULL, viewport, &destR);
}