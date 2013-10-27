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

#include "myplayercontroller.h"
#include "settings.h"
#include "../game.h"
#include "../log.h"
#include <slider.h>
#include <button.h>
#include <entityfactory.h>

/*
 * This event listener is toggled whenever the player presses the commit button.
 * This listener will actually do all the work regarding committing the player's decisions
 * to the system
 */
class CommitListener : public EventListener
{
  UniverseManager* universe;
  MyPlayerController* playerController;
public:
  CommitListener(MyPlayerController* playerController, UniverseManager* universe) { 
    this->universe = universe;
    this->playerController = playerController;
  }
  virtual ~CommitListener() {}
  void OnEvent(EventHandler* handler) {
    
    // Spawn the player
    playerController->SpawnPlayer();
    FILE_LOG(logWARNING) << "Turn button commited!!";
    
    // First, calculate our target rotation. This is done by (fillPercentage-0.5)*2*maxTurnRate.
    float finalAngle = ((Slider*)playerController->GetControl()->GetControl("DirectionControl"))->GetFillPercentage();
    finalAngle = playerController->GetPlayer()->GetCurrentRotation() + (finalAngle-0.5f) * 2 * 2;
    // TEST: Should we reset this afterwards?
    //((Slider*)playerController->GetControl()->GetControl("DirectionControl"))->SetFillPercentage(0.5f);
    
    // Second, calculate our target velocity.This is done by just fillPercentage*maxAcceleration.
    float finalVelocity = ((Slider*)playerController->GetControl()->GetControl("ThrustControl"))->GetFillPercentage();
    finalVelocity = finalVelocity * 200;

    playerController->GetPlayer()->SetTargetRotation(finalAngle);
    playerController->GetPlayer()->SetTargetVelocity(finalVelocity);
    universe->GetCurrentGalaxy()->CommitTurn();
    //playerController->GetPlayer()->CommitTurn(Galaxy::frameTime, Galaxy::maxTime);
  }
};

/*
 * This listener will handler the destruction and recreation of the player whenever the current galaxy changes.
 * Other ships will have their AI protocols or whatever do this for them
 */
class GalaxyChangeListener : public EventListener
{
  UniverseManager* universe;
  MyPlayerController* playerController;
public:
  GalaxyChangeListener(MyPlayerController* playerController, UniverseManager* universe) { 
    this->universe = universe;
    this->playerController = playerController;
  }
  virtual ~GalaxyChangeListener() {}
  void OnEvent(EventHandler* handler) {
      // TODO: Recreate player.
  }
};

/*
 * Constructor for this controller.
 */

MyPlayerController::MyPlayerController(Game* game) :
  BaseController(game)
{
  
  // Add an event listener for when the player commits a turn and the galaxy changes.
  commitListener = std::unique_ptr<EventListener>( new CommitListener(this, game->GetUniverseManager()) );
  galaxyChangeListener = std::unique_ptr<EventListener>( new GalaxyChangeListener(this, game->GetUniverseManager()) );
  game->GetUniverseManager()->OnGalaxyChange.AddListener(galaxyChangeListener.get());
  
  // Add the player control and corresponding gui elements
  playerControls = new BaseControl(game);
  playerControls->SetPosition(300, 500);
  
  // Add our GUI controls
  Slider* thrustSlider = new Slider(game);
  thrustSlider->SetSize(25, 100);
  thrustSlider->SetPosition(-30, -100);
  thrustSlider->SetHorizontal(false);
  thrustSlider->ShowHalfMark(true);
  playerControls->AddControl("ThrustControl", thrustSlider);
  
  Slider* directionSlider = new Slider(game);
  directionSlider->SetSize(100, 25);
  directionSlider->SetPosition(0, 0);
  directionSlider->ShowHalfMark(true);
  playerControls->AddControl("DirectionControl", directionSlider);
  
  Button* commitButton = new Button(game);
  commitButton->SetPosition(0, 60);
  commitButton->SetEnabled(false);
  commitButton->OnClick.AddListener(commitListener.get());
  playerControls->AddControl("CommitButton", commitButton);
  
  ResetGui();
}

