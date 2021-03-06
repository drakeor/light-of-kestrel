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

#include "basecontrol.h"
#include "../game.h"
#include "../log.h"
#include <map>

BaseControl::BaseControl(Game* game)
{
  this->game = game;
  this->parent = nullptr;
  absoluteSize = false;
  absolutePosition = false;
  this->size.x = 2;
  this->size.y = 2;
  this->internalSize = size;
  this->isEnabled = true;
  //game->GetGuiManager()->AddControlToPool(this);
}

BaseControl::~BaseControl()
{
  //game->GetGuiManager()->RemoveControlFromPool(this);
  for(std::map<std::string, BaseControl*>::iterator it = controls.begin(); it != controls.end(); ++it)
  {
    delete (*it).second;
  }
  controls.clear();
}

void BaseControl::AddControl(std::string controlName, BaseControl* control)
{
  control->SetParent(this);
  controls[controlName] = control;
}

void BaseControl::RemoveControl(std::string controlName)
{
  if(controls.find(controlName) != controls.end()) {
    delete controls[controlName];
    controls.erase(controlName);
  }
}

void BaseControl::Render()
{
  for(std::map<std::string, BaseControl*>::iterator it = controls.begin(); it != controls.end(); ++it)
  {
    (*it).second->Render();
  }
}

void BaseControl::Update(float dt)
{
  for(std::map<std::string, BaseControl*>::iterator it = controls.begin(); it != controls.end(); ++it)
  {
    (*it).second->Update(dt);
  }
}

BaseControl* BaseControl::GetControl(std::string controlName)
{
  if(controls.find(controlName) != controls.end()) {
    return controls[controlName];
  }
  return nullptr;
}

sf::Vector2f BaseControl::GetPosition()
{
  return position;
}

sf::Vector2f BaseControl::GetSize()
{
  return size;
}

void BaseControl::SetSize(float x, float y, bool absolute)
{
  internalSize = sf::Vector2f(x, y);
  absoluteSize = absolute;

  UpdatePosition();
}

void BaseControl::UpdatePosition()
{
   // If we don't have a parent, the rest of this is relavitely straight-forward, else we need to add their position.
  sf::Vector2f vecToAdd = sf::Vector2f(0, 0);
  if(parent != nullptr) {
    vecToAdd = parent->GetPosition();
  }
  
  // Now our new stuff
  if(absolutePosition == true) {
    position = internalPosition + vecToAdd;
  } else {
    if(parent != nullptr) {
      position = sf::Vector2f(parent->GetSize().x*internalPosition.x, parent->GetSize().y*internalPosition.y) + vecToAdd;
    } else {
      position = sf::Vector2f(game->GetWindow()->getSize().x*internalPosition.x, game->GetWindow()->getSize().y*internalPosition.y) + vecToAdd;
    }
  }
  
  // TODO: Set size based on absolute and relative
  size = internalSize;
  
  // Notify our children
  for(std::map<std::string, BaseControl*>::iterator it = controls.begin(); it != controls.end(); ++it)
  {
    (*it).second->UpdatePosition();
  }
}


void BaseControl::SetPosition(float x, float y, bool absolute)
{
  // Set internel position stuff
  internalPosition = sf::Vector2f(x, y);
  absolutePosition = absolute;
  
  UpdatePosition();
}

void BaseControl::SetParent(BaseControl* parent)
{
  this->parent = parent;
}

void BaseControl::CheckOnMouseClick(float x, float y)
{
  if(isEnabled) {
    for(std::map<std::string, BaseControl*>::iterator it = controls.begin(); it != controls.end(); ++it) {
      sf::Rect<float> controlContainer = sf::Rect<float>((*it).second->GetPosition().x, (*it).second->GetPosition().y, 
							(*it).second->GetSize().x, (*it).second->GetSize().y);
      //FILE_LOG(logWARNING) << controlContainer.left << "," << controlContainer.top << " | " << controlContainer.width << "," << controlContainer.height;
      if(controlContainer.contains(x, y)) {
	(*it).second->OnClick.Trigger();
      }
      (*it).second->CheckOnMouseClick(x, y);
    }
  }
}

void BaseControl::SetEnabled(bool enabled)
{
  isEnabled = enabled;
}


