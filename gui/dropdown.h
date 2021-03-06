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

#ifndef DROPDOWN_H
#define DROPDOWN_H

#include <basecontrol.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>


class Dropdown : public BaseControl
{
  std::map<std::string, std::string> values;
  std::pair<std::string, std::string> selectedPair;
  sf::RectangleShape rect;
  bool isExpanded;
  static const float sizeDif;
  sf::Text text;
  
public:
  
  // Gui Inherited Controls
  Dropdown(Game* game);
  virtual void UpdatePosition();
  virtual void SetSize(float x, float y, bool absolute = true);
  virtual sf::Vector2f GetSize();
  virtual void Update(float dt);
  virtual void Render();
  virtual void CheckOnMouseClick(float x, float y);
  virtual Dropdown& operator=(const Dropdown& other);
  
  // Dropdown Functions
  void ClearOptions();
  void AddOption(std::string identifier, std::string displayString);
  void RemoveOption(std::string identifier);
  void SetOption(std::string identifier);
  
};

#endif // DROPDOWN_H
