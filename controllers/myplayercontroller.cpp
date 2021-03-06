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
#include <dropdown.h>
#include <textlabel.h>
#include <entityfactory.h>
#include <components/missilebay.h>
#include <ai/shipai.h>

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
    
    // Return if the current turn is active
    if(universe->GetCurrentGalaxy()->IsTurnActive() && (playerController->GetPlayer() != nullptr))
      return;
    
    // Spawn the player only if he doesn't exist.
    playerController->SpawnPlayer();
    FILE_LOG(logDEBUG) << "MyPlayerController: Turn commited!";
    
    // First, calculate our target rotation. This is done by (fillPercentage-0.5)*2*maxTurnRate.
    float finalAngle = ((Slider*)playerController->GetControl()->GetControl("DirectionControl"))->GetFillPercentage();
    finalAngle = (finalAngle-0.5f) * 2 * 2;
    // Should we reset the controls afterwards?
    //((Slider*)playerController->GetControl()->GetControl("DirectionControl"))->SetFillPercentage(0.5f);
    
    // Second, calculate our target velocity.This is done by just fillPercentage*maxAcceleration.
    float finalVelocity = ((Slider*)playerController->GetControl()->GetControl("ThrustControl"))->GetFillPercentage();
    finalVelocity = finalVelocity * 200; //TODO: Set this to player's max velocity.
    playerController->GetPlayer()->SetDeltaRotation(finalAngle);
    playerController->GetPlayer()->SetTargetVelocity(finalVelocity);
    
    // Third, fire missiles if we have them.
    std::vector<std::string> selectedMissiles = playerController->GetSelectedMissiles();
    std::vector<missile_t> missilePositions;
    for(unsigned int i=0; i < selectedMissiles.size(); i++){
      // We can actually grab the missile this way.
      int tx, ty, ms;
      std::vector< missile_t > tempMissiles = playerController->GetPlayer()->GetMissiles();
      std::stringstream ss(selectedMissiles[i]);
      ss >> tx >> ty;
      ms = (ty*GameSettings::g_missilePerRow)+tx;
      missile_t tMissile = tempMissiles[ms];
      FILE_LOG(logWARNING) << "Firing missile " << ms;
      missilePositions.push_back(tMissile);
    }
    
    // We need to remove the spent missiles now that we fired them.
    for(unsigned int i=0; i < missilePositions.size(); i++){
      playerController->GetPlayer()->FireMissile(missilePositions[i]);
    }
    
    // Finally, reset the interfaces and commit the turn
    playerController->ResetMissileInterface();
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

class MissileButtonListener : public EventListener
{
 MyPlayerController* playerController;
public:
  MissileButtonListener(MyPlayerController* playerController) { 
    this->playerController = playerController;
  }
  virtual ~MissileButtonListener() {}
  void OnEvent(EventHandler* handler) {
    // Alright, we need to inform of the missile we got. Do some missile logic here now.
    playerController->SelectGUIMissile(handler->GetEventName());
    FILE_LOG(logWARNING) << "Missile button commited at position at " << handler->GetEventName();
  }  
};

/*
 * Constructor for this controller.
 */
MyPlayerController::MyPlayerController(Game* game) :
  BaseController(game)
{
  // Initialise some variables
  //hasPlayer = false;
  myPlayer = nullptr;
  
  // Add in our event listeners
  commitListener = std::unique_ptr<EventListener>( new CommitListener(this, game->GetUniverseManager()) );
  galaxyChangeListener = std::unique_ptr<EventListener>( new GalaxyChangeListener(this, game->GetUniverseManager()) );
  missileButtonListener  = std::unique_ptr<EventListener>( new MissileButtonListener(this) ); 
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
  commitButton->SetText("Commit");
  playerControls->AddControl("CommitButton", commitButton);
  
  // Add our missile controls and their corresponding elements
  missileControls = new BaseControl(game);
  missileControls->SetPosition(5, game->GetWindow()->getSize().y-145);
  for(int i=0; i < GameSettings::g_missilePerRow; i++)
  {
    for(int j=0; j < GameSettings::g_maxMissileColumns; j++)
    {
      std::stringstream ss;
      ss << i << ' ' << j;
      Button* missileButton = new Button(game);
      missileButton->SetPosition((i*20), (j*20));
      missileButton->SetImage("gfx/interface/empty.png");
      missileButton->SetUserData(ss.str());
      missileButton->OnClick.AddListener(missileButtonListener.get());
      missileButton->OnClick.SetEventName(ss.str());
      missileButton->SetSize(16,16);
      missileControls->AddControl(ss.str(), missileButton);
    }
  }
  TextLabel* missileAbout = new TextLabel(game);
  missileAbout->GetText()->setScale(0.75f, 0.75f);
  missileAbout->SetPosition(235, 10);
  missileAbout->SetText("Active Missile Bays: 2\nTotal Missile Bays: 2");
  missileControls->AddControl("MissileBays",missileAbout);
  activeMissileBays = 2;
  
  
  // (TEST) Add in an astroid, give it some velocity. I'll use this to test collisions later!
    // TODO: There are issues making entities in general on this area like Setting positions, rotations.
  srand(NULL);
    for(int i=0;i<2;i++) {
      Entity* rawr = EntityFactory::BuildEntity(game, TURRET);
      rawr->SetPosition((rand() % 1200)-600, (rand() % 1000)-500);
      //rawr->SetTargetVelocity(20);
     // rawr->SetTargetRotation(0.78f);
      game->GetUniverseManager()->GetCurrentGalaxy()->AddEntity(rawr);
    }
    /*
    for(int i=0;i<4;i++) {
      Entity* rawr = EntityFactory::BuildEntity(game, SS_HORNET);
      rawr->SetFaction(RelationshipManager::PLAYER_FACTION);
      rawr->InstallAI(new ShipAI());
      rawr->SetPosition((rand() % 1200)-600, (rand() % 1000)-500);
      //rawr->SetTargetVelocity(20);
     // rawr->SetTargetRotation(0.78f);
      game->GetUniverseManager()->GetCurrentGalaxy()->AddEntity(rawr);
    }
    */
    for(int i=0;i<4;i++) {
      Entity* rawr = EntityFactory::BuildEntity(game, ASTROID);
      rawr->SetPosition((rand() % 800)-400, (rand() % 600)-300);
      rawr->SetTargetVelocity(5);
      rawr->SetTargetRotation(0.78f);
      game->GetUniverseManager()->GetCurrentGalaxy()->AddEntity(rawr);
    }
    
  ResetGui();
}

