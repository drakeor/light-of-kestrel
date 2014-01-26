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

#ifndef BASEAI_H
#define BASEAI_H

class Entity;


/*
* Because AI routines are often fairly intensive, I've tried to make it as
* painless as possible while leveraging the fact that modern computers ACTUALLY have
* more than one core.
* 
* The AI is "installed" onto an entity and kept inside that entity. When the entity is deleted, the AI follows.
* 
* The PreProcessTurn is all done on the same thread in case we need to load anything important.
* 
* The ProcessTurn is now invoked on a seperate thread for each entity. Since technically we're only modifying future
* variables inside each entity and reading present variables, we shouldn't have any weird memory corruption or need to
* mutex the crap out of everything
*/

class Entity;

class BaseAI
{
protected:
  Entity* myEntity;
public:
  BaseAI();
  void SetEntity(Entity* ent);
  virtual void PreProcessTurn();
  virtual void ProcessTurn();
  virtual void Update(float dt);
  ~BaseAI();

};

#endif // BASEAI_H
