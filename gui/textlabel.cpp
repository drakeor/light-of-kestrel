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

#include "textlabel.h"
#include "../game.h"
#include "../log.h"

TextLabel::TextLabel(Game* game)
  : BaseControl(game)
{
  text.setFont(*game->GetAssetManager()->GetFont("gfx/fonts/sansation.ttf")); // font is a sf::Font
  text.setString("Unlabeled");
  text.setCharacterSize(24); // in pixels, not points!
  text.setColor(sf::Color::Red);
  text.setStyle(sf::Text::Bold);
}


TextLabel::~TextLabel()
{

}

void TextLabel::Render()
{
  BaseControl::Render();
  game->GetWindow()->draw(text);
}

sf::Text* TextLabel::GetText()
{
  return &text;
}

void TextLabel::SetText(std::string newText)
{
  text.setString(sf::String(newText));
}

void TextLabel::UpdatePosition()
{
  BaseControl::UpdatePosition();
  text.setPosition(position.x, position.y);
}





