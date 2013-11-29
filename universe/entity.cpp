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
#include "../game.h"
#include "../log.h"
#include "../components/missilebay.h"

Entity::Entity(Game* game)
{
  this->game = game;
  health = 100;
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
}

void Entity::Update(float dt) {
  // Fix NaN issues. Temp fix.
  if(isnan(position.x) || isnan(position.y)) {
    position = sf::Vector2f(0, 0);
    FILE_LOG(logERROR) << "Entity has NaN position!!";
  }
  if(isnan(currentRotation)) {
    currentRotation = 0.1f;
    FILE_LOG(logERROR) << "Entity has NaN rotation!!";
  }
}

void Entity::Render() {
  float realRotation = currentRotation*57.3;
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
  
}

void Entity::Iterate(float dt) {
  // Update positions and rotations.
  velocityMagnitude += deltaThrust * dt;
  currentRotation += deltaRotation * dt;
  position = sf::Vector2f(position.x + (cos(currentRotation)*velocityMagnitude*dt),
			  position.y + (sin(currentRotation)*velocityMagnitude*dt));
}

sf::Vector2f Entity::GetCurrentPosition() {
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


void Entity::SetPosition(float x, float y) {
  position = sf::Vector2f(x, y);
}

void Entity::SetTargetRotation(float target) {
  targetRotation = target;
}

void Entity::SetTargetVelocity(float target) {
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



