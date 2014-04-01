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
#include <settings.h>
#include <ai/missileai.h>
#include <controllers/console.h>
#include <controllers/myplayercontroller.h>
#include <iostream>
#include <cmath>


/*
 * Nice, big constructor
 */
Entity::Entity(Game* game) :
  position(sf::Vector2f(0.0f, 0.0f)), currentRotation(0), startRotation(0),
  targetThrust(0),  velocityMagnitude(0), targetRotation(0),
  health(100), startThrust(0), deltaRotation(0), frameTime(0),
  deltaThrust(0), maxTime(0), missileDelayTime(0), texRotOffset(0),
  id(0), missilesFired(0), maxRotationalSpeed(1.7f)
{
  this->game = game;
  this->faction = RelationshipManager::FACTIONLESS;
  this->position = sf::Vector2f(0.0f, 0.0f);
}

Entity::~Entity() { }

int Entity::GetHealth()
{
  return health;
}

void Entity::Initialise() {
  
  // Initial assigning.
  // We're going to set a default texture for now.
  sf::Texture* tempTex = game->GetAssetManager()->GetTexture("gfx/interface/invalid.png");
  texture = sf::Sprite(*tempTex);
  texture.setOrigin(tempTex->getSize().x/2, tempTex->getSize().y/2);
  texture.setPosition(200, 100);
  position = sf::Vector2f(1.0f, 1.0f);
  currentRotation = 0;
}

void Entity::Update(float dt) {

}

void Entity::SetId(int id)
{
  this->id = id;
}

Game* Entity::GetGame()
{
  return this->game;
}

void Entity::SetFaction(RelationshipManager::FACTION newFaction)
{
  this->faction = newFaction;
}

RelationshipManager::FACTION Entity::GetFaction()
{
  return this->faction;
}


void Entity::Render() {
  // For some reason, SFML likes degrees so we need to convert.
  float realRotation = (currentRotation+texRotOffset)*57.3;
  realRotation = fmod(realRotation, 360.0f);
  texture.setPosition(position);
  texture.setRotation(realRotation);
  this->game->GetWindow()->draw(texture);
}

