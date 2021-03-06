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



#include "game.h"
#include "log.h"
#include "controllers/myplayercontroller.h"
#include "controllers/worldcontroller.h"
#include "controllers/shipinterfacecontroller.h"
#include "controllers/console.h"

void Game::Initialise(sf::RenderWindow* window, sf::View* view)
{
  // Start up everything
  this->window = window;
  this->view = view;
  camera = Camera(this, view->getViewport());
  universeManager = std::unique_ptr<UniverseManager>( new UniverseManager(this) );
  guiManager = std::tr1::shared_ptr<GuiManager>( new GuiManager(this) );
  controllerManager = std::unique_ptr<ControllerManager>( new ControllerManager(this) );
  
  // Add our controllers
  controllerManager.get()->AddController("WorldController", new WorldController(this));
  controllerManager.get()->AddController("PlayerController", new MyPlayerController(this));
  controllerManager.get()->AddController("ShipInterface", new ShipInterfaceController(this));
  controllerManager.get()->AddController("Console", new Console(this));
  
  // State the game is now started up.
  FILE_LOG(logDEBUG) <<  "Initialised Game Engine.";
}

void Game::Render()
{
  camera.PreRender();
  universeManager->Render();
  camera.PostRender();
  controllerManager.get()->Render();
  guiManager.get()->Render();
}

void Game::Update(float dt)
{
  universeManager->Update(dt);
  controllerManager.get()->Update(dt);
  guiManager.get()->Update(dt);
}

void Game::Destroy()
{
  FILE_LOG(logDEBUG) <<  "Destroyed Game Engine.";
}

AssetManager* Game::GetAssetManager()
{
  return &assetManager;
}

sf::RenderWindow* Game::GetWindow()
{
  return window;
}

Camera* Game::GetCamera()
{
  return &camera;
}

GuiManager* Game::GetGuiManager()
{
  return guiManager.get();
}

ControllerManager* Game::GetControllerManager()
{
  return controllerManager.get();
}

UniverseManager* Game::GetUniverseManager()
{
  return universeManager.get();
}


