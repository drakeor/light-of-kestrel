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

#include "entity.h"
#include "game.h"
#include "log.h"

Entity::Entity(Game* game)
{
  this->game = game;
}

Entity::~Entity()
{

}

void Entity::Initialise() {
  // Initial assigning
  texture = sf::Sprite((*game->GetAssetManager()->GetTexture("dragon.png")));
  
}

void Entity::Update(float dt) {
  
}

void Entity::Render() {
  // Due to SFML's retardation, I need to set the texture everyframe so it doesn't drop out of memory.
  // Note that sending the sprite to the heap will segfault the program.
  texture.setTexture(*game->GetAssetManager()->GetTexture("dragon.png"));
  this->game->GetWindow()->draw(texture);
}

