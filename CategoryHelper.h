#ifndef CATEGORYHELPER_H_
#define CATEGORYHELPER_H_

#include "Category.h"
#include "Artemis\Entity.h"

class CategoryHelper
{
public:
	static inline void appendFlag(artemis::Entity* entity, Categories subjectCategories)
	{
		if(entity->getComponent<CategoryComponent>())
		{
			cout << "Before : " << ((CategoryComponent*)entity->getComponent<CategoryComponent>())->getCategories() << endl;
			Categories c = ((CategoryComponent*)entity->getComponent<CategoryComponent>())->getCategories();
			((CategoryComponent*)entity->getComponent<CategoryComponent>())->setCategories(c | subjectCategories);
			cout << "After : " << ((CategoryComponent*)entity->getComponent<CategoryComponent>())->getCategories() << endl;

			c = ((CategoryComponent*)entity->getComponent<CategoryComponent>())->getCategories();
			subjectCategories = (~subjectCategories);

			((CategoryComponent*)entity->getComponent<CategoryComponent>())->setCategories(c | subjectCategories);
			cout << "After 2 : " << ((CategoryComponent*)entity->getComponent<CategoryComponent>())->getCategories() << endl;

		}
	}

	static inline void removeFlag(artemis::Entity* entity, Categories subjectCategories)
	{
		if(entity->getComponent<CategoryComponent>())
		{
			cout << "Before : " << ((CategoryComponent*)entity->getComponent<CategoryComponent>())->getCategories() << endl;
			Categories c = ((CategoryComponent*)entity->getComponent<CategoryComponent>())->getCategories();
			subjectCategories = (~subjectCategories);

			((CategoryComponent*)entity->getComponent<CategoryComponent>())->setCategories(c | subjectCategories);
			cout << "After : " << ((CategoryComponent*)entity->getComponent<CategoryComponent>())->getCategories() << endl;
		}
	}

	static inline bool hasAnyFlag(artemis::Entity* entity, Categories subjectCategories)
	{
		if(entity->getComponent<CategoryComponent>())
		{
			return (((CategoryComponent*)entity->getComponent<CategoryComponent>())->getCategories() & subjectCategories) != 0;
		}
	}

	static inline bool hasAllFlag(Categories entityCategories, Categories subjectCategories)
	{
		return (entityCategories & subjectCategories) == subjectCategories;
	}

};
#endif