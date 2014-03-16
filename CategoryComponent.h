#ifndef CATEGORYCOMPONENT_H_
#define CATEGORYCOMPONENT_H_

#include "Artemis\Component.h"
#include "Category.h"

class CategoryComponent: public artemis::Component
{
public:
	CategoryComponent()
	{
		categories = Categories::None;
	}
	~CategoryComponent(){}

	Categories getCategories(){ return categories; }
	void setCategories(Categories val) { categories = val; }

private:
	Categories categories;
};
#endif