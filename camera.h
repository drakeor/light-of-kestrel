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

#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>

class Game;

class Camera
{
  sf::FloatRect originRect;
  sf::Vector2f position;
  bool isAttached;
  sf::Vector2f* attachedPosition;
  Game* game;
  sf::View view;
  
public:
  Camera();
  Camera(Game* game, sf::FloatRect originRect);
  void SetPosition(sf::Vector2f newPosition);
  void AttachCamera(sf::Vector2f* attachedPosition);
  void DetachCamera();
  void PreRender();
  void PostRender();
  void SetView(sf::View view);
  sf::Vector2f GetPosition();
  virtual ~Camera();
};

#endif // CAMERA_H
