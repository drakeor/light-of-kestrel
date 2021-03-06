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

#include "controllermanager.h"
#include "basecontroller.h"
#include "../game.h"

ControllerManager::ControllerManager(Game* game)
{
  this->game = game;
}

ControllerManager::~ControllerManager()
{
  for(std::map<std::string, BaseController*>::iterator it = controllers.begin(); it != controllers.end(); ++it)
  {
    delete (*it).second;
  }
  controllers.clear();
}

void ControllerManager::AddController(std::string name, BaseController* controller)
{
  controllers[name] = controller;
}

BaseController* ControllerManager::GetController(std::string name)
{
  if(controllers.find(name) != controllers.end())
    return controllers[name];
  return nullptr;
}

void ControllerManager::Render()
{
  for(std::map<std::string, BaseController*>::iterator it = controllers.begin(); it != controllers.end(); ++it)
  {
    (*it).second->Render();
  }
}

void ControllerManager::Update(float dt)
{
  for(std::map<std::string, BaseController*>::iterator it = controllers.begin(); it != controllers.end(); ++it)
  {
    (*it).second->Update(dt);
  }
}

