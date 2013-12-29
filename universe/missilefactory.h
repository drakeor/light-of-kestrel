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

#ifndef MISSILEFACTORY_H
#define MISSILEFACTORY_H
#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics/Sprite.hpp>

enum missile_t {
  MISSILE_NONE,
  MISSILE_VEILLON_I,
  MISSILE_SHIVE_I,
  MISSILE_MIRA_I
};

enum missile_flight_t {
  // Dumbfires
  DUMBFIRE,
  
  // Dumb Guided that slowly adjusts to the closest target.
  DUMB_GUIDED,
  
  // Radar Guided that locks onto ships and is more agile then dumb_guided
  RADAR_GUIDED,
};

class Missile
{
public:
  std::string missileName;
  int damage;
  missile_flight_t missileFlight;
  std::string missileImage;
};

class MissileFactory
{
public:
  MissileFactory();
  static Missile GetMissile(missile_t identifier);
  virtual ~MissileFactory();
};

#endif // MISSILEFACTORY_H
