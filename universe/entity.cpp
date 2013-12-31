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

#include "entity.h"
#include "entityfactory.h"
#include "../game.h"
#include "../log.h"
#include "../components/missilebay.h"
#include <controllers/settings.h>

Entity::Entity(Game* game) :
  position(sf::Vector2f(0.0f, 0.0f)), currentRotation(0), startRotation(0),
  targetThrust(0),  velocityMagnitude(0), targetRotation(0),
  health(100), startThrust(0), deltaRotation(0), frameTime(0),
  deltaThrust(0), maxTime(0), missileDelayTime(0), texRotOffset(0),
  id(0)
{
  this->game = game;
  this->position = sf::Vector2f(0.0f, 0.0f);
}

Entity::~Entity()
{

}

void Entity::Initialise() {
  // Initial assigning
  sf::Texture* tempTex = game->GetAssetManager()->GetTexture("dragon.png");
  texture = sf::Sprite(*tempTex);
  texture.setOrigin(tempTex->getSize().x/2, tempTex->getSize().y/2);
  texture.setPosition(200, 100);
  position = sf::Vector2f(1.0f, 1.0f);
  currentRotation = 0;
}

void Entity::Update(float dt) {
 /* FILE_LOG(logERROR) << "UPDATE Rot: " << currentRotation;
  FILE_LOG(logERROR) << "UPDATE Position: " << position.x << "|" << position.y;
  // Fix NaN issues. Temp fix.
  if(isnan(position.x) || isnan(position.y)) {
    position = sf::Vector2f(0, 0);
    FILE_LOG(logERROR) << "Entity has NaN position!!";
  }
  if(isnan(currentRotation)) {
    currentRotation = 0.1f;
    FILE_LOG(logERROR) << "Entity has NaN rotation!!";
  }*/
}

void Entity::SetId(int id)
{
  this->id = id;
}


void Entity::Render() {
  float realRotation = (currentRotation+texRotOffset)*57.3;
  realRotation = fmod(realRotation, 360.0f);
  texture.setPosition(position);
  texture.setRotation(realRotation);
  this->game->GetWindow()->draw(texture);
}

void Entity::CommitTurn(float frameTime, float maxTime)
{
  this->frameTime = frameTime;
  this->maxTime = maxTime;
  startRotation = currentRotation;
  startThrust = velocityMagnitude;
  deltaThrust = (targetThrust - startThrust) / maxTime;
  deltaRotation = (targetRotation - startRotation) / maxTime;
  /*FILE_LOG(logERROR) << "DeltaThrust: " << deltaThrust << " | " << targetThrust << " | " << startThrust << 
  " deltaRotation " << deltaRotation << " | " << targetRotation << " | " << startRotation;*/
}

void Entity::Iterate(float dt) {
  // Update positions and rotations.
  velocityMagnitude += deltaThrust * dt;
  currentRotation += deltaRotation * dt;
  //FILE_LOG(logERROR) << "Before iterate position: " << position.x << "|" << position.y;
  float cx = (float)cos(currentRotation);
  float cy = (float)sin(currentRotation);
  //FILE_LOG(logERROR) << "Rotation:" << currentRotation << " cx: " << cx << " cy: " << cy <<
    //" Velmag: " << velocityMagnitude << " DThrust: " << deltaThrust;
  position = sf::Vector2f(position.x + (cx*velocityMagnitude*dt),
			  position.y + (cy*velocityMagnitude*dt));
  //FILE_LOG(logERROR) << "After iterate position: " << position.x << "|" << position.y;
  // Fire missiles if we can.
  if(!activeMissiles.empty()) {
    if(missileDelayTime < 0) {
      // Create missile here.
      // We're going to queue it up with a slight offset.
      missile_t tMissile = activeMissiles.front();
      activeMissiles.erase(activeMissiles.begin());
      auto ent = EntityFactory::BuildEntity(game, MISSILE);
      auto misInfo = MissileFactory::GetMissile(tMissile);
      EntityFactory::BuildTexture(game, ent, misInfo.missileImage);
      ent->SetTextureRotOffset(1.57f);
      ent->SetPosition(position.x + (cx*(TempCollisionDistance+9)), position.y + (cy*(TempCollisionDistance+9)));
      ent->SetCurrentRotation(currentRotation);
      ent->SetTargetRotation(currentRotation);
      ent->SetCurrentVelocity(350);
      ent->SetTargetVelocity(350);
      ent->AddMissile(tMissile, 1);
      game->GetUniverseManager()->GetCurrentGalaxy()->QueueEntityForTurn(0.00f, ent);
      missileDelayTime = GameSettings::missileFireDelayTime;
    } else {
      missileDelayTime -= dt;
    }
  } else {
    missileDelayTime = -0.01f;
  }
}

