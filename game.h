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


#ifndef GAME_H
#define GAME_H

#include "universemanager.h"
#include "inputmanager.h"
#include "assetmanager.h"
#include "guimanager.h"
#include "camera.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class Game
{  
  std::unique_ptr<UniverseManager> universeManager;
  std::tr1::shared_ptr<GuiManager> guiManager;
  InputManager inputManager;
  AssetManager assetManager;
  Camera camera;
  sf::RenderWindow* window;
  sf::View* view;
  sf::FloatRect originRect;
   
public:
  Game() { }
  void Initialise(sf::RenderWindow* window, sf::View* view);
  void Render();
  void Update(float dt);
  void Destroy();
  AssetManager* GetAssetManager();
  GuiManager* GetGuiManager();
  sf::RenderWindow* GetWindow();
  sf::View* GetView();
  Camera* GetCamera();
  Game(const Game& other) { }
virtual ~Game() { }
};

#endif // GAME_H
