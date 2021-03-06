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

#include "guimanager.h"
#include "textlabel.h"
#include "button.h"
#include "slider.h"
#include "../log.h"
#include "../game.h"
#include <memory>
#include <SFML/Window/Mouse.hpp>

class exListener : public EventListener
{
public:
  exListener() {}
  virtual ~exListener() {}
  void OnEvent(EventHandler* handler) {
    FILE_LOG(logWARNING) << "BUTTON PRESSED!";
    exit(1);
  }
};

GuiManager::GuiManager(Game* game)
{
  this->game = game;
  rootNode = std::unique_ptr<BaseControl>( new BaseControl(game) );
}


GuiManager::~GuiManager()
{

}

BaseControl* GuiManager::GetRootNode()
{
  return rootNode.get();
}

void GuiManager::Render()
{
  rootNode.get()->Render();
}

void GuiManager::Update(float dt)
{
  // Get mouse clicks and toggle events for them
  
  rootNode.get()->Update(dt);
}

void GuiManager::HandleMouseClick()
{
  //float mouseX = sf::Mouse::getPosition(*(sf::Window*)(game->GetWindow())).x;
  //float mouseY = sf::Mouse::getPosition(*(sf::Window*)(game->GetWindow())).y;
  //rootNode.get()->CheckOnMouseClick(mouseX, mouseY);
  //sf::Vector2i viewVec = sf::Vector2i(game->GetWindow()->getViewport().top, game->GetWindow()->getViewport().left);
  //sf::Vector2i mouseVec = sf::Mouse::getPosition() - viewVec;
  /*
  sf::Vector2i calibrationVec = sf::Vector2i(0, 20);
  sf::Vector2i mouseVec = sf::Mouse::getPosition((*game->GetWindow()));
  FILE_LOG(logWARNING) << "Position: " << game->GetWindow()->getPosition().x << "," << game->GetWindow()->getPosition().y
    << " :: " << mouseVec.x << "," << mouseVec.y;
  */
  sf::Vector2f vec2 = game->GetWindow()->mapPixelToCoords(sf::Mouse::getPosition((*game->GetWindow())));
  rootNode.get()->CheckOnMouseClick(vec2.x, vec2.y);
}