sf::Vector2f Entity::GetCurrentPosition() {
  //FILE_LOG(logERROR) << "Get Position: " << position.x << "|" << position.y;
  return position;
}

float Entity::GetCurrentRotation() {
  return currentRotation;
}

float Entity::GetCurrentVelocity() {
  return velocityMagnitude;
}

void Entity::SetCurrentRotation(float target)
{
  currentRotation = target;
}

void Entity::SetCurrentVelocity(float target)
{
  velocityMagnitude = target;
}


sf::Sprite* Entity::GetSprite() {
  return &texture;
}


void Entity::SetSprite(sf::Sprite sprite) {
  this->texture = sprite;
}


void Entity::SetPosition(int x, int y) {
  //FILE_LOG(logERROR) << "Set Position: " << position.x << "|" << position.y << " || " << x << "|" << y;
  position = sf::Vector2f(x, y);
}

void Entity::SetTargetRotation(float target) {
  //FILE_LOG(logERROR) << "Target Rotation: " << target;
  targetRotation = target;
}

void Entity::SetTargetVelocity(float target) {
  //FILE_LOG(logERROR) << "Target Velocity: " << target;
  targetThrust = target;
}

void Entity::SetName(std::string newName) {
  this->name = newName;
}


std::string Entity::GetName()
{
  return name;
}


Galaxy* Entity::GetParent()
{
  return parent;
}

void Entity::SetParent(Galaxy* newParent)
{
  parent = newParent;
}

void Entity::OnCollision(Entity* other)
{
  // If I'm a missile, then give the entity I just collided with a world of hurt!
  if(name == "Missile") {
    
    // If the programmer was competent, then he'd have a missle inside this missile (Missile-ception!)
    if(missiles.size() < 1) { 
      FILE_LOG(logERROR) << "Entity error on collision: This missile entity has no missile assigned to it!";
    }
    Missile misInfo = MissileFactory::GetMissile(missiles[0]);
    
    /* We want to process the impact to the ship. 
     * Get the angle of impact to the ship by finding the angle between the
     * missile and the ship
     */
    float deltaY = position.y - other->GetCurrentPosition().y;
    float deltaX = position.x - other->GetCurrentPosition().x;
    float resAngle = atan2(deltaY, deltaX);
    // TODO: Add pushback force
    other->ProcessImpact(misInfo.damage, 1, resAngle);
    // Lastly, kill ourselves.
    game->GetUniverseManager()->GetCurrentGalaxy()->DeleteEntity(id);
  }
}

void Entity::ProcessImpact(float damage, float force, float direction)
{
  FILE_LOG(logERROR) << "I just recieved an impact of " << damage << " from direction " << direction;
}

void Entity::AddComponent(component_side_t componentSide, component_layer_t componentLayer, EntityComponent entComponent)
{
  components[componentSide][componentLayer].push_back(entComponent);
}

std::vector< EntityComponent > Entity::GetComponents(component_side_t componentSide, component_layer_t componentLayer)
{
  return components[componentSide][componentLayer];
}


std::string Entity::GetIcon()
{
  return entityIcon;
}

void Entity::SetIcon(std::string name)
{
  entityIcon = name;
}

void Entity::AddMissile(missile_t missile, int amount)
{
  missiles.insert(missiles.begin(), amount, missile);
}

void Entity::FireMissile(missile_t missile)
{
  // This will queue up the missile to be fired and remove it from active missiles
  for (auto sMissile = missiles.begin(); sMissile != missiles.end(); sMissile++) {
    if((*sMissile) == missile) {
      activeMissiles.push_back((*sMissile));
      missiles.erase(sMissile);
      break;
    }
  }
}

std::vector< missile_t > Entity::GetMissiles()
{
  return missiles;
}

bool Entity::HasMissile(missile_t missile)
{
  for (auto sMissile = missiles.begin(); sMissile != missiles.end(); sMissile++) {
    if((*sMissile) == missile) {
      return true;
      break;
    }
  }
  return false;
}

void Entity::SetTextureRotOffset(float offset)
{
  texRotOffset = offset;
}



