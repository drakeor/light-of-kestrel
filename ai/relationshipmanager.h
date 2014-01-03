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

#ifndef RELATIONSHIPMANAGER_H
#define RELATIONSHIPMANAGER_H

class Entity;

class RelationshipManager
{
public:
  RelationshipManager();
  ~RelationshipManager();

   enum RELATIONSHIP_TYPE {
     FRIEND,		/* This ship is an ally. We'll assist them if they are under attack :3 */
     NEUTRAL,		/* We can ignore their presence. If they get caught in the crossfire, oh well */
     ENEMY		/* Attack! */
   };
   
   enum FACTION {
      FACTIONLESS,	// Used for things like missiles.
      CIVILIAN,		// Civilian faction just kinda exists. It's their fault for being on the battlefield anyways.
      PLAYER_FACTION,	// Placeholder
      ENEMY_FACTION	// Placeholder
    };
   static RELATIONSHIP_TYPE GetRelationship(Entity* ent1, Entity* ent2);
};

#endif // RELATIONSHIPMANAGER_H
