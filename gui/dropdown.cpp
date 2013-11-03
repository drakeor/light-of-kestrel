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

#include "dropdown.h"
#include "../game.h"

Dropdown::Dropdown(Game* game) : BaseControl(game)
{
  this->size.x = 180;
  this->size.y = sizeDif;
  this->internalSize = size;
  isExpanded = false;
  
  // Set up our initial rectangle.
  text.setFont(*game->GetAssetManager()->GetFont("sansation.ttf")); // font is a sf::Font
  text.setString("Button");
  text.setCharacterSize(16); // in pixels, not points!
  text.setColor(sf::Color::White);
  text.setStyle(sf::Text::Bold);
  selectedPair = std::pair<std::string, std::string>("","");
}

void Dropdown::UpdatePosition()
{
BaseControl::UpdatePosition();
}

void Dropdown::SetSize(float x, float y, bool absolute)
{
  BaseControl::SetSize(x, y, absolute);
}

sf::Vector2f Dropdown::GetSize()
{
  return BaseControl::GetSize();
}

void Dropdown::Update(float dt)
{
  rect.setOutlineThickness(3.0);
  rect.setOutlineColor(sf::Color::Blue);
  rect.setFillColor(sf::Color::Black);
  rect.setSize(sf::Vector2f(GetSize().x, GetSize().y));
  rect.setPosition(GetPosition().x, GetPosition().y);
  BaseControl::Update(dt);
}

void Dropdown::Render()
{
  // Draw the initial thingy
  rect.setOutlineColor(sf::Color::Green);
  text.setPosition(position.x, position.y);
  text.setString(selectedPair.second);
  game->GetWindow()->draw(rect);
  game->GetWindow()->draw(text);
    
  // Draw the rectangle
  if(isExpanded) {
    int i = 1;
    for (auto it=values.begin(); it!=values.end(); ++it) {
      rect.setOutlineColor(sf::Color::Blue);
      text.setPosition(position.x, position.y + (i*sizeDif));
      rect.move(0, sizeDif);
      text.setString(it->second);
      game->GetWindow()->draw(rect);
      game->GetWindow()->draw(text);
      ++i;
    }
    
  }
  BaseControl::Render();
}

void Dropdown::CheckOnMouseClick(float x, float y)
{
  sf::Rect<float> tempRect = sf::Rect<float>(GetPosition().x, GetPosition().y, GetSize().x, GetSize().y);
  if(tempRect.contains(x, y)) {
    if(!isExpanded) isExpanded = true;
  }
  // Select an option if it's expanded. 
  if(isExpanded) {
    int i=1;
    for (auto it=values.begin(); it!=values.end(); ++it) {
      sf::Rect<float> tempRect2 = sf::Rect<float>(GetPosition().x, GetPosition().y+(sizeDif*i), GetPosition().x+GetSize().x, GetPosition().y+(sizeDif*(i+1)));
      if(tempRect2.contains(x, y)) {
	selectedPair = (*it);
	isExpanded = false;
	//break;
      }
      ++i;
    }
  }
  BaseControl::CheckOnMouseClick(x, y);
}


Dropdown& Dropdown::operator=(const Dropdown& other)
{
  return *this;
}

bool Dropdown::operator==(const Dropdown& other) const
{
///TODO: return ...;
}

void Dropdown::AddOption(std::string identifier, std::string displayString)
{
  values[identifier] = displayString;
}

void Dropdown::ClearOptions()
{
 values.clear();;
}

void Dropdown::RemoveOption(std::string identifier)
{
  values.erase(identifier);
}

