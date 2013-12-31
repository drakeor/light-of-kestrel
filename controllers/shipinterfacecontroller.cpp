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
  advancedDisplay = false;
  
  sf::Texture* tempTex = game->GetAssetManager()->GetTexture("gfx/interface/shipcontainer.png");
  shipContainer = sf::Sprite(*tempTex);
  shipContainer.setOrigin(tempTex->getSize().x/2, tempTex->getSize().y/2);
  guiAnchor = sf::Vector2i(game->GetWindow()->getSize().x-128, game->GetWindow()->getSize().y-128);
  shipContainer.setPosition(guiAnchor.x+64, guiAnchor.y+64);
  shipContainer.setScale(0.75f, 0.75f);
  
  tempTex = game->GetAssetManager()->GetTexture("gfx/interface/advancedcontainer.png");
  advancedContainer = sf::Sprite(*tempTex);
  advancedGuiAnchor = sf::Vector2i(game->GetWindow()->getSize().x-150, game->GetWindow()->getSize().y-150);
  
  advancedContainer.setOrigin(tempTex->getSize().x/2, tempTex->getSize().y/2);
  advancedContainer.setPosition(advancedGuiAnchor.x, advancedGuiAnchor.y);
}

void ShipInterfaceController::Render()
{
  game->GetWindow()->draw(shipContainer);
  if(targettedEntity != nullptr) {
    if(!targettedEntity->GetIcon().empty()) {
      selectedIcon.setPosition(guiAnchor.x+64, guiAnchor.y+64);
      selectedIcon.setScale(0.75f, 0.75f);
      game->GetWindow()->draw(selectedIcon);
    }
    
    // Draw the advanced display if needed
    if(advancedDisplay) {
      
      // Draw the GUI background
      game->GetWindow()->draw(advancedContainer);
      selectedIcon.setScale(1.0f, 1.0f);
      selectedIcon.setPosition(advancedGuiAnchor.x, advancedGuiAnchor.y);
      game->GetWindow()->draw(selectedIcon);
      
      if(targettedEntity != nullptr) {
	// TODO: Fix this up. Remove allocating a sprite for each thingy.
	// Draw the first layer ship components
	for(int i=0;i<MAX_COMPONENT_LAYERS;++i)
	{
	  for(int j=0;j<MAX_COMPONENT_SIDES;++j)
	  {
	    component_side_t comSide = (component_side_t)j;
	    std::vector<EntityComponent> entComponents = targettedEntity->GetComponents(comSide, (component_layer_t)i);
	    int z = 0;
	    for(std::vector<EntityComponent>::iterator it = entComponents.begin(); it != entComponents.end(); ++it) {
	      sf::Texture* tempTex;
	      if((*it).health > 0) {
		tempTex = game->GetAssetManager()->GetTexture((*it).icon);
	      } else {
		tempTex = game->GetAssetManager()->GetTexture("gfx/icons/destroyed.png");
	      }
	      sf::Sprite iconSprite = sf::Sprite(*tempTex);
	      iconSprite.setOrigin(tempTex->getSize().x/2, tempTex->getSize().y/2);
	      int offset1 = 60;
	      int offset2 = 80;
	      int centreAdd = 64;
	      int centreInc = 9;
	      if(comSide == FRONT) {
		iconSprite.setPosition(advancedGuiAnchor.x-offset1+z+centreAdd-(centreInc*entComponents.size()), advancedGuiAnchor.y-offset2);
	      } else if(comSide == LEFT) {
		iconSprite.setPosition(advancedGuiAnchor.x-offset2, advancedGuiAnchor.y-offset1+z+centreAdd-(centreInc*entComponents.size()));
	      } else if(comSide == RIGHT) {
		iconSprite.setPosition(advancedGuiAnchor.x+offset2, advancedGuiAnchor.y-offset1+z+centreAdd-(centreInc*entComponents.size()));
	      } else {
		iconSprite.setPosition(advancedGuiAnchor.x-offset1+z+centreAdd-(centreInc*entComponents.size()), advancedGuiAnchor.y+offset2);
	      }
	      game->GetWindow()->draw(iconSprite);
	      z += 18;
	    }
	  }
	}
      }
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
  // For gui stuff
  sf::Vector2f vec = game->GetWindow()->mapPixelToCoords(sf::Mouse::getPosition((*game->GetWindow())));
  if((vec.x > guiAnchor.x) && (vec.y > guiAnchor.y)) {
    // Toggle the larger control panel
    advancedDisplay = !advancedDisplay;
  } else {
    // Search through entities to see if we catch any of them!
    sf::Vector2i vec2 = sf::Mouse::getPosition((*game->GetWindow()));
    vec2 = sf::Vector2i(vec2.x + game->GetCamera()->GetPosition().x, vec2.y + game->GetCamera()->GetPosition().y);
    if(game->GetUniverseManager()->GetCurrentGalaxy() != nullptr) {
      Galaxy* cGalaxy = game->GetUniverseManager()->GetCurrentGalaxy();
      std::vector<Entity*> entity = game->GetUniverseManager()->GetCurrentGalaxy()->GetEntityList();
      for(std::vector<Entity*>::iterator it = entity.begin(); it != entity.end(); ++it) {
	if((*it) != nullptr) {
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
  }
}
