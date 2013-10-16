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

#include <SFML/Graphics.hpp>
#include "log.h"
#include "game.h"

int main()
{
    // Few initial variables
    Game* game = new(std::nothrow) Game();
    FILELog::ReportingLevel() = FILELog::FromString("DEBUG1");
    FILE* pFile = fopen("game.log", "a");
    Output2FILE::Stream() = pFile;
    if(game == nullptr) {
      FILE_LOG(logERROR) << "Could not create game object!";
      return 0;
    }
    
    // Create our SFML window and object.
    sf::RenderWindow window(sf::VideoMode(1024, 786), "Light Of Kestrel");
    window.setFramerateLimit(60);
    sf::View view;
    view.reset(sf::FloatRect(0, 0, 1024, 786));
    window.setView(view);
    
    // Initialise the game
    game->Initialise(&window, &view);
    
    // Update Loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                exit(1);
	    if (event.type == sf::Event::MouseButtonReleased)
	      game->GetGuiManager()->HandleMouseClick();
        }
        // We're locking the game to 60fps.
        // If it goes too slowly, then the simulation will move slowly.
        // This would be stupid to do in a realtime game.
	game->Update(0.016f);
        window.clear();
	sf::View view = window.getDefaultView();
	window.setView(view);
	game->Render();
	
        window.display();
    }

    // Destroy the game
    game->Destroy();
    delete game;
    
    return 0;
}