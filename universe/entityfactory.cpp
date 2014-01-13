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

// Our includes
#include "entityfactory.h"
#include "../game.h"
#include <components/armourplating.h>
#include <components/missilebay.h>
#include <components/vectorthruster.h>
#include <ai/turretai.h>

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
      BuildShip(game, entity, "gfx/ships/nope.png", "Spaceship");
      break;
   
      /* Test turret */
    case TURRET:
      
      BuildShip(game, entity, "gfx/ships/turret.png", "Turret");
      entity->SetIcon("gfx/icons/shipicon.png");
      entity->AddComponent(FRONT, LAYER_1, MissileBay());
      entity->AddComponent(FRONT, LAYER_1, MissileBay());
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      
      entity->InstallAI(new TurretAI());
      entity->AddMissile(MISSILE_VEILLON_I, 12);
      entity->SetFaction(RelationshipManager::ENEMY_FACTION);
      break;
      
    /*
     * The hornet is a scout-class ship and easily the weakest ship in the game armourwise. 
     * Survival from a single missile hit is extremely low. The hornet is only equipped with 
     * dumbfire missiles which still cause considerable damage.
     * 
     * HP (Top,Sides,Rear): 233/233/200
     */
    case SS_HORNET:
      BuildShip(game, entity, "gfx/ships/nope.png", "Hornet I");
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
      
      entity->SetFaction(RelationshipManager::PLAYER_FACTION);
      break;
    
      /* The intrepid is the improved version of the hornet. It has a bit more armour,
       * increasing the survivalibity rate of a single missile hit by a bit. The intrepid
       * also carries dumb-guided missiles which increases their effectiveness
       * 
       * HP (Top, Side, Rear): 366/366/200
       */
    case INTREPID:
      BuildShip(game, entity, "gfx/ships/intrepid.png", "Intrepid");
      entity->SetIcon("gfx/icons/shipicon.png");
      
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, MissileBay());
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(LEFT, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(RIGHT, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      
      entity->AddComponent(BACK, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      entity->AddComponent(BACK, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      
      entity->AddMissile(MISSILE_VEILLON_I, 7);
      entity->AddMissile(MISSILE_SHIVE_I, 3);
      
      entity->SetFaction(RelationshipManager::PLAYER_FACTION);
      break;
   
   /* The Sabre is the staple frigate-class fighter. It adds an extra missile bay, effectively doubling the
    * firepower as opposed to the scout-class fighters.
    */
   case SABRE:
      BuildShip(game, entity, "gfx/ships/sabre.png", "Sabre");
      entity->SetIcon("gfx/icons/shipicon.png");
      
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      
      entity->AddComponent(LEFT, LAYER_1, MissileBay());
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(LEFT, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      
      entity->AddComponent(RIGHT, LAYER_1, MissileBay());
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(RIGHT, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      
      entity->AddComponent(BACK, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      entity->AddComponent(BACK, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      
      entity->AddMissile(MISSILE_VEILLON_I, 12);
      entity->AddMissile(MISSILE_SHIVE_I, 6);
      
      entity->SetFaction(RelationshipManager::PLAYER_FACTION);
      break;
   
   /* The Daelalus is the improved version of the Sabre and is much less common on the battlefield. The extra armour means 
    * the Daelalus can easily survive a single missile hit, as long as it's not directly in the engines. It is the only
    * tier one ship to carry Agile missiles
    */
   case DAELALUS:
      BuildShip(game, entity, "gfx/ships/daelalus.png", "Daelalus");
      entity->SetIcon("gfx/icons/shipicon.png");
      
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      
      entity->AddComponent(LEFT, LAYER_1, MissileBay());
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(LEFT, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      
      entity->AddComponent(RIGHT, LAYER_1, MissileBay());
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(RIGHT, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      
      entity->AddComponent(BACK, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL));
      entity->AddComponent(BACK, LAYER_1, VectorThruster(VECTORTHRUSTER_BASIC));
      
      entity->AddMissile(MISSILE_VEILLON_I, 14);
      entity->AddMissile(MISSILE_SHIVE_I, 4);
      entity->AddMissile(MISSILE_MIRA_I, 3);
      
      entity->SetFaction(RelationshipManager::PLAYER_FACTION);
      break;
      
   /* Turrets and starbases */
   
   /*
    * The gnat is a cheap, basic defensive turret. It's usually deployed in numbers to
    * add suppressive fire. They are the weakest thing in the game capable of being destroyed by
    * even the weakest of missiles.
    */
   case GNAT:
      BuildShip(game, entity, "gfx/ships/gnat.png", "Gnat");
      entity->SetIcon("gfx/icons/shipicon.png");
      
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, MissileBay());
      
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      
      entity->AddMissile(MISSILE_VEILLON_I, 8);
      entity->SetFaction(RelationshipManager::PLAYER_FACTION);
     break;
   
   /*
    * The Wasp is a staple defensive turret, capable of dealing out and taking
    * a good deal of damage. 
    */
   case WASP:
      BuildShip(game, entity, "gfx/ships/wasp.png", "Wasp");
      entity->SetIcon("gfx/icons/shipicon.png");
      
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(FRONT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      
      entity->AddComponent(LEFT, LAYER_1, MissileBay());
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(LEFT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      
      entity->AddComponent(RIGHT, LAYER_1, MissileBay());
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(RIGHT, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
      entity->AddComponent(BACK, LAYER_1, ArmourPlating(ARMOUR_STEEL) );
    
     break;
   
   /* World Objects */
    case ASTROID:
      BuildTexture(game, entity, "gfx/universe/astroid.png");
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
      entity->SetName("Astroid");
      entity->collidesWith = COLLISION_GROUP_1; /* Will collide with other spaceships/astroids */
      entity->collisionGroup = COLLISION_GROUP_1;
      break;
   
    // Nope missile is the basis.
    case MISSILE:
      BuildTexture(game, entity, "gfx/interface/default_missile.png");
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

