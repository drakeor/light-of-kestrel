/*
 * 
 * Copyright (c) 2013 River Bartz <drakeor.dragon@gmail.com>
 * 
 *     Permission is hereby granted, free of charge, to any person
 *     obtaining a copy of this software and associated documentation
 *     files (the "Software"), to deal in the Software without
 *     restriction, including without limitation the rights to use,
 *     copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the
 *     Software is furnished to do so, subject to the following
 *     conditions:
 * 
 *     The above copyright notice and this permission notice shall be
 *     included in all copies or substantial portions of the Software.
 * 
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *     OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *     NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *     HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *     WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *     FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *     OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

#include <entity.h>
#include "game.h"
#include "ai/shipai.h"
#include "ai/relationshipmanager.h"
#include "log.h"

#define AI_AGGRO_RANGE 4000
#define AI_HOME_RANGE 2000


ShipAI::ShipAI() : targetEntity(0),
  homePosition(sf::Vector2f(0,0)),
  aiState(ShipAIState::SEEKING)
{

}

ShipAI::~ShipAI()
{

}

// Helper functions
float GetDistance(sf::Vector2f pos1, sf::Vector2f pos2)
{
  return (float)sqrt( pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2) );
}
float GetAngle(sf::Vector2f pos1, sf::Vector2f pos2)
{
  float dY = (pos2.y) - (pos1.y);
  float dX = (pos2.x) - (pos1.x);
  return (float)atan2(dY, dX);
}
inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}

void ShipAI::ProcessTurn()
{
  // Do a few checks
  if(myEntity == nullptr) {
    FILE_LOG(logERROR) << "(ShipAI): The entity I'm attached to is null!";
  }
  if(myEntity->GetFaction() == RelationshipManager::FACTIONLESS) {
    FILE_LOG(logWARNING) << "(ShipAI): The entity I'm attached to is FACTIONLESS: " << myEntity->GetName();
  }
  if((homePosition.x == 0) && (homePosition.y == 0)) {
    homePosition = myEntity->GetCurrentPosition();
  }
  
  // Make sure we have a valid entity
  if(myEntity != nullptr) {
    
    // Set up initial variables
    Entity* targettedEnt = nullptr;
    int targetID = 0;
    float targettedDist = AI_AGGRO_RANGE;
    float targettedAngle = 0;
    float myRotation = fmod (myEntity->GetCurrentRotation(),6.28);
    std::vector<Entity*> listOfEntites = myEntity->GetGame()->GetUniverseManager()->GetCurrentGalaxy()->GetEntityList();
    
    
    /*
     * Things may have happened between this turn and the last such as
     * 1) The enemy dying, then correct back to wandering state
     */
    if((listOfEntites[targetEntity] == nullptr) || (targetEntity == 0)) {
      aiState = ShipAIState::SEEKING;
      FILE_LOG(logWARNING) << "Looking for new target";
    }
    
    
    /*
     * This is our ships wandering state. 
     * Basically, the ship is going to look around for some enemies.
     * 
     * If we drift too far from the home position, we're immediately going to turn and drive right towards it.
     * If we find a target, we're immediately going after it.
     * 
     * Otherwise, pick a random direction and go there.
     */
    if(aiState == ShipAIState::SEEKING) {
	for(auto it = listOfEntites.begin(); it != listOfEntites.end(); ++it) {
	  if((*it) != nullptr) {
	    float dist = GetDistance(myEntity->GetCurrentPosition(), (*it)->GetCurrentPosition());
	    if(dist < AI_AGGRO_RANGE) {
	      
	      // If the other entity is an enemy..
	      if(RelationshipManager::GetRelationship(myEntity, (*it)) == RelationshipManager::ENEMY) {
		
		// We want to prioritize the ones that are "easiest" to reach. AKA rotation points in their direction.
		float rot2 = 0; 
		rot2 = GetAngle(myEntity->GetCurrentPosition(), (*it)->GetCurrentPosition());
		float deltaRot = rot2 - myRotation;
		if(abs(deltaRot) < targettedDist) { 
		  targettedEnt = (*it);
		  targettedDist = abs(deltaRot);
		}
	      }
	    }
	  }
	}
	
	/* If we actually found a target, we're going to the ENGAGING STATE immediately */
	if(targettedEnt != nullptr) {
	  FILE_LOG(logWARNING) << "Found new target: " << targettedEnt->GetName();
	  targetEntity = targettedEnt->GetId();
	  aiState = ShipAIState::ENGAGING;
	} else {
	   /* !! We're committing the movement for wandering straightaway !! */
	   myEntity->SetTargetVelocity(120);
	   float dist = GetDistance(myEntity->GetCurrentPosition(), homePosition);
	   // If we're too far away, lets head back home.
	   if(dist > AI_HOME_RANGE) {
	      // Correct to the home position
	     float targetRotation = GetAngle(myEntity->GetCurrentPosition(), homePosition);
	     float deltaRot = targetRotation - myRotation;
	     deltaRot = clamp(deltaRot, -1.7f, 1.7f);
	     myEntity->SetTargetRotation(myRotation+deltaRot);
	     myEntity->SetTargetVelocity(10);
	   }
	}
    }
    
    /*
     * This is our ships ENGAGING ai state. 
     * 
     * We already have a targettedEntity in mind.
     * We're going to correct course towards them.
     */
    if(aiState == ShipAIState::ENGAGING) {
      if(targetEntity == 0) FILE_LOG(logERROR) << "(Ship AI): Dropped ship pointer on Engaging state" << std::endl;
      /* !! We're committing movement for ENGAGE state right here! */
       float targetRotation = GetAngle(myEntity->GetCurrentPosition(), listOfEntites[targetEntity]->GetCurrentPosition());
      if(myEntity->HasMissile(MISSILE_VEILLON_I)) {
	myEntity->FireMissile(MISSILE_VEILLON_I);
      }
      float deltaRot = targetRotation - myRotation;
      deltaRot = clamp(deltaRot, -1.7f, 1.7f);
      
      myEntity->SetTargetVelocity(80);
      myEntity->SetTargetRotation(myRotation+deltaRot);
    }
    
    
  }
}

