#include "FileLoader.h"

void FileLoader::loadMap(const std::string filepath)
{
	//open up the file
	std::filebuf fb;
	fb.open(filepath, std::ios::in);

	//try to parse it
	Json::Value root;
	Json::Reader reader;
	bool parseSuccessful = reader.parse(std::istream(&fb), root);
	//now we have it parsed we can close it
	fb.close();

	if(!parseSuccessful)
	{
		std::cerr << "Unsuccessful parse of : " << filepath << std::endl;
		std::cerr << reader.getFormatedErrorMessages() << std::endl;
		return;
	}

	Json::Value numberMapping = root.get("NumberMap", NULL);
	if (numberMapping == NULL)
	{
		cout << "Number Values is NULL" << endl;
	}

	int encounterChance, minLevel, maxLevel;
	encounterChance = minLevel = maxLevel = 0;
	for(unsigned int i = 0; i < numberMapping.size(); i++)
	{
		Json::Value numberData = numberMapping[i];
		int tileID = numberData["name"].asInt();
		string textureFilepath = numberData["textureFilepath"].asString();
		bool collision = numberData["Collision"].asBool();

		Json::Value randomEncounter = numberData["RandomEncounter"];
		if(randomEncounter.asBool())
		{
			encounterChance = randomEncounter["EncounterChance"].asInt();
			minLevel = randomEncounter["MinLevel"].asInt();
			maxLevel = randomEncounter["MaxLevel"].asInt();
		}
		else
		{
			encounterChance = 0;
			minLevel = 0;
			maxLevel = 0;
		}

		backgroundData[tileID] = new Tile(tileID, *TextureManager::getInstance()->getResource(textureFilepath), collision, encounterChance, minLevel, maxLevel);
		encounterChance = minLevel = maxLevel = 0;
	}

	Json::Value mapData = root.get("Map", NULL);
	if(mapData == NULL)
	{
		cout << "mapData is null" << endl;
	}

	for(unsigned int i = 0; i < mapData.size(); i++)
	{
		Json::Value mapRow = mapData[i];
		for (unsigned int j = 0; j < mapRow.size(); j++)
		{
			e = &this->entityManager->create();
			e->addComponent(new SpriteComponent(backgroundData[mapRow[j].asInt()]->texture));
			spriteMapper.get(*e)->centreOnOrigin();
			if(backgroundData[mapRow[j].asInt()]->collision)
			{
				e->addComponent(new CollisionComponent());
				groupManager->set("Wall", *e);
			}
			e->addComponent(new PositionComponent((float)j * 16, (float)i * 16));
			if(backgroundData[mapRow[j].asInt()]->encounterChance > 0)
			{
				int encounterChance = backgroundData[mapRow[j].asInt()]->encounterChance;
				int minLevel = backgroundData[mapRow[j].asInt()]->minLevel;
				int maxLevel = backgroundData[mapRow[j].asInt()]->maxLevel;
				sf::IntRect encRect((int)(positionMapper.get(*e))->getPosX(), (int)(positionMapper.get(*e))->getPosY(), (spriteMapper.get(*e))->getTexture()->getSize().x, (spriteMapper.get(*e))->getTexture()->getSize().y);
				/*sf::IntRect encRect(pos, );
				collisionBox->left = (int)(p.getPosX() - width / 2);
				collisionBox->top = (int)(p.getPosY() - height / 2);
				collisionBox->width = width;
				collisionBox->height = height;
				*/

				e->addComponent(new BattleEncounterComponent(encRect, encounterChance, minLevel, maxLevel));
			}
			e->refresh();
		}
	}

	artemis::Entity& tree = entityManager->create();
	tree.addComponent(new PositionComponent(0,0));
	tree.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/Trees/tree1.png")));
	spriteMapper.get(tree)->getSprite()->setScale(1.5f,1.5f);
	spriteMapper.get(tree)->getSprite()->setTextureRect(sf::IntRect(0,0,440,96));
	spriteMapper.get(tree)->getTexture()->setRepeated(true);
	tree.refresh();
	groupManager->set("Tree", tree);

	artemis::Entity& shrub = entityManager->create();
	shrub.addComponent(new PositionComponent(0,135));
	shrub.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/Trees/shrub2.png")));
	//spriteMapper.get(tree)->getSprite()->setScale(1.5f,1.5f);
	spriteMapper.get(shrub)->getSprite()->setTextureRect(sf::IntRect(0,0,261,28));
	spriteMapper.get(shrub)->getTexture()->setRepeated(true);
	shrub.refresh();
	groupManager->set("Tree", shrub);

	artemis::Entity& shrub2 = entityManager->create();
	shrub2.addComponent(new PositionComponent(360,135));
	shrub2.addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource("Media/Images/Trees/shrub2.png")));
	//spriteMapper.get(tree)->getSprite()->setScale(1.5f,1.5f);
	spriteMapper.get(shrub2)->getSprite()->setTextureRect(sf::IntRect(0,0,348,28));
	spriteMapper.get(shrub2)->getTexture()->setRepeated(true);
	shrub2.refresh();
	groupManager->set("Tree", shrub2);

	Json::Value architecture = root.get("Architecture", NULL);
	for(unsigned int i = 0; i < architecture.size(); i++)
	{
		Json::Value building = architecture[i];
		e = &this->entityManager->create();
		e->addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource(building["textureFilepath"].asString())));
		((SpriteComponent*)e->getComponent<SpriteComponent>())->centreOnOrigin();
		if(building["Scale"].asBool())
		{
			((SpriteComponent*)e->getComponent<SpriteComponent>())->getSprite()->setScale((float)building["Scale"]["x"].asDouble(), (float)building["Scale"]["y"].asDouble());
		}
		e->addComponent(new PositionComponent((float)building["Position"]["x"].asDouble(), (float)building["Position"]["y"].asDouble()));
		if(building["Collision"].asBool()){ e->addComponent(new CollisionComponent()); }
		groupManager->set("Architecture", *e);
		e->refresh();
	}
}

