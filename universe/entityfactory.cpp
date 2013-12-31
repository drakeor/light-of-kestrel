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

#include "entityfactory.h"
#include "../game.h"
#include <components/armourplating.h>
#include <components/missilebay.h>
#include <components/vectorthruster.h>

EntityFactory::EntityFactory()
{

}

EntityFactory::~EntityFactory()
{

}

void EntityFactory::BuildTexture(Game* game, Entity* entity, std::string textureName)
{
  sf::Texture* tempTex;
  sf::Sprite texture;
  tempTex = game->GetAssetManager()->GetTexture(textureName);
  texture = sf::Sprite(*tempTex);
  texture.setOrigin(tempTex->getSize().x/2, tempTex->getSize().y/2);
  entity->SetSprite(texture);
  entity->TempCollisionDistance = tempTex->getSize().x/2;
}

void EntityFactory::BuildShip(Game* game, Entity* entity, std::string textureName, std::string entName)
{
  BuildTexture(game, entity, textureName);
  entity->SetName(entName);
  entity->collidesWith = COLLISION_GROUP_1; /* Will collide with other spaceships/astroids */
  entity->collisionGroup = COLLISION_GROUP_1;
}

Entity* EntityFactory::BuildEntity(Game* game, entity_t entityType)
{
  Entity* entity = new Entity(game);
  entity->Initialise();
  sf::Texture* tempTex;
  sf::Sprite texture;
  
  switch(entityType) {
    
    /* Spaceships */
    case SPACESHIP:
      BuildShip(game, entity, "nope.png", "Spaceship");
      break;
   
    case SS_HORNET:
      
      BuildShip(game, entity, "nope.png", "Hornet I");
      entity->SetIcon("gfx/icons/shipicon.png");
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, MissileBay());
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(LEFT, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(RIGHT, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      entity->AddComponent(BACK, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      entity->AddComponent(BACK, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      
      entity->AddMissile(MISSILE_VEILLON_I, 8);
      entity->AddMissile(MISSILE_SHIVE_I, 3);
      break;
    
   /* World Objects */
    case ASTROID:
      BuildTexture(game, entity, "astroid.png");
      entity->SetIcon("gfx/icons/astroidicon.png");
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->SetName("Astroid");
      entity->collidesWith = COLLISION_GROUP_1; /* Will collide with other spaceships/astroids */
      entity->collisionGroup = COLLISION_GROUP_1;
      break;
   
    // Nope missile is the basis.
    case MISSILE:
      BuildTexture(game, entity, "nope_missile.png");
      entity->SetName("Missile");
      entity->collidesWith = COLLISION_GROUP_1; /* Will interact with other spaceships/astroids but not with each other. */
      entity->collisionGroup = COLLISION_GROUP_2; 
      break;
      
    // Not implemented. In theory. Yet.
    case ANTIMISSILE:
      entity->SetName("Antimissile");
      entity->collidesWith = COLLISION_GROUP_1 | COLLISION_GROUP_2 | COLLISION_GROUP_3; /* Will interact with everything but is part of nothing. */
      entity->collisionGroup = 0; 
      entity->TempCollisionDistance = tempTex->getSize().x/2;
      break;
  };
  return entity;
}

