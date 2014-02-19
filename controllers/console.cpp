/*
 *
 * Copyright (c) 2013 River Bartz <drakeor.dragon@gmail.com>
 * 
 *     Permission is hereby granted, free of charge, to any person
 *     obtaining a copy of this software and associated documentation
 *     files (the "Software"), to deal in the Software without
 *     restriction, including without limitation the rights to use,
 *     copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the
 *     Software is furnished to do so, subject to the following
 *     conditions:
 * 
 *     The above copyright notice and this permission notice shall be
 *     included in all copies or substantial portions of the Software.
 * 
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *     OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *     NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *     HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *     WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *     FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *     OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

#include "../controllers/console.h"
#include <queue>
#include <game.h>
#include <sstream>

Console::Console(Game* game) :
  BaseController(game)
{
  consoleText = new TextLabel(game);
  consoleText->GetText()->setScale(0.75f, 0.75f);
  consoleText->SetPosition(10, 10);
  consoleText->GetText()->setColor(sf::Color::White);
  consoleText->SetText("Active Missile Bays: 2\nTotal Missile Bays: 2");
  game->GetGuiManager()->GetRootNode()->AddControl("MissileBays", consoleText);
}

Console::~Console()
{

}

void Console::Update(float dt)
{

}

void Console::Render()
{
  std::stringstream ss;
  for(auto it=consoleLines.begin(); it!=consoleLines.end();++it)
        ss << *it << "\n";
  this->consoleText->SetText(ss.str());
}

void Console::AddLine(std::string consoleLine)
{
  consoleLines.push_back(consoleLine);
  if(consoleLines.size() > MAX_CONSOLE_LINES)
    consoleLines.pop_front();
}

