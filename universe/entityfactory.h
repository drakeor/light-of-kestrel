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

#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "entity.h"

enum entity_t {
  // Gameworld objects
  SPACESHIP,
  ASTROID,
  MISSILE,
  ANTIMISSILE, /* Not implemented */
  
  // Spaceships
  SS_HORNET
};
  
class EntityFactory
{
  
  static void BuildShip(Game* game, Entity* entity, std::string textureName, std::string entName);
public:
  EntityFactory();
  virtual ~EntityFactory();
  static Entity* BuildEntity(Game* game, entity_t entityType);
  static void BuildTexture(Game* game, Entity* entity, std::string textureName);
};

#endif // ENTITYFACTORY_H
