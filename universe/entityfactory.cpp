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

#include "entityfactory.h"
#include "../game.h"

EntityFactory::EntityFactory()
{

}

EntityFactory::~EntityFactory()
{

}

Entity* EntityFactory::BuildEntity(Game* game, entity_t entityType)
{
  Entity* entity = new Entity(game);
  entity->Initialise();
  sf::Texture* tempTex;
  sf::Sprite texture;
  
  switch(entityType) {
    
    case SPACESHIP:
      tempTex = game->GetAssetManager()->GetTexture("nope.png");
      texture = sf::Sprite(*tempTex);
      texture.setOrigin(tempTex->getSize().x/2, tempTex->getSize().y/2);
      entity->SetSprite(texture);
      entity->SetName("Spaceship");
      break;
      
    case ASTROID:
      tempTex = game->GetAssetManager()->GetTexture("astroid.png");
      texture = sf::Sprite(*tempTex);
      texture.setOrigin(tempTex->getSize().x/2, tempTex->getSize().y/2);
      entity->SetSprite(texture);
      entity->SetName("Astroid");
      break;
      
  };
  return entity;
}