void FileLoader::loadStateSprites(const std::string filepath)
{
	//open up the file
	std::filebuf fb;
	fb.open(filepath, std::ios::in);

	//try to parse it
	Json::Value root;
	Json::Reader reader;
	bool parseSuccessful = reader.parse(std::istream(&fb), root);
	//now we have it parsed we can close it
	fb.close();

	if(!parseSuccessful)
	{
		std::cerr << "Unsuccessful parse of : " << filepath << std::endl;
		std::cerr << reader.getFormatedErrorMessages() << std::endl;
		return;
	}

	Json::Value Sprites = root.get("ScreenState", NULL);
	if(Sprites == NULL)
	{
		cout << "mapData is null" << endl;
	}

	for(int i = 0; i < Sprites.size(); i++)
	{
		Json::Value SpriteData = Sprites[i];

		e = &this->entityManager->create();
		e->addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource(SpriteData["textureFilepath"].asString())));
		
		if(SpriteData["Scale"].asBool())
		{
			spriteMapper.get(*e)->getSprite()->setScale((float)SpriteData["Scale"]["x"].asDouble(), (float)SpriteData["Scale"]["y"].asDouble());
		}

		Json::Value position = SpriteData.get("Position", -1);
		if(position != -1){ e->addComponent(new PositionComponent((float)position["x"].asDouble(), (float)position["y"].asDouble())); }
		
		if(SpriteData["Group"].asString() != ""){ groupManager->set(SpriteData["Group"].asString(), *e); }
		e->refresh();
	}
}

void FileLoader::loadPlayerDetails(const std::string filepath)
{
	//open up the file
	std::filebuf fb;
	fb.open(filepath, std::ios::in);

	//try to parse it
	Json::Value root;
	Json::Reader reader;
	bool parseSuccessful = reader.parse(std::istream(&fb), root);
	//now we have it parsed we can close it
	fb.close();

	if(!parseSuccessful)
	{
		std::cerr << "Unsuccessful parse of : " << filepath << std::endl;
		std::cerr << reader.getFormatedErrorMessages() << std::endl;
		return;
	}

	e = &this->entityManager->create();

	Json::Value tag = root.get("Tag", NULL);
	if(tag.asBool()){ tagManager->subscribe(tag.asString(), *e); }

	Json::Value group = root.get("Group", NULL);
	if(group.asBool()){ groupManager->set(group.asString(), *e); }

	Json::Value speed = root.get("Speed", -1);
	if(speed != -1){ e->addComponent(new VelocityComponent((float)speed["x"].asDouble(), (float)speed["y"].asDouble())); }

	Json::Value position = root.get("Position", -1);
	if(position != -1){ e->addComponent(new PositionComponent((float)position["x"].asDouble(), (float)position["y"].asDouble())); }
	
	Json::Value collision = root.get("Collision", NULL);
	if(collision.asBool()){ e->addComponent(new CollisionComponent()); }

	e->addComponent(new PlayerInputComponent(0, 30.0f));

	loadAbilities(root, e);
	loadAnimations(root, e);
	loadPlayerItems(root, e);

	e->refresh();
}


