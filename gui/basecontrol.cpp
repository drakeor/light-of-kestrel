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
#include <map>

BaseControl::BaseControl(Game* game)
{
  this->game = game;
}

BaseControl::~BaseControl()
{
  for(std::map<std::string, BaseControl*>::iterator it = controls.begin(); it != controls.end(); ++it)
  {
    delete (*it).second;
  }
  controls.clear();
}

void BaseControl::AddControl(std::string controlName, BaseControl* control)
{
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
  
}

void BaseControl::Update(float dt)
{
  
}


