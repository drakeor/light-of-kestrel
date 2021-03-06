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

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <map>

/*
 * The asset manager is responsible for loading new textures/fonts and caching them
 */
class AssetManager
{
  
  // Containers that hold the resources for the game
  std::map<std::string, sf::Texture> texture;
  std::map<std::string, sf::Font> font;
public:
  
  AssetManager();
  
  /* 
   * GetTexture will return a texture if it's been loaded.
   * If the texture hasn't been loaded yet, the AssetManager will attempt to load it
   * and return it.
   */
  sf::Texture* GetTexture(std::string textureName);
  
  /* 
   * GetFont will return a texture if it's been loaded.
   * If the texture hasn't been loaded yet, the AssetManager will attempt to load it
   * and return it.
   */
  sf::Font* GetFont(std::string fontName);
  virtual ~AssetManager();
};

#endif // ASSETMANAGER_H