void FileLoader::loadAnimations(Json::Value &root, artemis::Entity* entity)
{
	std::map<string, sf::Texture*> textures;
	std::map<std::string, Animation*> *animationListPtr = new std::map<std::string, Animation*>();
	
	std::vector<string> stateStrings;
	bool movement = root["Movement"].asBool();

	Json::Value animations = root.get("Animations", NULL);
	if(animations != NULL)
	{
		for(unsigned int i = 0; i < animations.size(); i++)
		{
			Json::Value animation = animations[i];

			string name = animation["name"].asString();
			sf::Vector2i origin(animation["origin"]["x"].asInt(), animation["origin"]["y"].asInt());
			unsigned int frameCount = animation["frameCount"].asUInt();
			sf::Vector2i frameSize(animation["frameSize"]["width"].asInt(), animation["frameSize"]["height"].asInt());
			sf::Time frameDuration = sf::milliseconds(animation["frameDuration"].asInt());

			Animation* a = new Animation();
			a->addRow(origin, frameCount, frameSize, frameDuration);

			(*animationListPtr)[name] = a;
			textures[name] = (TextureManager::getInstance()->getResource(name));
		
			if(movement)
				stateStrings.push_back(name);
		}
		e->addComponent(new AnimationComponent(animationListPtr, textures));
		if (movement)
		{
			e->addComponent(new MovementComponent(stateStrings));
			((MovementComponent*)e->getComponent<MovementComponent>())->attach((AnimationComponent*)e->getComponent<AnimationComponent>());
			((MovementComponent*)e->getComponent<MovementComponent>())->notify();
		}
	}
}

void FileLoader::loadAbilities(Json::Value &root, artemis::Entity* entity)
{
	Json::Value RPGAttributes = root.get("CharacterRPGAttributes", NULL);
	if(RPGAttributes != NULL)
	{
		CharacterRPGComponent* characterAttributes = new CharacterRPGComponent();

		characterAttributes->setAttributeValue(CharacterAttributes::MAXHEALTH, RPGAttributes["MaxHealth"].asDouble());
		characterAttributes->setAttributeValue(CharacterAttributes::CURRENTHEALTH, RPGAttributes["CurrentHealth"].asDouble());
		characterAttributes->setAttributeValue(CharacterAttributes::AGILITY, RPGAttributes["Agility"].asDouble());
		characterAttributes->setAttributeValue(CharacterAttributes::STRENGTH, RPGAttributes["Strength"].asDouble());
		characterAttributes->setAttributeValue(CharacterAttributes::INTELLIGENCE, RPGAttributes["Intelligence"].asDouble());

		Json::Value abilities = root.get("AbilityData", NULL);
		if (abilities != NULL)
		{
			std::map<string, Ability> characterAbilities;
			for(unsigned int i = 0; i < abilities.size(); i++)
			{
				Json::Value ability = abilities[i];
				std::string name = ability["abilityName"].asString();
				Ability a(name, ability["damageValue"].asInt(), ability["healingValue"].asInt(), ability["abilityAnimationFilepath"].asString());
				characterAbilities[name] = a;
			}
			characterAttributes->setAbilities(characterAbilities);
		}
		entity->addComponent(characterAttributes);
	}
}

