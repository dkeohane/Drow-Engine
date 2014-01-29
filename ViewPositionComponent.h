#ifndef VIEWPOSITIONCOMPONENT_H_
#define VIEWPOSITIONCOMPONENT_H_

class ViewPositionComponent: public PositionComponent
{
public:
	ViewPositionComponent(float posX, float posY)
		:PositionComponent(posX, posY)
	{

	}
	~ViewPositionComponent(){}
};
#endif