/*
 *  When we need to spawn the player into the universe, we'll call this function.
 */
void MyPlayerController::SpawnPlayer()
{
  if(!this->hasPlayer()) {
    
    Slider* thrustSlider = (Slider*)playerControls->GetControl("ThrustControl");
    thrustSlider->SetFillPercentage(0.0f);
    Slider* directionSlider = (Slider*)playerControls->GetControl("DirectionControl");
    directionSlider->SetFillPercentage(0.5f);
    
    // Add in our player immediately when this controller is constructed.
    //this->hasPlayer = true;
    this->myPlayer = EntityFactory::BuildEntity(game, DAELALUS);
    myPlayer->SetPosition((rand() % 200)-100, (rand() % 200)-100);
    game->GetUniverseManager()->GetCurrentGalaxy()->AddEntity(myPlayer);
    
    // Add in our ghost
    this->ghostObject = nullptr;

  }
}

bool MyPlayerController::hasPlayer()
{
  if(myPlayer != nullptr) return true;
  else return false;
}

/*
 * This is called by the commit button listener
 * This will update the missiles on the ship!
 */
void MyPlayerController::ResetMissileInterface()
{
  if(hasPlayer()) {
    
    // Reassemble the list of missiles
    std::vector< missile_t > tempMissiles = this->myPlayer->GetMissiles();
    auto sMissile = tempMissiles.begin();
    for(int y = 0; y < GameSettings::g_maxMissileColumns; y++) {
      for(int x = 0; x < GameSettings::g_missilePerRow; x++) {
	std::stringstream ss;
	ss << x << ' ' << y;
	if(missileControls->GetControl(ss.str()) != nullptr) {
	  if(sMissile != tempMissiles.end()) {
	    Missile tMiss = MissileFactory::GetMissile((*sMissile));
	    ((Button*)missileControls->GetControl(ss.str()))->SetImage(tMiss.missileImage);
	    sMissile++;
	  } else {
	      ((Button*)missileControls->GetControl(ss.str()))->SetImage("gfx/interface/empty.png");
	  }
	}
      }
    }
    selectedMissiles.clear();
  }
}

/*
 * Reload the GUI
 */
void MyPlayerController::ResetGui()
{
  UnsetGui();
  game->GetGuiManager()->GetRootNode()->AddControl("PlayerControl", playerControls);
  game->GetGuiManager()->GetRootNode()->AddControl("MissileControl", missileControls);
  playerControls->SetPosition(game->GetWindow()->getDefaultView().getCenter().x-50, game->GetWindow()->getDefaultView().getCenter().y+50);
  missileControls->SetPosition(5, game->GetWindow()->getSize().y-145);
}

void MyPlayerController::UnsetGui()
{
  game->GetGuiManager()->GetRootNode()->RemoveControl("PlayerControl");
}

/*
 * The event listener will call this one
 */
void MyPlayerController::SelectGUIMissile(std::string guiElement)
{
  // First we're going to check if this exists in our list. If it does, remove it.
  bool isRemoved = false;
  for(unsigned int i=0; i < selectedMissiles.size(); i++){
    if(guiElement == selectedMissiles[i]) {
      selectedMissiles.erase(selectedMissiles.begin()+i);
      isRemoved = true;
      break;
    }
  }
  
  if(hasPlayer()) {
    activeMissileBays = -myPlayer->missilesFired;
    for(int j=0;j<MAX_COMPONENT_SIDES; ++j) {
      for(int i=0;i<MAX_COMPONENT_LAYERS; ++i) {
	auto components = myPlayer->GetComponents((component_side_t)j, (component_layer_t)i);
	for(auto comp : components) {
	  if((comp.name == "Missile Bay") && (comp.health > 0)) {
	    activeMissileBays += 1;
	  }
	}
      }
    }
  }
  // If we didn't need to remove anything, we can probably add it assuming we didn't hit the max on missiles or it's invalid.
  if(!isRemoved) {
    if(selectedMissiles.size() < activeMissileBays) {
      selectedMissiles.push_back(guiElement);
    }
  }
}

