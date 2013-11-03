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

#include "missilefactory.h"

MissileFactory::MissileFactory()
{

}

MissileFactory::~MissileFactory()
{

}

Missile MissileFactory::GetMissile(missile_t identifier)
{
  Missile tMissile;
  
  /* The Veillon I is the most basic of missiles. It's dumbfire but deals a good amount of damage against lower tier ships */
  tMissile.missileName = "Invalid missile";
  tMissile.missileFlight = DUMBFIRE;
  tMissile.damage = 0;
  
  switch(identifier) {
    
    case MISSILE_VEILLON_I:
      /* The Veillon I is the most basic of missiles. It's dumbfire but deals a good amount of damage against lower tier ships */
      tMissile.missileName = "Veillon I";
      tMissile.missileFlight = DUMBFIRE;
      tMissile.damage = 350;
      break;
    /* The Shive I is a basic dumb-guided missile that steers towards the closest target. Only does slightly less damage */
    case MISSILE_SHIVE_I:
      tMissile.missileName = "Shive I";
      tMissile.missileFlight = DUMB_GUIDED;
      tMissile.damage = 300;
      break;
    
    /* The Mira I is the basic smart guided missile. Although more agile and intelligent then the Shive, it deals the least amount of damage.
	making it useful for finishing off targets. */
    case MISSILE_MIRA_I:
      tMissile.missileName = "Mira I";
      tMissile.missileFlight = RADAR_GUIDED;
      tMissile.damage = 250;
      break;
  };
  return tMissile;
}


