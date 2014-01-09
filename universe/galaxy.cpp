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
#include "entityfactory.h"
#include "../log.h"
#include "../game.h"
#include "../settings.h"
#include <controllers/shipinterfacecontroller.h>
#include <controllers/myplayercontroller.h>

Galaxy::Galaxy(Game* game)
{
  this->game = game;
  entity.clear();
 // AddEntity(new Entity(game));
  rawrTexture = game->GetAssetManager()->GetTexture("gfx/universe/starfield.png");
  rawrTexture->setRepeated(true);
  backgroundImage.setTexture(*rawrTexture);
}

Galaxy::Galaxy(const Galaxy& other)
{

}

Galaxy::~Galaxy()
{
  for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    if((*it) != nullptr) {
      delete(*it);
      (*it) = nullptr;
    }
  }
}

void Galaxy::Render() {
  backgroundImage.setPosition(game->GetCamera()->GetPosition());
  backgroundImage.setTextureRect(sf::Rect<int>((int)game->GetCamera()->GetPosition().x, (int)game->GetCamera()->GetPosition().y, 
    game->GetWindow()->getDefaultView().getSize().x, game->GetWindow()->getDefaultView().getSize().y
  ));
  game->GetWindow()->draw(backgroundImage);
  for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    if((*it) != nullptr) {
      (*it)->Render();
    }
  }
}

void Galaxy::Iterate(float dt)
{
  /* TODO: This is an incrediably rough and inefficient way to
   * handle collisions. I have a few ideas in mind to speed this up but I'm much too lazy to implement them.
   * Fix this part up. Right now, we're just using circular collisions with just simple vector addition to get the projected speed */
  // TODO: Check if the entity is enabled otherwise the queued entities will collide with each other.
  for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    for(std::vector<Entity*>::iterator it2 = entity.begin(); it2 != entity.end(); ++it2) {
      /* Calculate distance */
      if((it != it2) && ((*it) != nullptr) && ((*it2) != nullptr)) {
	/* Remember to reposition objects IMMEDIATELY after so they don't keep colliding. Which would be pretty funny.*/
	float distance = sqrt(pow(((*it)->GetCurrentPosition().x - (*it2)->GetCurrentPosition().x),2) + pow(((*it)->GetCurrentPosition().y - (*it2)->GetCurrentPosition().y),2));
	if(distance < ((*it)->TempCollisionDistance + (*it2)->TempCollisionDistance)) {
	  if((*it)->collidesWith & (*it2)->collisionGroup){
	    (*it)->OnCollision((*it2));
	  }
	  if(((*it) != nullptr) && ((*it2) != nullptr)) {
	    if((*it2)->collidesWith & (*it)->collisionGroup){
	      (*it2)->OnCollision((*it));
	    }
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
 // Remove dead entities
  /*for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    if((*it) == nullptr) {
      it = entity.erase(it);
    }
  }*/
  
 // Process turns for entities if we need to.
 for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
   if((*it) != nullptr) {
      (*it)->Update(dt);
      if(currentTime < GameSettings::maxTime)
	(*it)->Iterate(dt);
   }
  }
  if(currentTime < GameSettings::maxTime) {
    
    // Deal with our queued entities
    auto tQueuedEnts = queuedEntities;
    tQueuedEnts.clear();
    for(auto it = queuedEntities.begin(); it != queuedEntities.end(); ++it) {
      float myTime; 
      Entity* myEnt;
      std::pair<float, Entity*> qEnt = (*it);
      myTime = qEnt.first;
      myEnt = qEnt.second;
      myTime -= dt;
      if(myTime < dt) {
	AddEntity(myEnt);
      } else {
	std::pair<float, Entity*> queuedEnt (myTime, myEnt);
	tQueuedEnts.push_back(queuedEnt);
      }
    }
    queuedEntities.clear();
    queuedEntities = tQueuedEnts;
    
    // Deal with our collisions
    Iterate(dt);
    currentTime += dt;
  }
}

void Galaxy::CommitTurn() {
  currentTime = 0;
  
  for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    if((*it) != nullptr) {
      // Process AI routines
      if((*it)->GetAI() != nullptr) {
	(*it)->GetAI()->ProcessTurn();
      }
      // Committing turn
      (*it)->CommitTurn(GameSettings::frameTime, GameSettings::maxTime);
    }
  }
}

void Galaxy::AddEntity(Entity* m_entity)
{
  m_entity->SetParent(this);
  m_entity->SetId(entity.size());
  entity.push_back(m_entity);
}

void Galaxy::DeleteEntity(int id)
{
  if(entity[id] != nullptr)  {
      /* This fixes the crash with MyPlayerController */
      MyPlayerController* playerController = (MyPlayerController*) game->GetControllerManager()->GetController("PlayerController");
      Entity* ent2 = playerController->GetPlayer();
      if(ent2 != nullptr) {
	if(ent2->GetId() == id) {
	  playerController->ResetPlayer();
	}
      }
      
      /* This fixes crashes with deleting the entity this controller is watching. */
      ShipInterfaceController* interface2 = (ShipInterfaceController*) game->GetControllerManager()->GetController("ShipInterface");
      Entity* ent = interface2->GetEntity();
      if(ent != nullptr) {
	if(ent->GetId() == id) {
	  interface2->SetEntity(nullptr);
	}
      }
      delete(entity[id]);
      entity[id] = nullptr;
    }
}

void Galaxy::QueueEntityForTurn(float time, Entity* entity2)
{
  std::pair<float, Entity*> queuedEnt (time, entity2);
  queuedEntities.push_back(queuedEnt);
  
}