void Entity::CommitTurn(float frameTime, float maxTime)
{
  // Reset the missile counter
  missilesFired = 0;
  this->frameTime = frameTime;
  this->maxTime = maxTime;
  
  // Calculate the ending velocity and rotations.
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
      
      // Create our missile entity.
      EntityFactory::BuildTexture(game, ent, misInfo.missileImage);
      ent->SetTextureRotOffset(1.57f);
      ent->SetPosition(position.x + (cx*(TempCollisionDistance+9)), position.y + (cy*(TempCollisionDistance+9)));
      ent->SetCurrentRotation(currentRotation);
      ent->SetTargetRotation(currentRotation);
      ent->SetCurrentVelocity(350);
      ent->SetTargetVelocity(350);
      ent->AddMissile(tMissile, 1);
      ent->SetFaction(faction);
      
      
      if((misInfo.missileFlight == DUMB_GUIDED) || (misInfo.missileFlight == RADAR_GUIDED)) {
	
	// Install AI into the missile if it's seeking.
	ent->InstallAI(new MissileAI());
	
	// Fix for Missile AI taking time to warm up
	ent->GetAI()->ProcessTurn();
	ent->CommitTurn(GameSettings::frameTime, GameSettings::maxTime);
      }
      
      // We need to delay fire the missile
      game->GetUniverseManager()->GetCurrentGalaxy()->QueueEntityForTurn(0.00f, ent);
      missileDelayTime = GameSettings::missileFireDelayTime;
      ++missilesFired;
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

int Entity::GetId()
{
  return id;
}


void Entity::ProcessImpact(float damage, float force, float direction)
{
  // We first want to grab the console and player controllers.
  Console* cons = (Console*)game->GetControllerManager()->GetController("Console");
  MyPlayerController* mpc = (MyPlayerController*)game->GetControllerManager()->GetController("PlayerController");
      
  /* Check the direction of impact */
  float deltaRotation = currentRotation - direction;
  deltaRotation = fabs(fmod(deltaRotation, 6.28f));
  FILE_LOG(logERROR) << "I just recieved an impact of " << damage << " from direction " << direction << " (local: " << deltaRotation << ")";
  
  /* Check which components were hit */
  float damages[4] = {0,0,0,0};
  float dPercents[4] = {0,0,0,0};
  for(int i=0; i < 4; i++) {
    if((deltaRotation < (1.57f*(i+1)))  && (deltaRotation > (1.57f*i))) {
      int j=i+1;
      if(j == 4) j = 0;
      dPercents[j] = (deltaRotation-(1.57f*(i)))/1.57f;
      dPercents[i] = 1 - dPercents[j];
      damages[i] = dPercents[i] * damage;
      damages[j] = dPercents[j] * damage;
    }
  }
  FILE_LOG(logERROR) << "Actual Damage: Front: " << damages[0] << "  Right: " << damages[1] << "  Back: " << damages[2] << "  Left: " << damages[3];
  FILE_LOG(logERROR) << "Percentages: Front: " << dPercents[0] << " Right: " << dPercents[1] << " Back: " << dPercents[2] << " Left:  " << dPercents[3];
  /* Deal damage to the components now */
  for(int j=0;j<MAX_COMPONENT_SIDES;++j) {
    for(int i=0;i<MAX_COMPONENT_LAYERS;++i) {
      for(auto it = components[j][i].begin(); it != components[j][i].end(); ++it) {
	if(damages[j] > 0) {
	  if((*it).health > damages[j]) {
	    // This component neutralised all the damage.
	    (*it).health -= damages[j];
	    damages[j] = 0;
	    break;
	  } else {
	    // This component was destroyed in the process
	    damages[j] -= (*it).health;
	    (*it).health = 0;
	    if(this->id == mpc->GetPlayer()->GetId()) {
		std::stringstream ss;
		ss << (*it).name << " destroyed.";
		cons->AddLine(ss.str());
	    }
	  }
	}
      }
    }
    /* Damage leaked through to the core */
    if(damages[j] > 0) {
      this->health -= damages[j];
      
      if(this->id == mpc->GetPlayer()->GetId()) {
	  std::stringstream ss;
	  ss << "WARNING: Core sustained " << ceil(damages[j]) << " damage (HP: " << ceil(this->health) << "%)";
	  cons->AddLine(ss.str());
	  if(health < 0) {
	    cons->AddLine("CRITICAL: Core integrity compromised");
	  }
      }
      FILE_LOG(logWARNING) << "Took damage from side " << j << " to core for " << damages[j] << " damage";
    }
  }
  /* If our core suffered too much damage, destroy ourselves */
  if(this->health < 0) {
    game->GetUniverseManager()->GetCurrentGalaxy()->DeleteEntity(this->id);
  }
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
  // This will queue up the missile to be fired and remove it from active missiles ONLY if we have an active bay.
  int readyMissleBays = GetMissileBays();
  readyMissleBays -= activeMissiles.size();
  
  if(readyMissleBays > 0) {
    for (auto sMissile = missiles.begin(); sMissile != missiles.end(); sMissile++) {
      if((*sMissile) == missile) {
	activeMissiles.push_back((*sMissile));
	missiles.erase(sMissile);
	return;
	break;
      }
    }
    FILE_LOG(logWARNING) << "Warning: Entity " << GetName() << " tried to fire Missile ID " << (int)missile << " but failed.";
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


void Entity::InstallAI(BaseAI* aiSys)
{
  aiSys->SetEntity(this);
  ai = std::unique_ptr<BaseAI>(aiSys);
}

BaseAI* Entity::GetAI()
{
  return ai.get();
}

std::string Entity::GetDescription()
{
  return description;
}

void Entity::SetDescription(std::string description)
{
  this->description = description;
}

float Entity::GetMaxRotationalSpeed()
{
  return maxRotationalSpeed;
}

inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}

void Entity::SetDeltaRotation(float target)
{
  target = clamp(target, -maxRotationalSpeed, maxRotationalSpeed);
  targetRotation += target;
}

void Entity::SetMaxRotationalSpeed(float target)
{
  maxRotationalSpeed = target;
}


int Entity::GetMissileBays()
{
  int activeMissileBays = 0;
  for(int j=0;j<MAX_COMPONENT_SIDES; ++j) {
      for(int i=0;i<MAX_COMPONENT_LAYERS; ++i) {
	auto components = GetComponents((component_side_t)j, (component_layer_t)i);
	for(auto comp : components) {
	  if((comp.name == "Missile Bay") && (comp.health > 0)) {
	    activeMissileBays += 1;
	  }
	}
      }
    }
  return activeMissileBays;
}
