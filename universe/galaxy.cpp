/*
    Copyright (c) 2013 River Bartz <drakeor.dragon@gmail.com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/

#include "galaxy.h"
#include "../log.h"
#include "../game.h"
#include "../controllers/settings.h"

Galaxy::Galaxy(Game* game)
{
  this->game = game;
 // AddEntity(new Entity(game));
  rawrTexture = game->GetAssetManager()->GetTexture("starfield.png");
  rawrTexture->setRepeated(true);
  backgroundImage.setTexture(*rawrTexture);
}

Galaxy::Galaxy(const Galaxy& other)
{

}

Galaxy::~Galaxy()
{
  for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    delete(*it);
    (*it) = nullptr;
  }
}

void Galaxy::Render() {
  backgroundImage.setPosition(game->GetCamera()->GetPosition());
  backgroundImage.setTextureRect(sf::Rect<int>((int)game->GetCamera()->GetPosition().x, (int)game->GetCamera()->GetPosition().y, 
    game->GetWindow()->getDefaultView().getSize().x, game->GetWindow()->getDefaultView().getSize().y
  ));
  game->GetWindow()->draw(backgroundImage);
  for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    (*it)->Render();
  }
}

void Galaxy::Iterate(float dt)
{
  /* TODO: This is an incrediably rough and inefficient way to
   * handle collisions. I have a few ideas in mind to speed this up but I'm much too lazy to implement them.
   * Fix this part up. Right now, we're just using circular collisions with just simple vector addition to get the projected speed */
  for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    for(std::vector<Entity*>::iterator it2 = entity.begin(); it2 != entity.end(); ++it2) {
      /* Calculate distance */
      if(it != it2) {
	/* Remember to reposition objects IMMEDIATELY after so they don't keep colliding. Which would be pretty funny.*/
	float distance = sqrt(pow(((*it)->GetCurrentPosition().x - (*it2)->GetCurrentPosition().x),2) + pow(((*it)->GetCurrentPosition().y - (*it2)->GetCurrentPosition().y),2));
	if(distance < ((*it)->TempCollisionDistance + (*it2)->TempCollisionDistance)) {
	  if((*it)->collidesWith & (*it2)->collisionGroup){
	    (*it)->OnCollision((*it2));
	  }
	  if((*it2)->collidesWith & (*it)->collisionGroup){
	    (*it2)->OnCollision((*it));
	  }
	}
      }
    }
  }
}

std::vector< Entity* > Galaxy::GetEntityList()
{
  return entity;
}


void Galaxy::Update(float dt) {
 // Process turns for entities if we need to.
 for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    (*it)->Update(dt);
    if(currentTime < GameSettings::maxTime)
      (*it)->Iterate(dt);
  }
  if(currentTime < GameSettings::maxTime) {
    Iterate(dt);
    currentTime += dt;
  }
}

void Galaxy::CommitTurn() {
  currentTime = 0;
  for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    (*it)->CommitTurn(GameSettings::frameTime, GameSettings::maxTime);
  }
}

void Galaxy::AddEntity(Entity* m_entity)
{
  m_entity->SetParent(this);
  entity.push_back(m_entity);
}


