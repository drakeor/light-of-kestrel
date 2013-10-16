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

#include "slider.h"
#include "../game.h"
#include "../log.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

Slider::Slider(Game* game) :
  BaseControl(game)
{
  this->size.x = 100;
  this->size.y = 25;
  this->internalSize = size;
  this->isHorizontal = true;
}

void Slider::Update(float dt)
{
  // <code>
  sf::Vector2i calibrationVec = sf::Vector2i(0, 20);
  // If it corresponds to our control, we want to capture it.
  if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    sf::Vector2f vec2 = game->GetWindow()->mapPixelToCoords(sf::Mouse::getPosition((*game->GetWindow())));
    //float x = sf::Mouse::getPosition((*game->GetWindow())).x + calibrationVec.x;
    //float y = sf::Mouse::getPosition((*game->GetWindow())).y + calibrationVec.y;
    float x = vec2.x;
    float y = vec2.y;
    sf::Rect<float> controlContainer = sf::Rect<float>(GetPosition().x, GetPosition().y, GetSize().x, GetSize().y);
    if(controlContainer.contains(x, y)) {
      sf::Vector2f relativeClick = sf::Vector2f((x - GetPosition().x)/GetSize().x, (y - GetPosition().y)/GetSize().y);
      if(isHorizontal) this->fillPercentage = relativeClick.x;
      else this->fillPercentage = 1-relativeClick.y;
      //FILE_LOG(logWARNING) << "Percentage: " << this->fillPercentage;
    }
  }
  // </code>
  
  // Set up our rectangle
  rect.setOutlineThickness(3.0);
  rect.setOutlineColor(sf::Color::Blue);
  rect.setFillColor(sf::Color::Blue);
  if(isHorizontal) {
    rect.setSize(sf::Vector2f(GetSize().x*fillPercentage, GetSize().y));
    rect.setPosition(GetPosition());
  } else {
    rect.setSize(sf::Vector2f(GetSize().x, -GetSize().y*fillPercentage));
    rect.setPosition(GetPosition().x, GetPosition().y+GetSize().y);
  }
 
  BaseControl::Update(dt);
}

void Slider::Render()
{
  game->GetWindow()->draw(rect);
  // If we have a half mark, show it.
  if(showHalf) {
    sf::VertexArray lines(sf::LinesStrip, 2);
    if(isHorizontal) {
      lines[0].position = sf::Vector2f(GetPosition().x + GetSize().x/2, GetPosition().y);
      lines[1].position = sf::Vector2f(GetPosition().x + GetSize().x/2, GetPosition().y + GetSize().y);
    } else {
      lines[0].position = sf::Vector2f(GetPosition().x, GetPosition().y + GetSize().y/2);
      lines[1].position = sf::Vector2f(GetPosition().x + GetSize().x, GetPosition().y + GetSize().y/2);
    }
    game->GetWindow()->draw(lines);
  }
  BaseControl::Render();
}

void Slider::CheckOnMouseClick(float x, float y)
{
  BaseControl::CheckOnMouseClick(x, y);
}

void Slider::SetValue(int min, int max)
{
  this->minValue = min;
  this->maxValue = max;
}

float Slider::GetFillPercentage()
{
  return fillPercentage;
}

void Slider::SetFillPercentage(float newPercentage) {
  fillPercentage = newPercentage;
}

void Slider::SetHorizontal(bool newValue) {
 isHorizontal = newValue;
}

void Slider::ShowHalfMark(bool showHalf) {
  this->showHalf = showHalf;
}

