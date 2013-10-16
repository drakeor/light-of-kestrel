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

#include "camera.h"
#include "game.h"
#include "log.h"

Camera::Camera()
{
  this->game = nullptr;
}

Camera::Camera(Game* game, sf::FloatRect originRect)
{
  this->game = game;
  this->originRect = originRect;
}

Camera::~Camera()
{

}

void Camera::AttachCamera(sf::Vector2f* attachedPosition)
{
  isAttached = true;
  this->attachedPosition = attachedPosition;
}

void Camera::DetachCamera()
{
  isAttached = false;
  attachedPosition = nullptr;
}

sf::Vector2f Camera::GetPosition()
{
  return position;
}

void Camera::SetPosition(sf::Vector2f newPosition)
{
  position = newPosition;
}


// Sets our camera to the camera position in the 2D world.
void Camera::PreRender()
{
  view = game->GetWindow()->getDefaultView();
  view.move(position);
  game->GetWindow()->setView(view);
}

// Resets the camera back to the base position for drawing GUI elements
void Camera::PostRender()
{
  view = game->GetWindow()->getDefaultView();
  game->GetWindow()->setView(view);
}


