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

#include "galaxy.h"
#include "../log.h"

Galaxy::Galaxy(Game* game)
{
  this->game = game;
  AddEntity(new Entity(game));
}

Galaxy::Galaxy(const Galaxy& other)
{

}

Galaxy::~Galaxy()
{

}

void Galaxy::Render() {
  for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    (*it)->Render();
  }
}

void Galaxy::Update(float dt) {
 for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
    (*it)->Update(dt);
  } 
}

void Galaxy::CommitTurn() {

}

void Galaxy::AddEntity(Entity* m_entity)
{
  m_entity->Initialise();
  entity.push_back(m_entity);
}

