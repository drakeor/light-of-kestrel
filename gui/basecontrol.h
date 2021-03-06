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

#ifndef BASECONTROL_H
#define BASECONTROL_H

#include <SFML/System/Vector2.hpp>
#include <map>
#include <string>
#include "../events/eventhandler.h"

class Game;

class BaseControl
{
protected:
  Game* game;
  sf::Vector2f position;
  sf::Vector2f internalPosition;
  sf::Vector2f internalSize;
  sf::Vector2f size;
  bool absolutePosition;
  bool absoluteSize;
  std::map<std::string, BaseControl*> controls;
  BaseControl* parent;
  bool isEnabled;
  
public:
  BaseControl(Game* game);
  virtual ~BaseControl();
  void AddControl(std::string controlName, BaseControl* control);
  BaseControl* GetControl(std::string controlName);
  void RemoveControl(std::string controlName);
  void SetParent(BaseControl* parent);
  void SetPosition(float x, float y, bool absolute = true);
  void SetEnabled(bool enabled);
  sf::Vector2f GetPosition();
  virtual void UpdatePosition();
  virtual void SetSize(float x, float y, bool absolute = true);
  virtual sf::Vector2f GetSize();
  virtual void Update(float dt);
  virtual void Render();
  virtual void CheckOnMouseClick(float x, float y);
  
  EventHandler OnClick;
  EventHandler OnFocus;
  EventHandler OnDefocus;
};

#endif // BASECONTROL_H
