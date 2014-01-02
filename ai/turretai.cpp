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

#include <ai/turretai.h>
#include <entity.h>
#include <game.h>
#include <vector>

#define MAX_VISUAL_RANGE 500

TurretAI::TurretAI()
{

}

TurretAI::~TurretAI()
{

}

void TurretAI::ProcessTurn()
{
  // Make sure we have a valid entity
  if(myEntity != nullptr) {
    
    // Get our list of entities in this galaxy
    std::vector<Entity*> listOfEntites = myEntity->GetGame()->GetUniverseManager()->GetCurrentGalaxy()->GetEntityList();
    
    // We only want to target ones within our range.
    // MAX_VISUAL_RANGE
    
    // We only want to target ones that are our enemies.
  }
}

