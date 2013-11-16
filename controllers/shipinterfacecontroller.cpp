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

ShipInterfaceController::ShipInterfaceController(Game* game): BaseController(game)
{
  shipDisplayControls = new BaseControl(game);
  shipDisplayControls->SetPosition(600, 500);
  shipDisplayControls->SetSize(300, 300);
  
  sf::Texture* tempTex = game->GetAssetManager()->GetTexture("gfx/interface/shipcontainer.png");
  shipContainer = sf::Sprite(*tempTex);
  shipContainer.setOrigin(tempTex->getSize().x/2, tempTex->getSize().y/2);
  shipContainer.setPosition(800, 600);
}

void ShipInterfaceController::Render()
{
  game->GetWindow()->draw(shipContainer);
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
