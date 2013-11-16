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

#include "shipinterfacecontroller.h"
#include "../game.h"
#include "../log.h"
#include <SFML/Window/Mouse.hpp>

ShipInterfaceController::ShipInterfaceController(Game* game): BaseController(game)
{
  shipDisplayControls = new BaseControl(game);
  shipDisplayControls->SetPosition(600, 500);
  shipDisplayControls->SetSize(300, 300);
  targettedEntity = nullptr;
  sf::Texture* tempTex = game->GetAssetManager()->GetTexture("gfx/interface/shipcontainer.png");
  shipContainer = sf::Sprite(*tempTex);
  shipContainer.setOrigin(tempTex->getSize().x/2, tempTex->getSize().y/2);
  shipContainer.setPosition(800, 600);
}

void ShipInterfaceController::Render()
{
  game->GetWindow()->draw(shipContainer);
  if(targettedEntity != nullptr) {
    if(!targettedEntity->GetIcon().empty()) {
      selectedIcon.setPosition(800, 600);
      game->GetWindow()->draw(selectedIcon);
    }
  }
  BaseController::Render();
}

void ShipInterfaceController::Update(float dt)
{
  BaseController::Update(dt);
}

Entity* ShipInterfaceController::GetEntity()
{
  return targettedEntity;
}

void ShipInterfaceController::SetEntity(Entity* entity)
{
  targettedEntity = entity;
}

void ShipInterfaceController::HandleMouseClick()
{
  // Search through entities to see if we catch any of them!
  sf::Vector2i vec2 = sf::Mouse::getPosition((*game->GetWindow()));
  vec2 = sf::Vector2i(vec2.x + game->GetCamera()->GetPosition().x, vec2.y + game->GetCamera()->GetPosition().y);
  if(game->GetUniverseManager()->GetCurrentGalaxy() != nullptr) {
    Galaxy* cGalaxy = game->GetUniverseManager()->GetCurrentGalaxy();
    std::vector<Entity*> entity = game->GetUniverseManager()->GetCurrentGalaxy()->GetEntityList();
    for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
      sf::Vector2i entPosition = game->GetWindow()->mapCoordsToPixel(sf::Vector2f((*it)->GetCurrentPosition().x, (*it)->GetCurrentPosition().y));
      float distance = sqrt(pow((entPosition.x - vec2.x),2) + pow((entPosition.y - vec2.y),2));
      if(distance < ((*it)->TempCollisionDistance)) {
	// Select the new entity and change the icon
	targettedEntity = (*it);
	if(!targettedEntity->GetIcon().empty()) {
	  sf::Texture* tempTex = game->GetAssetManager()->GetTexture(targettedEntity->GetIcon());
	  selectedIcon = sf::Sprite(*tempTex);
	  selectedIcon.setOrigin(tempTex->getSize().x/2, tempTex->getSize().y/2);
	}
      }
    }
  }
}
