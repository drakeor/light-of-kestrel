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
#if _WIN32
  #include <memory>
#else
  #include <tr1/memory>
#endif
#include <map>
#include <../components/entitycomponent.h>
#include "missilefactory.h"
#include <ai/baseai.h>
#include <ai/relationshipmanager.h>

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
  BACK,
  RIGHT,
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
  std::string description;
  std::string entityIcon;
  Galaxy* parent;
  std::unique_ptr<BaseAI> ai;
  RelationshipManager::FACTION faction;
  
  // Physiks Variables.
  // Anmerkung: Die Einheiten (Anlagen?) bist im Pixelen / Sekunden
  sf::Vector2f position;
  float velocityMagnitude;
  float currentRotation;
  float maxRotationalSpeed;
  float maxLinearSpeed;
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
  
  /*
   * Common functions
   */
  void Initialise();
  void Update(float dt);
  void Render();
  void Iterate(float dt);
  void CommitTurn(float frameTime, float maxTime);
  
  /*
   * Set functions
   */
  void SetPosition(int x, int y);
  void SetCurrentVelocity(float target);
  
  /*
   * Sets the current rotation in radians
   */
  void SetCurrentRotation(float target);
  
  /*
   * Hardsets the target rotation. It's not recommended to do this
   */
  void SetTargetVelocity(float target);
  
  /* Sets the target delta speed. Useful for saying.
   * "I want to move this way at 200uph"
   * This cannot exceed the max linear speed
   */
  void SetDeltaVelocity(float target);
  
  /*
   * Hardsets the target rotation. It's not recommended to do this
   */
  void SetTargetRotation(float target);
  
  /*
   * Sets the target delta rotation. Useful for saying
   * "I want to move 1.41 radians this way" SetDeltaRotation(1.41f).
   * This cannot exceeded the maximum rotational limit.
   */
  void SetDeltaRotation(float target);
  
  /*
   * Sets the max rotation speed
   */
  void SetMaxRotationalSpeed(float target);
  
  /*
   * Gets the max rotation speed in radians
   */
  float GetMaxRotationalSpeed();
  
  void SetSprite(sf::Sprite sprite);
  void SetName(std::string newName);
  std::string GetName();
  std::string GetDescription();
  void SetDescription(std::string description);
  std::string GetIcon();
  void SetIcon(std::string name);
  
  /*
   * Sets the rotation offset. Useful for correcting textures.
   * The offset is added to the core rotation right before rendering.
   */
  void SetTextureRotOffset(float offset);
  
  /*
   * Sets the ID of the entity in the galaxyManager
   * Only the entityManager should set this variable.
   * Do not set it directly unless you need too.
   */
  void SetId(int id);
  
  /*
   * Sets which faction this entity belongs too
   * for calculating friends and enemies
   */
  void SetFaction(RelationshipManager::FACTION newFaction);
  
  /*
   * Sets which galaxy owns this entity.
   */
  void SetParent(Galaxy* newParent);
  
  /*
   * Sets an AI on top of this entity.
   * The entity assumes control of managing the AI memory.
   */
  void InstallAI(BaseAI* aiSys);
  
  /*
   * Get Functions
   */
  int GetId();
  int GetHealth();
  Game* GetGame();
  RelationshipManager::FACTION GetFaction();
  BaseAI* GetAI();
  Galaxy* GetParent();
  sf::Sprite* GetSprite();
  sf::Vector2f GetCurrentPosition();
  float GetCurrentVelocity();
  
  /*
   * Returns the current rotation in radians
   */
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
  
  /*
   * This will return how many missile bays this entity has.
   * Note this can be both fired and reloading BUT WILL NOT included destroyed.
   */
  int GetMissileBays();
  
  // Missiles fired from the last turn
  int missilesFired;
};

#endif // ENTITY_H
