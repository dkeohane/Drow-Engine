#ifndef FILELOADER_H_
#define FILELOADER_H_

#include "Global.h"

//for file reading
#include "json\json.h"
#include <fstream>
//end file reading

#include "Tile.h"
#include "TextureManager.h"
#include "FontManager.h"

#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "PositionComponent.h"
#include "MenuTextComponent.h"
#include "PlayerInputComponent.h"
#include "ViewPositionComponent.h"
#include "VelocityComponent.h"
#include "CharacterRPGComponent.h"
#include "Ability.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"
#include "InventoryComponent.h"
#include "BattleEncounterComponent.h"

#include "EquipableItemSlots.h"
#include "CharacterAttributes.h"

#include "Animation.h"

class FileLoader
{
public:
	FileLoader(artemis::World* world)
	{
		this->world = world;
		entityManager = world->getEntityManager();
		groupManager = world->getGroupManager();
		tagManager = world->getTagManager();
		spriteMapper.init(*world);
		positionMapper.init(*world);
	}
	~FileLoader(){}

	void loadMap(const std::string filepath);
	void loadStateSprites(const std::string filepath);
	void loadAnimations(Json::Value &root, artemis::Entity* entity);
	void loadTextComponents(const std::string filepath, bool viewRelativePositioning, std::vector<artemis::Entity*>* entityVector);
	void loadPlayerDetails(const std::string filepath);
	void loadAbilities(Json::Value &root, artemis::Entity* entity);
	void loadPlayerItems(Json::Value &root, artemis::Entity* entity);

private:
	artemis::Entity* e;
	artemis::World* world;

	artemis::EntityManager* entityManager;
	artemis::GroupManager* groupManager;
	artemis::TagManager* tagManager;

	std::map<int, Tile*> backgroundData;
	std::map<string, EquipableItemSlots::ItemSlots> equipableSlots;

	artemis::ComponentMapper<SpriteComponent> spriteMapper;
	artemis::ComponentMapper<PositionComponent> positionMapper;
};

#endif