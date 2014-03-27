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

#ifndef GALAXY_H
#define GALAXY_H
#include <vector>
#include "entity.h"

class Game;

/*
 * The Galaxy class holds individual galaxies that are availiable in the game.
 * Each galaxy has a list of entities that they manage and rely on the UniverseManager to update
 * and transfer entities between them.
 */

class Galaxy
{
  // Background image for the galaxy. Different galaxies may have different images
  sf::Sprite backgroundImage;
  sf::Texture* bgTexture;
   
  //List of entities and list of entities queued for creation.
  std::vector<Entity*> entity;
  std::vector<std::pair<float, Entity*>> queuedEntities;
  
  // Pointer to the game object
  Game* game;
  
  // Holds the current time. Used for Iteration cycles.
  float currentTime;
  
  // If a turn is currently being processed, this private function is called that will automatically
  // update the entities and process collisions.
  void Iterate(float dt);
 
public:
  
  // Constructor/Destructors
  Galaxy(Game* game);
  virtual ~Galaxy();
  Galaxy(const Galaxy& other);
  
  /*
   * Adds an entity to this galaxy.
   * FOR STABILITY, USE QueueEntityForTurn INSTEAD!
   * It is highly recommended to create an Entity with the entityFactory class rather then
   * creating an entity from new Entity().
   * 
   * Note that this entity's addition is immediate and may cause weird issues with the controllers in the middle of
   * iteration cycles.
   */
  void AddEntity(Entity* entity);
  
  /*
   * Returns true if the entity exists or not
   * As some entities may be in queue for deletion, this is the recommended way to check
   * as opposed to just checking if the pointer on that entity exists or not.
   */
  bool EntityExists(int id);
  
  /*
   * Deletes an entity from the entitylist. 
   * TODO: Allow bypassing of garbage collector if we're just transferring the entity from one galaxy
   * to the other.
   */ 
  void DeleteEntity(int id);
  
  /*
   * Defers the addition of an entity to the galaxy based on the time.
   * It is highly recommended to create an Entity with the entityFactory class rather then
   * creating an entity from new Entity().
   * 
   * Note that this is added into a queue with is processed every few cycles to update the entityList.
   */
  void QueueEntityForTurn(float time, Entity* entity);
  
  /*
   * Returns the entire entityList for the galaxy
   */
  std::vector<Entity*> GetEntityList();
  
  // Render and update cycles
  void Render();
  void Update(float dt);
  
  // Immediate starts turn processing
  void CommitTurn();
  
  // Are we currently processing a turn?
  bool IsTurnActive();
  
};

#endif // GALAXY_H
