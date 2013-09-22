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

void Game::Initialise(sf::RenderWindow* window)
{
  this->window = window;
  universeManager = new(std::nothrow) UniverseManager(this);
  guiManager = std::tr1::shared_ptr<GuiManager>( new GuiManager(this) );
  universeManager->Initialise();
  FILE_LOG(logDEBUG) <<  "Initialised Game Engine.";
}

void Game::Render()
{
  universeManager->Render();
}

void Game::Update(float dt)
{
  universeManager->Update(dt);
}

void Game::Destroy()
{
  delete universeManager;
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


