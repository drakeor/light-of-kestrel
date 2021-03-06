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

#include "armourplating.h"

ArmourPlating::ArmourPlating(armour_class_t armourClass)
{
  icon = "gfx/icons/class1_armour.png";
  negateCoefficient = 0.01;
  switch(armourClass) {
    /* Steel armour is the most basic armour money can buy. It makes up a majority of ships. 
	It negates 25% of damage to itself and 100% absorption rate.. */
    case ARMOUR_STEEL:
      negateCoefficient = 0.25;
      name = "Steel Armour";
  }
}

void ArmourPlating::Iterate(float dt)
{
EntityComponent::Iterate(dt);
}

void ArmourPlating::Render()
{
EntityComponent::Render();
}

void ArmourPlating::Update(float dt)
{
EntityComponent::Update(dt);
}

