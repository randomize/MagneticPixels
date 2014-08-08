#ifndef __SHAKE_H__
#define __SHAKE_H__

#include "ECBase.h"

class Shake : public cocos2d::ActionInterval
{

public:

   Shake();

   // Create the action with a time and a strength (same in x and y)
   static Shake* create(float d, float strength);
   // Create the action with a time and strengths (different in x and y)
   static Shake* createWithStrength(float d, float strength_x, float strength_y);

   ActionInterval* reverse() const override;
   ActionInterval* clone() const override;

protected:

   bool initWithDuration(float d, float strength_x, float strength_y);

   void startWithTarget(Node *pTarget);
   void update(float time);
   void stop(void);

   // Initial position of the shaked node
   float m_initial_x, m_initial_y;
   // Strength of the action
   float m_strength_x, m_strength_y;

};

#endif //__SHAKE_H__