void MyPlayerController::SpawnPlayer()
{
  if(!this->hasPlayer) {
    
    Slider* thrustSlider = (Slider*)playerControls->GetControl("ThrustControl");
    thrustSlider->SetFillPercentage(0.0f);
    Slider* directionSlider = (Slider*)playerControls->GetControl("DirectionControl");
    directionSlider->SetFillPercentage(0.5f);
    
    // Add in our player immediately when this controller is constructed.
    this->hasPlayer = true;
    this->myPlayer = EntityFactory::BuildEntity(game, SPACESHIP);
    myPlayer->SetPosition(100, 100);
    game->GetUniverseManager()->GetCurrentGalaxy()->AddEntity(myPlayer);
    
    // (TEST) Add in an astroid, give it some velocity. I'll use this to test collisions later!
    for(int i=0;i<1;i++) {
      Entity* rawr = EntityFactory::BuildEntity(game, ASTROID);
      rawr->SetPosition((rand() % 1000 - 500), (rand() % 1000 - 500));
      rawr->SetTargetVelocity(((float)rand()/(float)RAND_MAX)*60);
      rawr->SetTargetRotation(((float)rand()/(float)RAND_MAX)*6.28);
      game->GetUniverseManager()->GetCurrentGalaxy()->AddEntity(rawr);
    }
  }
}


void MyPlayerController::ResetGui()
{
  UnsetGui();
  game->GetGuiManager()->GetRootNode()->AddControl("PlayerControl", playerControls);
  playerControls->SetPosition(game->GetWindow()->getDefaultView().getCenter().x-50, game->GetWindow()->getDefaultView().getCenter().y+50);
}

void MyPlayerController::UnsetGui()
{
  game->GetGuiManager()->GetRootNode()->RemoveControl("PlayerControl");
}

void MyPlayerController::Render()
{
  
  if(this->hasPlayer) {
    
    game->GetCamera()->PreRender();
    game->GetWindow()->draw(ghostEntity);
    game->GetCamera()->PostRender();
    // Bind the camera to the player
    sf::Vector2f camPosition = myPlayer->GetCurrentPosition();
    camPosition.x -= game->GetWindow()->getDefaultView().getCenter().x;
    camPosition.y -= game->GetWindow()->getDefaultView().getCenter().y;
    game->GetCamera()->SetPosition(camPosition);
    
  }  
  BaseController::Render();
}

void MyPlayerController::Update(float dt)
{
  if(this->hasPlayer) {
    ghostMovementDelay += dt;
    ghostRotation += ghostDeltaRotation * dt;
    ghostVelocity += ghostDeltaVelocity * dt;
    ghostEntity.rotate(ghostRotation);
    sf::Vector2f position = sf::Vector2f((cos(ghostRotation)*ghostVelocity*dt),
			  (sin(ghostRotation)*ghostVelocity*dt));
    ghostEntity.move(position.x, position.y);
    
    if(ghostMovementDelay > GameSettings::ghostMovementDelayMax) {
      
	  ghostEntity = (*myPlayer->GetSprite());
	  ghostEntity.setPosition(myPlayer->GetCurrentPosition());
	  ghostVelocity = myPlayer->GetCurrentVelocity();
	  ghostRotation = myPlayer->GetCurrentRotation();
	  
	  float finalAngle = ((Slider*)GetControl()->GetControl("DirectionControl"))->GetFillPercentage();
	  finalAngle = GetPlayer()->GetCurrentRotation() + (finalAngle-0.5f) * 2 * 2;
	  float finalVelocity = ((Slider*)GetControl()->GetControl("ThrustControl"))->GetFillPercentage();
	  finalVelocity = finalVelocity * 200;
	  ghostMovementDelay = 0;
	  ghostDeltaRotation = (finalAngle - this->myPlayer->GetCurrentRotation()) / Galaxy::maxTime;
	  ghostDeltaVelocity = (finalVelocity - this->myPlayer->GetCurrentVelocity()) / Galaxy::maxTime;
    }
  }
  BaseController::Update(dt);
}

void MyPlayerController::ResetPlayer()
{

}

Entity* MyPlayerController::GetPlayer()
{
  return myPlayer;
}

BaseControl* MyPlayerController::GetControl()
{
  return playerControls;
}


