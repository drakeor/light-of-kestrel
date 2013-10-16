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
#include "../game.h"
#include "../log.h"
#include <slider.h>
#include <button.h>

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
    FILE_LOG(logWARNING) << "Turn button commited!!";
    float finalAngle = ((Slider*)playerController->GetControl()->GetControl("DirectionControl"))->GetFillPercentage();
    finalAngle = finalAngle * 6.28;
    float finalVelocity = ((Slider*)playerController->GetControl()->GetControl("ThrustControl"))->GetFillPercentage();
    finalVelocity = finalVelocity * 10;
    playerController->GetPlayer()->SetTargetRotation(finalAngle);
    playerController->GetPlayer()->SetTargetVelocity(finalVelocity);
    universe->GetCurrentGalaxy()->CommitTurn();
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

MyPlayerController::MyPlayerController(Game* game) :
  BaseController(game)
{
  this->hasPlayer = true;
  this->myPlayer = new Entity(game);
  myPlayer->SetPosition(100, 100);
  game->GetUniverseManager()->GetCurrentGalaxy()->AddEntity(myPlayer);
  
  commitListener = std::unique_ptr<EventListener>( new CommitListener(this, game->GetUniverseManager()) );
  galaxyChangeListener = std::unique_ptr<EventListener>( new GalaxyChangeListener(this, game->GetUniverseManager()) );
  game->GetUniverseManager()->OnGalaxyChange.AddListener(galaxyChangeListener.get());
  
  // Add the player control and corresponding gui elements
  playerControls = new BaseControl(game);
  playerControls->SetPosition(300, 500);
  
  Slider* thrustSlider = new Slider(game);
  thrustSlider->SetSize(100, 25);
  playerControls->AddControl("ThrustControl", thrustSlider);
  
  Slider* directionSlider = new Slider(game);
  directionSlider->SetSize(100, 25);
  directionSlider->SetPosition(0, 30);
  playerControls->AddControl("DirectionControl", directionSlider);
  
  Button* commitButton = new Button(game);
  commitButton->SetPosition(0, 60);
  commitButton->SetEnabled(false);
  commitButton->OnClick.AddListener(commitListener.get());
  playerControls->AddControl("CommitButton", commitButton);
  
  ResetGui();
}

void MyPlayerController::ResetGui()
{
  UnsetGui();
  game->GetGuiManager()->GetRootNode()->AddControl("PlayerControl", playerControls);
  playerControls->SetPosition(game->GetWindow()->getDefaultView().getCenter().x, game->GetWindow()->getDefaultView().getCenter().y);
}

void MyPlayerController::UnsetGui()
{
  game->GetGuiManager()->GetRootNode()->RemoveControl("PlayerControl");
}

void MyPlayerController::Render()
{
  // Bind the camera to the player
  if(this->hasPlayer) {
    sf::Vector2f camPosition = myPlayer->GetCurrentPosition();
    camPosition.x -= game->GetWindow()->getDefaultView().getCenter().x;
    camPosition.y -= game->GetWindow()->getDefaultView().getCenter().y;
    game->GetCamera()->SetPosition(camPosition);
  }
  BaseController::Render();
}

void MyPlayerController::Update(float dt)
{
  
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


