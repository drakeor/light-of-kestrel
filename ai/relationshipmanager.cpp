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

#include "relationshipmanager.h"
#include <entity.h>

RelationshipManager::RelationshipManager()
{

}

RelationshipManager::~RelationshipManager()
{

}

RelationshipManager::RELATIONSHIP_TYPE RelationshipManager::GetRelationship(Entity* ent1, Entity* ent2)
{
  // If we're dealing with any factionless or civilian people, we can just ignore them.
  if((ent1->GetFaction() == CIVILIAN) || (ent2->GetFaction() == CIVILIAN) || (ent1->GetFaction() == FACTIONLESS) || (ent2->GetFaction() == FACTIONLESS)) {
    return NEUTRAL;
  } else {
    // If they're our own faction, we can assume they are our ally.
    if(ent1->GetFaction() == ent2->GetFaction()) {
      return FRIEND;
    } else {
      return ENEMY;
    }
  }
}


