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

#include "button.h"
#include "../game.h"
#include <SFML/Graphics/RenderWindow.hpp>

void Button::Update(float dt)
{
  rect.setOutlineThickness(1.0);
  rect.setOutlineColor(sf::Color::Blue);
  rect.setFillColor(sf::Color::Black);
  rect.setSize(GetSize());
  rect.setPosition(GetPosition());
  text.setPosition(position.x + 5, position.y);
  BaseControl::Update(dt);
}

void Button::Render()
{
  game->GetWindow()->draw(rect);
  game->GetWindow()->draw(text);
  BaseControl::Render();
}

Button::Button(Game* game)
  : BaseControl(game) {
  this->position.x = 20;
  this->position.y = 20;
  this->size.x = 100;
  this->size.y = 25;
  this->internalSize = size;
  text.setFont(*game->GetAssetManager()->GetFont("sansation.ttf")); // font is a sf::Font
  text.setString("Button");
  text.setCharacterSize(18); // in pixels, not points!
  text.setColor(sf::Color::White);
  text.setStyle(sf::Text::Bold);
  
}

Button::~Button()
{

}