void MyPlayerController::Render()
{
  
  if(this->hasPlayer()) {
    
    // Draw our ghost player in the world
    game->GetCamera()->PreRender();
    if(ghostObject.get() != nullptr) ghostObject.get()->Render();
    game->GetCamera()->PostRender();
    
    // Bind the camera to the player
    sf::Vector2f camPosition = myPlayer->GetCurrentPosition();
    camPosition.x -= game->GetWindow()->getDefaultView().getCenter().x;
    camPosition.y -= game->GetWindow()->getDefaultView().getCenter().y;
    game->GetCamera()->SetPosition(camPosition);
    
    // Missile Interfaces (TODO: Avoid allocating sprites every frame. Eric would kill me. xD)
    sf::Texture* tempTex = game->GetAssetManager()->GetTexture("gfx/interface/missilecontainer.png");
    sf::Sprite iconSprite = sf::Sprite(*tempTex);
    iconSprite.setPosition(0, missileControls->GetPosition().y);
    game->GetWindow()->draw(iconSprite);
      
    missileAnchorPoint = sf::Vector2i(5, game->GetWindow()->getSize().y-150);
    for(unsigned int i=0; i < selectedMissiles.size(); i++) {
      int tx, ty;
      std::stringstream ss(selectedMissiles[i]);
      ss >> tx >> ty;
      sf::Texture* tempTex = game->GetAssetManager()->GetTexture("gfx/interface/selectionbox.png");
      sf::Sprite iconSprite = sf::Sprite(*tempTex);
      iconSprite.setPosition(missileControls->GetPosition().x + (tx*20)-1, 
			     missileControls->GetPosition().y + (ty*20)-1);
      game->GetWindow()->draw(iconSprite);
    }
  }  
  BaseController::Render();
}

void MyPlayerController::Update(float dt)
{ 
  if(this->hasPlayer()) {
    ghostMovementDelay += dt;
    
    // We don't want to over-iterate the ghost
    if(ghostMovementDelay < GameSettings::maxTime)
      if(ghostObject.get() != nullptr) ghostObject.get()->Iterate(dt);
    
    if(ghostMovementDelay > GameSettings::ghostMovementDelayMax) {
      
	  // Grab the gui values
	  // TODO: This is repetitive to the commit turn listener. Move this to a function later.
	  float finalAngle = ((Slider*)GetControl()->GetControl("DirectionControl"))->GetFillPercentage();
	  finalAngle = GetPlayer()->GetCurrentRotation() + (finalAngle-0.5f) * 2 * 2;
	  float finalVelocity = ((Slider*)GetControl()->GetControl("ThrustControl"))->GetFillPercentage();
	  finalVelocity = finalVelocity * 200;
	  ghostMovementDelay = 0;
	  
	  // Reset this stuff.
	  this->ghostObject = std::unique_ptr<Entity>(EntityFactory::BuildEntity(game, SPACESHIP));
	  sf::Color ghostColor = sf::Color::White;
	  ghostColor.a = 80;
	  this->ghostObject.get()->GetSprite()->setColor(ghostColor);
	  ghostObject.get()->SetPosition(myPlayer->GetCurrentPosition().x, myPlayer->GetCurrentPosition().y);
	  ghostObject.get()->SetCurrentVelocity(myPlayer->GetCurrentVelocity());
	  ghostObject.get()->SetCurrentRotation(myPlayer->GetCurrentRotation());
	  ghostObject.get()->SetTargetRotation(finalAngle);
	  ghostObject.get()->SetTargetVelocity(finalVelocity);
	  ghostObject.get()->CommitTurn(GameSettings::frameTime, GameSettings::maxTime);
    }
    
    if(game->GetUniverseManager()->GetCurrentGalaxy()->IsTurnActive()) {
      //Get our text label and update it
	TextLabel* tempLabel = (TextLabel*) missileControls->GetControl("MissileBays");
	int activeMissileBays = myPlayer->GetMissileBays();
	std::stringstream ss;
	ss << "Ready Missile Bays: " << (activeMissileBays-myPlayer->missilesFired) << 
	    "\nActive Missile Bays: " << activeMissileBays << 
	    "\n\nCore Health: " << myPlayer->GetHealth();
	tempLabel->SetText(ss.str());
    }
  }
  BaseController::Update(dt);
}

void MyPlayerController::ResetPlayer()
{
  myPlayer = nullptr;
}

Entity* MyPlayerController::GetPlayer()
{
  return myPlayer;
}

BaseControl* MyPlayerController::GetControl()
{
  return playerControls;
}

BaseControl* MyPlayerController::GetMissileControls()
{
  return missileControls;
}

std::vector< std::string > MyPlayerController::GetSelectedMissiles()
{
  return selectedMissiles;
}

