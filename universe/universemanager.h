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

#ifndef UNIVERSEMANAGER_H
#define UNIVERSEMANAGER_H
#include <vector>
#include "galaxy.h"
#include "../events/eventhandler.h"

class Game;

class UniverseManager
{
  std::vector<Galaxy*> galaxy;
  Galaxy* currentGalaxy;
  Game* game;
  
public:
  UniverseManager(Game* target_game);
  void Initialise();
  void Update(float dt);
  void Render();
  void ChangeGalaxy(Galaxy* galaxy);
  Galaxy* GetCurrentGalaxy();
  UniverseManager(const UniverseManager& other);
  virtual ~UniverseManager();
  EventHandler OnGalaxyChange;
};

#endif // UNIVERSEMANAGER_H