void FileLoader::loadTextComponents(const std::string filepath, bool viewRelativePositioning, std::vector<artemis::Entity*>* entityVector)
{
	//open up the file
	std::filebuf fb;
	fb.open(filepath, std::ios::in);

	//try to parse it
	Json::Value root;
	Json::Reader reader;
	bool parseSuccessful = reader.parse(std::istream(&fb), root);
	//now we have it parsed we can close it
	fb.close();

	if(!parseSuccessful)
	{
		std::cerr << "Unsuccessful parse of : " << filepath << std::endl;
		std::cerr << reader.getFormatedErrorMessages() << std::endl;
		return;
	}

	Json::Value MenuTextComponents = root.get("MenuTextComponents", NULL);
	if(MenuTextComponents == NULL)
	{
		cout << "MenuTextComponents is Null" << endl;
	}

	for(unsigned int i = 0; i < MenuTextComponents.size(); i++)
	{
		Json::Value MenuText = MenuTextComponents[i];

		string displayText = MenuText["DisplayText"].asString();
		sf::Vector2f position((float)MenuText["Position"]["x"].asDouble(),
							  (float)MenuText["Position"]["y"].asDouble());

		sf::Color selectedColour(MenuText["SelectedColour"]["R"].asUInt(),
								 MenuText["SelectedColour"]["G"].asUInt(),
								 MenuText["SelectedColour"]["B"].asUInt());

		sf::Color deselectedColour(MenuText["DeselectedColour"]["R"].asUInt(),
								   MenuText["DeselectedColour"]["G"].asUInt(),
								   MenuText["DeselectedColour"]["B"].asUInt());
		int characterSize = MenuText["CharacterSize"].asInt();
		string fontFilepath = MenuText["Font"].asString();

		e = &this->entityManager->create();
		e->addComponent(new PositionComponent(position.x, position.y));
		e->addComponent(new MenuTextComponent(displayText, *FontManager::getInstance()->getResource(fontFilepath), selectedColour, deselectedColour, characterSize));
		e->addComponent(new PlayerInputComponent(0, 30));
		if(viewRelativePositioning)
		{
			e->addComponent(new ViewPositionComponent(position.x, position.y));
		}
		this->entityManager->refresh(*e);
		e->refresh();

		if(MenuText["Group"].asBool())
		{
			groupManager->set(MenuText["Group"].asString(), *e);
		}
		if(entityVector)
		{
			entityVector->push_back(e);
		}
	}
}

void FileLoader::loadPlayerItems(Json::Value &root, artemis::Entity* entity)
{
	Json::Value inventory = root.get("Inventory", NULL);
	if(inventory != NULL)
	{
		InventoryComponent* inventory = new InventoryComponent();

		this->equipableSlots["Head"] = EquipableItemSlots::HEAD;
		this->equipableSlots["Chest"] = EquipableItemSlots::CHEST;
		this->equipableSlots["Shoulders"] = EquipableItemSlots::SHOUDLERS;
		this->equipableSlots["Gloves"] = EquipableItemSlots::GLOVES;
		this->equipableSlots["Leggings"] = EquipableItemSlots::LEGGINGS;
		this->equipableSlots["Boots"] = EquipableItemSlots::BOOTS;
		this->equipableSlots["Weapon"] = EquipableItemSlots::WEAPON;

		Json::Value equipableItems = root.get("EquipableItems", NULL);
		for(unsigned int i = 0; i < equipableItems.size(); i++)
		{
			Json::Value equipableItem = equipableItems[i];

			std::map<CharacterAttributes::Attribute, double> itemValues;
			Json::Value attributes = equipableItem["Attributes"];

			string name = equipableItem["name"].asString();
			string slotTypeString = equipableItem["slotType"].asString();
			string textureFilepath = equipableItem["textureFilepath"].asString();
			string itemDescription = equipableItem["itemDescription"].asString();
			EquipableItemSlots::ItemSlots slotType = equipableSlots[slotTypeString];

			itemValues[CharacterAttributes::MAXHEALTH] = attributes["MaxHealth"].asDouble();
			itemValues[CharacterAttributes::CURRENTHEALTH] = attributes["CurrentHealth"].asDouble();
			itemValues[CharacterAttributes::AGILITY] = attributes["Agility"].asDouble();
			itemValues[CharacterAttributes::STRENGTH] = attributes["Strength"].asDouble();
			itemValues[CharacterAttributes::INTELLIGENCE] = attributes["Intelligence"].asDouble();

			EquipableItem* item = new EquipableItem(slotType, itemValues, name);
			item->setItemDescription(itemDescription);
			item->setTextureFilePath(textureFilepath);
			item->attach((CharacterRPGComponent*)entity->getComponent<CharacterRPGComponent>());
			inventory->addItem(item);
		}
		entity->addComponent(inventory);
	}
}
