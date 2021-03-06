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

#include "assetmanager.h"
#include "log.h"

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}

sf::Texture* AssetManager::GetTexture(std::string textureName)
{
  // Check if the texture exists
  if(texture.find(textureName) == texture.end()) {
    texture[textureName] = sf::Texture();
    if(!(texture[textureName].loadFromFile(textureName)))
    {
      FILE_LOG(logERROR) << "Failed to load texture: " << textureName;
    }
    FILE_LOG(logINFO) << "Loaded texture: " << textureName;
  }
  
  // Texture exists, return the proper texture
  return &texture[textureName];
}

sf::Font* AssetManager::GetFont(std::string fontName)
{
  // Return the font if it exists
  if(font.find(fontName) == font.end()) {
    font[fontName] = sf::Font();
    font[fontName].loadFromFile(fontName);
    FILE_LOG(logINFO) << "Loaded font: " << fontName;
  }
  
  return &font[fontName];
}


 