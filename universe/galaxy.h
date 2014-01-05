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

class Galaxy
{
  sf::Sprite backgroundImage;
  std::vector<Entity*> entity;
  std::vector<std::pair<float, Entity*>> queuedEntities;
  Game* game;
  float currentTime;
  
  void Iterate(float dt);
  sf::Texture* rawrTexture;
public:
  Galaxy(Game* game);
  void AddEntity(Entity* entity);
  void DeleteEntity(int id);
  void QueueEntityForTurn(float time, Entity* entity);
  std::vector<Entity*> GetEntityList();
  void Render();
  void Update(float dt);
  void CommitTurn();
  Galaxy(const Galaxy& other);
  virtual ~Galaxy();
};

#endif // GALAXY_H
