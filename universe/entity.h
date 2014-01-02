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

#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics/Sprite.hpp>
#include <tr1/memory>
#include <map>
#include <../components/entitycomponent.h>
#include "missilefactory.h"
#include <ai/baseai.h>

class Galaxy;
class Game;

// Collision groups! With bitflags!
enum collision_group_t {
  
  /* Space ships will belong to this one */
  COLLISION_GROUP_1 = 1, 
  
  /* Missles will belong to this one */
  COLLISION_GROUP_2 = 2, 
  
  COLLISION_GROUP_3 = 4, 
  COLLISION_GROUP_4 = 8, 
  COLLISION_GROUP_5 = 16,
  COLLISION_GROUP_6 = 32,
  COLLISION_GROUP_7 = 64,
  COLLISION_GROUP_8 = 128
};

/* Max Component sides = 5 */
enum component_side_t {
  FRONT,
  LEFT,
  RIGHT,
  BACK,
  CORE,
  MAX_COMPONENT_SIDES
};

/* Max Components Layers = 3 */
enum component_layer_t {
  LAYER_1,
  LAYER_2,
  LAYER_3,
  MAX_COMPONENT_LAYERS
};

// Entities belong to the galaxys they are part of. 
class Entity
{
  //Beginnen Variables
  sf::Sprite texture;
  float texRotOffset;
  Game* game;
  std::string name;
  std::string entityIcon;
  Galaxy* parent;
  std::unique_ptr<BaseAI> ai;
  
  // Physiks Variables.
  // Anmerkung: Die Einheiten (Anlagen?) bist im Pixelen / Sekunden
  sf::Vector2f position;
  float velocityMagnitude;
  float currentRotation;
  int id;
  
  // Simulation Variables
  float startThrust;
  float startRotation;
  float targetThrust;
  float targetRotation;
  float deltaThrust;
  float deltaRotation;
  float frameTime;
  float maxTime;
  
  // Missile Simulation variables
  float missileDelayTime;
  
  // Entity Components
  std::vector<EntityComponent> components[MAX_COMPONENT_SIDES][MAX_COMPONENT_LAYERS];
  std::vector<missile_t> missiles;
  std::vector<missile_t> activeMissiles;
  int health;
  
public:
  Entity(Game* game);
  void Initialise();
  void Update(float dt);
  void Render();
  void Iterate(float dt);
  void CommitTurn(float frameTime, float maxTime);
  
  void SetPosition(int x, int y);
  void SetCurrentVelocity(float target);
  void SetCurrentRotation(float target);
  void SetTargetVelocity(float target);
  void SetTargetRotation(float target);
  void SetSprite(sf::Sprite sprite);
  void SetName(std::string newName);
  std::string GetName();
  std::string GetIcon();
  void SetIcon(std::string name);
  void SetTextureRotOffset(float offset);
  void SetId(int id);
  int GetId();
  Game* GetGame();
  
  void InstallAI(BaseAI* aiSys);
  BaseAI* GetAI();
  
  void SetParent(Galaxy* newParent);
  Galaxy* GetParent();
  
  sf::Sprite* GetSprite();
  sf::Vector2f GetCurrentPosition();
  float GetCurrentVelocity();
  float GetCurrentRotation();
  virtual ~Entity();
  
  // Physiks Abfall
  char collidesWith;
  char collisionGroup;
  int TempCollisionDistance;
  void OnCollision(Entity* other);
  void ProcessImpact(float damage, float force, float direction);
  
  // Component Related
  void AddComponent(component_side_t componentSide, component_layer_t componentLayer, EntityComponent entComponent);
  std::vector<EntityComponent> GetComponents(component_side_t componentSide, component_layer_t componentLayer);
  
  // Missile related
  void AddMissile(missile_t missile, int amount);
  std::vector<missile_t> GetMissiles();
  bool HasMissile(missile_t missile);
  void FireMissile(missile_t missile);
};

#endif // ENTITY_H
