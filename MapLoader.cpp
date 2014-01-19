#include "MapLoader.h"

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
		std::cerr << "Unable to load entity: " << filepath << std::endl;
		std::cerr << reader.getFormatedErrorMessages() << std::endl;
		return;
	}

	Json::Value numberMapping = root.get("NumberMap", NULL);
	if (numberMapping == NULL)
	{
		cout << "Number Values is NULL" << endl;
	}

	cout << numberMapping << endl;
	for(unsigned int i = 0; i < numberMapping.size(); i++)
	{
		Json::Value numberData = numberMapping[i];
		int num = numberData["name"].asInt();
		string textureFilepath = numberData["textureFilepath"].asString();
		bool collision = numberData["Collision"].asBool();
		backgroundData[num] = new Tile(num, *TextureManager::getInstance()->getResource(textureFilepath), collision);
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
}