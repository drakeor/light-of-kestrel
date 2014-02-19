/*
 * /*
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

#include "missileai.h"
#include <entity.h>
#include "game.h"
#include "ai/shipai.h"
#include "ai/relationshipmanager.h"
#include "log.h"

#define MAX_VISUAL_RANGE 1200

float GetDistancess(sf::Vector2f pos1, sf::Vector2f pos2)
{
  return (float)sqrt( pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2) );
}
float GetAngless(sf::Vector2f pos1, sf::Vector2f pos2)
{
  return (float)atan2((pos2.y) - (pos1.y), (pos2.x) - (pos1.x));
}
inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}


MissileAI::MissileAI()
{
  TargettedId = 0;
}

MissileAI::~MissileAI()
{

}

void MissileAI::ProcessTurn()
{
  // Check for errors
  if(myEntity->GetFaction() == RelationshipManager::FACTIONLESS) {
    FILE_LOG(logWARNING) << "(TurretAI): The entity I'm attached to is FACTIONLESS: " << myEntity->GetName();
  }
  
  /*
  // Make sure we have a valid entity
  if(myEntity != nullptr) {
    
    // Set up initial variables
    Entity* targettedEnt = nullptr;
    float targettedDist = 9999;
    float targettedAngle = 1.0;
    float myRotation = fmod (myEntity->GetCurrentRotation(),6.28);
    
    // Get our list of entities in this galaxy
    std::vector<Entity*> listOfEntites = myEntity->GetGame()->GetUniverseManager()->GetCurrentGalaxy()->GetEntityList();
    
    // We only want to target ones within our range.
    // We're only going to grab a target in the beginning.
    if(targettedEnt == nullptr)
    {
      for(auto it = listOfEntites.begin(); it != listOfEntites.end(); ++it) {
	if((*it) != nullptr) {
	  float dist = GetDistancess(myEntity->GetCurrentPosition(), (*it)->GetCurrentPosition());
	  if(((*it)->GetId() != myEntity->GetId()) && ((*it)->GetName() != "Missile")) {
	    if(dist < MAX_VISUAL_RANGE) {
	      
	      // If the other entity is an enemy..
	      if(RelationshipManager::GetRelationship(myEntity, (*it)) != RelationshipManager::FRIEND) {

		// We want to prioritize the ones that are "easiest" to reach. AKA rotation points in their direction.
		// NOT distance
		float rot2 = 0; 
		rot2 = GetAngless(myEntity->GetCurrentPosition(), (*it)->GetCurrentPosition());
		float deltaRot = rot2 - myRotation;
		if(abs(deltaRot) < targettedAngle) {
		  targettedEnt = (*it);
		  FILE_LOG(logERROR) << "I am targetting ID: " << (*it)->GetId() << " with name " << (*it)->GetName() << " at angle " << deltaRot;
		  targettedAngle = deltaRot;
		}
		
	      }
	    }
	  }
	}
      }
    }
    
    // If we have a target, we'll adjust to it.
    if(targettedEnt != nullptr) {
      float targetRotation = GetAngless(myEntity->GetCurrentPosition(), targettedEnt->GetCurrentPosition());
      float deltaRot = targetRotation - myRotation;
     // deltaRot = clamp(deltaRot, -3.0f, 3.0f);
      
      //myEntity->SetTargetRotation(myRotation+deltaRot);
      myEntity->SetTargetRotation(targetRotation);
    }
  }
  */
}
