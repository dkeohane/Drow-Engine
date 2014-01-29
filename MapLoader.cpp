#include "MapLoader.h"
#include "ViewPositionComponent.h"
#include "VelocityComponent.h"
#include "CharacterRPGAttributes.h"
#include "Ability.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"

void MapLoader::loadMap(const std::string filepath)
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

		backgroundData[tileID] = new Tile(tileID, *TextureManager::getInstance()->getResource(textureFilepath), collision, encounterChance, minLevel, maxLevel);
		encounterChance = minLevel = maxLevel = 0;
	}

	Json::Value mapData = root.get("Map1", NULL);
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
			((SpriteComponent*)e->getComponent<SpriteComponent>())->centreOnOrigin();
			if(backgroundData[mapRow[j].asInt()]->collision)
			{
				e->addComponent(new CollisionComponent());
				groupManager->set("Wall", *e);
			}
			e->addComponent(new PositionComponent((float)j * 70, (float)i * 70));
			e->refresh();
		}
	}

	Json::Value architecture = root.get("Architecture", NULL);
	for(unsigned int i = 0; i < architecture.size(); i++)
	{
		Json::Value building = architecture[i];
		e = &this->entityManager->create();
		e->addComponent(new SpriteComponent(*TextureManager::getInstance()->getResource(building["textureFilepath"].asString())));
		((SpriteComponent*)e->getComponent<SpriteComponent>())->centreOnOrigin();
		if(building["Scale"].asBool())
		{
			((SpriteComponent*)e->getComponent<SpriteComponent>())->getSprite()->setScale((float)building["Scale"]["x"].asDouble(), (float)building["Scale"]["x"].asDouble());
		}
		e->addComponent(new PositionComponent((float)building["Position"]["x"].asDouble(), (float)building["Position"]["y"].asDouble()));
		if(building["Collision"].asBool()){ e->addComponent(new CollisionComponent()); }
		groupManager->set("Architecture", *e);
		e->refresh();
	}
}

void MapLoader::loadPlayerDetails(const std::string filepath)
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

	e->refresh();
}


void MapLoader::loadAnimations(Json::Value &root, artemis::Entity* entity)
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
		}
	}
}

void MapLoader::loadAbilities(Json::Value &root, artemis::Entity* entity)
{
	Json::Value RPGAttributes = root.get("CharacterRPGAttributes", NULL);
	if(RPGAttributes != NULL)
	{
		CharacterRPGAttributes* characterAttributes = new CharacterRPGAttributes();
		characterAttributes->setMaxHealth(RPGAttributes["MaxHealth"].asDouble());
		characterAttributes->setCurrentHealth(RPGAttributes["CurrentHealth"].asDouble());
		characterAttributes->setStrength(RPGAttributes["Strength"].asDouble());
		characterAttributes->setAgility(RPGAttributes["Agility"].asDouble());
		characterAttributes->setIntelligence(RPGAttributes["Intelligence"].asDouble());

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

void MapLoader::loadTextComponents(const std::string filepath, bool viewRelativePositioning, std::vector<artemis::Entity*>* entityVector)
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

		e = &this->entityManager->create();
		e->addComponent(new PositionComponent(position.x, position.y));
		e->addComponent(new MenuTextComponent(displayText, *FontManager::getInstance()->getResource("Media/Fonts/font1.ttf"), selectedColour, deselectedColour, characterSize));
		e->addComponent(new PlayerInputComponent(0, 30));
		if(viewRelativePositioning)
		{
			e->addComponent(new ViewPositionComponent(position.x, position.y));
		}
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
