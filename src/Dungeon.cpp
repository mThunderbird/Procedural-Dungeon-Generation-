#include "Dungeon.h"
#include "World.h"

Dungeon::Dungeon()
{
}

Dungeon::~Dungeon()
{
}

vector<SDL_FRect> Dungeon::generateCorridorv2(SDL_FRect a, SDL_FRect b)
{
	vector<SDL_FRect> res;

	queue<pair<SDL_FRect, SDL_FRect>> connectQ;

	connectQ.push(make_pair(a, b));

	while (!connectQ.empty())
	{
		auto pair = connectQ.front();
		connectQ.pop();

		if (collRectRectF_NOTPERFECT(pair.first, pair.second))
		{
			continue;
		}

		SDL_FRect newCor;

		bool hasColl = false;

		if (makeCorridor(pair.first, pair.second, newCor))
		{

			for (auto& room : m_rooms)
			{
				if (collRectRectF_NOTPERFECT(newCor, room.second.drawable.rect))
				{
					hasColl = true;

					room.second.type = ROOM_TYPE::DEFAULT;
					connectQ.push(make_pair(pair.first, room.second.drawable.rect));
					connectQ.push(make_pair(pair.second, room.second.drawable.rect));

					break;
				}
			}

			if (!hasColl)
			{
				for (auto& corridor : m_corridors)
				{
					if (collRectRectF_NOTPERFECT(newCor, corridor.second.drawable.rect))
					{
						hasColl = true;

						connectQ.push(make_pair(pair.first, corridor.second.drawable.rect));
						connectQ.push(make_pair(pair.second, corridor.second.drawable.rect));

						break;
					}
				}
			}

			if (!hasColl)
			{
				res.push_back(newCor);
			}

			continue;
		}

		float2 pivot;

		float2 center = m_rooms.at(mainRoomID).getCenter();

		SDL_FRect corridorTurn;

		bool isPossible = true;

		if (distance(float2{ centerRect(pair.first).x,centerRect(pair.second).y }, center)
			< distance(float2{ centerRect(pair.second).x,centerRect(pair.first).y }, center))
		{
			pivot = float2{ centerRect(pair.first).x,centerRect(pair.second).y };

			corridorTurn.x = pivot.x - CORRIDOR_WIDTH / 2;
			corridorTurn.y = pivot.y - CORRIDOR_WIDTH / 2;
			corridorTurn.w = CORRIDOR_WIDTH;
			corridorTurn.h = CORRIDOR_WIDTH;

			corridorTurn.x = roundToTile(float2{ corridorTurn.x,corridorTurn.y }).x;
			corridorTurn.y = roundToTile(float2{ corridorTurn.x,corridorTurn.y }).y;

			if (collRectRectF_NOTPERFECT(pair.first, corridorTurn) || collRectRectF_NOTPERFECT(pair.second, corridorTurn))
			{
				pivot = float2{ centerRect(pair.second).x,centerRect(pair.first).y };

				corridorTurn.x = pivot.x - CORRIDOR_WIDTH / 2;
				corridorTurn.y = pivot.y - CORRIDOR_WIDTH / 2;
				corridorTurn.w = CORRIDOR_WIDTH;
				corridorTurn.h = CORRIDOR_WIDTH;

				corridorTurn.x = roundToTile(float2{ corridorTurn.x,corridorTurn.y }).x;
				corridorTurn.y = roundToTile(float2{ corridorTurn.x,corridorTurn.y }).y;

				if (collRectRectF_NOTPERFECT(pair.first, corridorTurn) || collRectRectF_NOTPERFECT(pair.second, corridorTurn))
				{
					isPossible = false;
				}
			}

		}
		else
		{
			pivot = float2{ centerRect(pair.second).x,centerRect(pair.first).y };

			corridorTurn.x = pivot.x - CORRIDOR_WIDTH / 2;
			corridorTurn.y = pivot.y - CORRIDOR_WIDTH / 2;
			corridorTurn.w = CORRIDOR_WIDTH;
			corridorTurn.h = CORRIDOR_WIDTH;

			corridorTurn.x = roundToTile(float2{ corridorTurn.x,corridorTurn.y }).x;
			corridorTurn.y = roundToTile(float2{ corridorTurn.x,corridorTurn.y }).y;

			if (collRectRectF_NOTPERFECT(pair.first, corridorTurn) || collRectRectF_NOTPERFECT(pair.second, corridorTurn))
			{
				pivot = float2{ centerRect(pair.first).x,centerRect(pair.second).y };

				corridorTurn.x = pivot.x - CORRIDOR_WIDTH / 2;
				corridorTurn.y = pivot.y - CORRIDOR_WIDTH / 2;
				corridorTurn.w = CORRIDOR_WIDTH;
				corridorTurn.h = CORRIDOR_WIDTH;

				corridorTurn.x = roundToTile(float2{ corridorTurn.x,corridorTurn.y }).x;
				corridorTurn.y = roundToTile(float2{ corridorTurn.x,corridorTurn.y }).y;

				if (collRectRectF_NOTPERFECT(pair.first, corridorTurn) || collRectRectF_NOTPERFECT(pair.second, corridorTurn))
				{
					isPossible = false;
				}
			}

		}
		if (!isPossible)
		{
			continue;
		}

		hasColl = false;

		for (auto& room : m_rooms)
		{
			if (collRectRectF_NOTPERFECT(corridorTurn, room.second.drawable.rect))
			{
				hasColl = true;

				room.second.type = ROOM_TYPE::DEFAULT;
				connectQ.push(make_pair(pair.first, room.second.drawable.rect));
				connectQ.push(make_pair(pair.second, room.second.drawable.rect));

				break;
			}
		}

		if (!hasColl)
		{
			for (auto& corridor : m_corridors)
			{
				if (collRectRectF_NOTPERFECT(corridorTurn, corridor.second.drawable.rect))
				{
					hasColl = true;

					connectQ.push(make_pair(pair.first, corridor.second.drawable.rect));
					connectQ.push(make_pair(pair.second, corridor.second.drawable.rect));

					break;
				}
			}
		}

		if (!hasColl)
		{
			res.push_back(corridorTurn);
			connectQ.push(make_pair(pair.first, corridorTurn));
			connectQ.push(make_pair(pair.second, corridorTurn));
		}
	}
	return res;
}

vector<SDL_FRect> Dungeon::generateCorridorv3(int id1, int id2)
{
	queue<pair<int, int>> connectQ;

	vector<SDL_FRect> res;

	connectQ.push(make_pair(id1,id2));

	while (!connectQ.empty())
	{
		auto pair = connectQ.front();
		connectQ.pop();

		line collLine;
		collLine.start = centerRect(m_rooms.at(pair.first).drawable.rect);
		collLine.finish = centerRect(m_rooms.at(pair.second).drawable.rect);


		bool hasColl = false;

		for (auto& room : m_rooms)
		{
			if (room.first != pair.first && room.first != pair.second)
			{
				if (collLineAndSDL_FRect(collLine, room.second.drawable.rect))
				{
					connectQ.push(make_pair(pair.first, room.first));
					connectQ.push(make_pair(pair.second, room.first));

					hasColl = true;
					break;
				}
			}
		}

		if (hasColl)
		{
			continue;
		}

		SDL_FRect newCor;
		
		if (makeCorridor(m_rooms.at(pair.first).drawable.rect, m_rooms.at(pair.second).drawable.rect, newCor))
		{
			res.push_back(newCor);
		}
		else
		{
			cout << "we make a turn";
		}


	}

	
	return res;
}

SDL_FRect Dungeon::makeHorizontalCorridor(SDL_FRect a, SDL_FRect b)
{
	if (collRectRectF(a, b))
	{
		return SDL_FRect();
	}

	SDL_FRect res;

	if (a.x > b.x)
	{
		swap(a, b);
	}

	res.x = a.x + a.w;
	res.w = b.x - a.x - a.w;

	res.y = (a.y + a.y + a.h + b.y + b.y + b.h) / 4 - 20 / 2;
	res.h = 20;

	return res;
}

SDL_FRect Dungeon::makeVerticalCorridor(SDL_FRect a, SDL_FRect b)
{
	if (collRectRectF(a, b))
	{
		return SDL_FRect();
	}

	SDL_FRect res;

	if (a.y > b.y)
	{
		swap(a, b);
	}

	res.y = a.y + a.h;
	res.h = b.y - a.y - a.h;

	if (a.w < b.w)
	{
		res.x = centerRect(a).x - 20 / 2;
		res.w = 20; 
	}
	else
	{
		res.x = centerRect(b).x - 20 / 2;
		res.w = 20;
	}

	return res;
}

bool Dungeon::makeCorridor(SDL_FRect a, SDL_FRect b, SDL_FRect& corridor)
{

	if (!(a.x + a.w < b.x || a.x > b.x + b.w))
	{
		
		if (a.y > b.y)
		{
			swap(a, b);
		}

		corridor.y = a.y + a.h;
		corridor.h = b.y - a.y - a.h;

		corridor.x = (min(a.x + a.w, b.x + b.w) + max(a.x, b.x)) / 2 - 20 / 2;
		corridor.x = roundToTile(float2{ corridor.x,0.0f }).x;
		corridor.w = 20;

		return true;
	}

	if (!(a.y + a.h < b.y || a.y > b.y + b.h))
	{

		if (a.x > b.x)
		{
			swap(a, b);
		}

		corridor.x = a.x + a.w;
		corridor.w = b.x - a.x - a.w;

		corridor.y = (min(a.y + a.h, b.y + b.h) + max(a.y,b.y)) / 2 - 20 / 2;
		corridor.y = roundToTile(float2{ 0.0f,corridor.y }).y;
		corridor.h = 20;

		return true;
	}

	return false;
}

vector<SDL_FRect> Dungeon::makeCorridorv2(SDL_FRect a, SDL_FRect b)
{
	SDL_FRect corridor1;

	if (!(a.x + a.w < b.x || a.x > b.x + b.w))
	{

		if (a.y > b.y)
		{
			swap(a, b);
		}

		corridor1.y = a.y + a.h;
		corridor1.h = b.y - a.y - a.h;

		corridor1.x = (min(a.x + a.w, b.x + b.w) + max(a.x, b.x)) / 2 - 20 / 2;
		corridor1.x = roundToTile(float2{ corridor1.x,0.0f }).x;
		corridor1.w = 20;

		return vector<SDL_FRect>{corridor1};
	}

	if (!(a.y + a.h < b.y || a.y > b.y + b.h))
	{

		if (a.x > b.x)
		{
			swap(a, b);
		}

		corridor1.x = a.x + a.w;
		corridor1.w = b.x - a.x - a.w;

		corridor1.y = (min(a.y + a.h, b.y + b.h) + max(a.y, b.y)) / 2 - 20 / 2;
		corridor1.y = roundToTile(float2{ 0.0f,corridor1.y }).y;
		corridor1.h = 20;

		return vector<SDL_FRect>{corridor1};
	}

	//we make a turn

	SDL_FRect turn;
	turn.w = CORRIDOR_WIDTH;
	turn.h = CORRIDOR_WIDTH;

	if (rand() % 2 == 0)
	{
		turn.x = centerRect(a).x - CORRIDOR_WIDTH/2;
		turn.y = centerRect(b).y - CORRIDOR_WIDTH/2;
	}
	else
	{
		turn.x = centerRect(b).x - CORRIDOR_WIDTH / 2;
		turn.y = centerRect(a).y - CORRIDOR_WIDTH / 2;
	}

	return vector<SDL_FRect>{turn, makeCorridorv2(a, turn)[0], makeCorridorv2(b, turn)[0]};
}

void Dungeon::init()
{
	m_spawnRoomTexture = World::loadTexture("img\\game\\room_background\\spawnRoom.bmp");
	m_corridorRoomTexture = World::loadTexture("img\\game\\room_background\\corridor.bmp");
	m_defaultRoomTexture = World::loadTexture("img\\game\\room_background\\defaultRoom.bmp");

	m_background.texture = World::loadTexture("img\\game\\room_background\\background.bmp");
	m_defaultRoomTexture = World::loadTexture("img\\game\\room_background\\defaultRoom.bmp");
	m_mainRoomTexture = World::loadTexture("img\\game\\room_background\\mainRoom.bmp");
	m_hubRoomTexture = World::loadTexture("img\\game\\room_background\\hubRoom.bmp");
	m_spawnRoomTexture = World::loadTexture("img\\game\\room_background\\spawnRoom.bmp");
	m_mediatorRoomTexture = World::loadTexture("img\\game\\room_background\\mediatorRoom.bmp");
	m_mediatorRoomTextureIn = World::loadTexture("img\\game\\room_background\\mediatorRoomIn.bmp");
	m_mediatorRoomTextureOut = World::loadTexture("img\\game\\room_background\\mediatorRoomOut.bmp");
	m_mediatorRoomTextureMid = World::loadTexture("img\\game\\room_background\\mediatorRoomMid.bmp");
	m_eventRoomTexture = World::loadTexture("img\\game\\room_background\\eventRoom.bmp");
	m_corridorRoomTexture = World::loadTexture("img\\game\\room_background\\corridor.bmp");
	m_tileTexture = World::loadTexture("img\\game\\room_background\\tile.bmp");
	m_wallTexture = World::loadTexture("img\\game\\room_background\\wall.bmp");
	m_wallSideTexture = World::loadTexture("img\\game\\room_background\\wall_side.bmp");

	//inner circle

	m_background.rect = { 0,0,1920, 1080 };

	circle innerZone;
	innerZone.coordinates = DUNGEON_CENTER;
	innerZone.radius = DUNGEON_RADIUS;
	sectorCircles.push_back(innerZone);

	Uint32 start;


	start = SDL_GetTicks();

	spawnRooms();

	cout << "spawnRooms completed for " << SDL_GetTicks() - start << " ms\n";
	start = SDL_GetTicks();
	stirRooms();
	cout << "stirRooms completed for " << SDL_GetTicks() - start << " ms\n";

	/// build graph
	start = SDL_GetTicks();

	
	cout << "graph completed for " << SDL_GetTicks() - start << " ms\n";

	start = SDL_GetTicks();

	int outerZone, midZone;
	assignRooms(outerZone, midZone);
	cout << "assignRooms completed for " << SDL_GetTicks() - start << " ms\n";


	start = SDL_GetTicks();
	delunayTriangulationv2();
	cout << "delunayTriangulation + corridors completed for " << SDL_GetTicks() - start << " ms\n";

	deleteRooms();

	SDL_Delay(5000);

	start = SDL_GetTicks();
	generateTextures();

	scale(SCALE);

	cout << "scale completed for " << SDL_GetTicks() - start << " ms\n";


	cout << "Done\n";
	cout << m_rooms.size();
}

void Dungeon::update()
{

}

void Dungeon::draw()
{
	
	//World::drawObject(m_background);

	for (auto& itr : m_walls)
	{
		World::drawObject(itr);
	}

	for (auto& itr : m_corridors)
	{
		World::drawObject(itr.second.drawable);
	}
	for (auto& itr : m_rooms)
	{
		World::drawObject(itr.second.drawable);
	}


	//for (auto& itr : sectorCircles)
	//{
	//	World::drawCircle(itr.coordinates.x, itr.coordinates.y, itr.radius);
	//}

	for (auto& itr : m_tiles)
	{
		World::drawObject(itr);
	}

	//for (auto& itr : nodes)
	//{
	//	World::drawCircle(itr.coordinates.x,itr.coordinates.y, itr.radius);
	//}

	for (auto& itr : connections)
	{
		World::drawLine(itr,itr.r,itr.g,itr.b,255);
	}

}

void Dungeon::move(int2 move)
{
	for (auto& itr : m_rooms)
	{
		itr.second.drawable.rect.x -= move.x;
		itr.second.drawable.rect.y -= move.y;
	}

	for (auto& itr : m_corridors)
	{
		itr.second.drawable.rect.x -= move.x;
		itr.second.drawable.rect.y -= move.y;
	}	
	
	for (auto& itr : m_tiles)
	{
		itr.rect.x -= move.x;
		itr.rect.y -= move.y;
	}	
	
	for (auto& itr : m_walls)
	{
		itr.rect.x -= move.x;
		itr.rect.y -= move.y;
	}
}

void Dungeon::spawnRooms()
{

	for (int i = 0; i < ROOM_NUMBER; i++)
	{
		float2 spawnPoint;
		float2 dimensions;
		Room newRoom;
		
		bool flag = true;

		while (flag)
		{
			flag = false;


			spawnPoint = roundToTile(getRandomPointInCircle(DUNGEON_RADIUS, DUNGEON_CENTER));

			if (random() % 4 == 0)
			{
				dimensions.x = HUB_MIN_WIDTH + random() % (HUB_MAX_WIDTH - HUB_MIN_WIDTH);
				dimensions.y = HUB_MIN_HEIGHT + random() % (HUB_MAX_HEIGHT - HUB_MIN_HEIGHT);

			}
			else
			{
				dimensions.x = DEF_MIN_WIDTH + random() % (DEF_MAX_WIDTH - DEF_MIN_WIDTH);
				dimensions.y = DEF_MIN_HEIGHT + random() % (DEF_MAX_HEIGHT - DEF_MIN_HEIGHT);
			}

			dimensions = roundToTile(float2{ (float)dimensions.x,(float)dimensions.y });

			newRoom.id = i;
			newRoom.drawable.rect = { spawnPoint.x - dimensions.x/2, spawnPoint.y - dimensions.y/2 ,dimensions.x,dimensions.y };
			newRoom.drawable.texture = m_defaultRoomTexture;

			for (auto& itr : m_rooms)
			{
				if (itr.second.drawable.rect.x == newRoom.drawable.rect.x && itr.second.drawable.rect.y == newRoom.drawable.rect.y
					&& itr.second.drawable.rect.w == newRoom.drawable.rect.w && itr.second.drawable.rect.h == newRoom.drawable.rect.h)
				{
					flag = true;
					cout << "fixed an overriding room\n";
					break;
				}
			}

		}

		m_rooms.insert(make_pair(i, newRoom));

		drawForContent();

	}

	Room mainRoom;
	mainRoom.id = m_rooms.size();
	mainRoom.drawable.rect.x = DUNGEON_CENTER.x - WIDTH_MAX/2;
	mainRoom.drawable.rect.y = DUNGEON_CENTER.y - HEIGTH_MAX/2;
	mainRoom.drawable.rect.w = WIDTH_MAX;
	mainRoom.drawable.rect.h = HEIGTH_MAX;
	mainRoom.drawable.texture = m_mainRoomTexture;
	mainRoom.type = ROOM_TYPE::MAIN;

	mainRoomID = m_rooms.size();

	m_rooms.insert(make_pair(m_rooms.size(), mainRoom));

}

void Dungeon::stirRooms()
{
	/// find each room's 'neighbours' calculate steer vector and apply
	
	bool isCompleted = false;
	map<int, float2> forces; // room ID -> force that should be applied
	vector<Room> neighbours; // computation vector that contains forces

	while (!isCompleted)
	{

		forces.clear();

		for (auto& itr1 : m_rooms)
		{

			if (itr1.first == m_rooms.size() - 1)
			{
				continue;
			}

			neighbours.clear();	


			for (auto& itr2 : m_rooms)
			{
				if (collRectRect(wrap(itr1.second.drawable.rect),wrap(itr2.second.drawable.rect)))
				{
					neighbours.push_back(itr2.second);
				}
			}

			//calculate computation vector and store it
			float2 force;
			force.reset();

			for (int i = 0; i < neighbours.size(); i++)
			{
				force.x += neighbours[i].getCenter().x - itr1.second.getCenter().x;
				force.y += neighbours[i].getCenter().y - itr1.second.getCenter().y;
			}

			//invert the vector and divide based on neighbours count

			force.x *= -1;
			force.y *= -1;

			force.x /= neighbours.size();
			force.y /= neighbours.size();

			forces.insert(make_pair(itr1.first, force));
		}

		//apply forces

		isCompleted = true;

		for (auto& itr : forces)
		{
			itr.second.normalise();

			if (itr.second.x != 0.0f || itr.second.y != 0.0f)
			{
				isCompleted = false;
			}

			m_rooms.at(itr.first).drawable.rect.x += itr.second.x;
			m_rooms.at(itr.first).drawable.rect.y += itr.second.y;
		}

		drawForContent();

	}

	//check for colliding int

	for (auto& itr : m_rooms)
	{
		float2 coor;
		coor.x = itr.second.drawable.rect.x;
		coor.y = itr.second.drawable.rect.y;

		coor = roundToTile(coor);

		itr.second.drawable.rect.x = coor.x;
		itr.second.drawable.rect.y = coor.y;
	}
	
	//for (auto& itr1 : m_rooms)
	//{
	//	for (auto& itr2 : m_rooms)
	//	{
	//		if (itr1.first != itr2.first && collRectRectF(itr1.second.drawable.rect, itr2.second.drawable.rect))
	//		{
	//			cout << "colliding rooms\n";
	//		}
	//	}
	//}

}

void Dungeon::assignRooms(int& outerZone, int& midZone)
{
	/// choose potential spawn rooms
	/// split the map on sectors based on number of teams
	/// each first/second half of each sector will contain 1 spawn room
	/// as spawn room is chosen the furthest one from the center that is in the desired sector
	/// therefore:
	/// 1) spawn rooms of different teams are not close to each other
	/// 2) they are as far away as possible so the game is fair and most of the map will be explored

	
	float midDist = 0;
	float maxDist = 0;

	for (auto& room : m_rooms)
	{
		midDist += distance(room.second.getCenter(), m_rooms.at(mainRoomID).getCenter());
		if (maxDist < distance(room.second.getCenter(), m_rooms.at(mainRoomID).getCenter()))
		{
			maxDist = distance(room.second.getCenter(), m_rooms.at(mainRoomID).getCenter());
		}
	}

	midDist = midDist / (m_rooms.size() - 1);
	outerZone = maxDist;	/// finds dungeon span
	midZone = midDist;		/// find dungeon mid zone 

	vector<int> potentialSpawnRooms;	/// spawn rooms can only spawn in the outer zone of the dungeon
	

	int splitAngle = 360 / NUMBER_OF_TEAMS;

	map<int, vector<int>> sectors;
	

	for (auto& itr : m_rooms)
	{
		float currAngle = fix(returnAngleByCoordinates(int2{ int(itr.second.getCenter().x - DUNGEON_CENTER.x),int(itr.second.getCenter().y - DUNGEON_CENTER.y) }));
		
		if (currAngle < 0)
		{
			currAngle = currAngle + 360;
		}
		
		sectors[int(floor(currAngle / splitAngle))].push_back(itr.first);
	}
	for (auto& sector : sectors)
	{

		line temp;

		temp.start = DUNGEON_CENTER;
		temp.finish.x = temp.start.x + returnCoordinatesByAngle(sector.first * splitAngle).x * outerZone;
		temp.finish.y = temp.start.y + returnCoordinatesByAngle(sector.first * splitAngle).y * outerZone;

		sectorLines.push_back(temp);

		temp.start = DUNGEON_CENTER;
		temp.finish.x = temp.start.x + returnCoordinatesByAngle(sector.first * splitAngle + splitAngle/2).x * outerZone;
		temp.finish.y = temp.start.y + returnCoordinatesByAngle(sector.first * splitAngle + splitAngle/2).y * outerZone;

		sectorLines.push_back(temp);

		vector<pair<int, int>> possibleAngles;	// roomID -> angle

		for (auto& id : sector.second)
		{
			float currDist = distance(m_rooms.at(id).getCenter(), m_rooms.at(m_rooms.size()-1).getCenter());
			float currAngle = fix(returnAngleByCoordinates(int2{ int(m_rooms.at(id).getCenter().x - DUNGEON_CENTER.x),int(m_rooms.at(id).getCenter().y - DUNGEON_CENTER.y) }));
			if (currAngle < 0)
			{
				currAngle = currAngle + 360;
			}
			

			if (currAngle < sector.first * splitAngle + splitAngle/2)
			{
				possibleAngles.push_back(make_pair(id, currDist));
			}
		}
		
		float maxID = 0;

		for (int i = 1; i < possibleAngles.size(); i++)
		{

			if (possibleAngles[i].second > possibleAngles[maxID].second)
			{
				maxID = i;
			}
		}
		m_spawnRoomIDs.push_back(possibleAngles[maxID].first);
		m_rooms.at(possibleAngles[maxID].first).drawable.texture = m_spawnRoomTexture;
		m_rooms.at(possibleAngles[maxID].first).type = ROOM_TYPE::SPAWN;

		
		drawForContent();
	}

	assert(m_spawnRoomIDs.size() == NUMBER_OF_TEAMS);


	bool canPlaceHub = random()%2;
	map<int, bool> sectorHasHub;
	
	for (auto& sector : sectors)
	{

		if (canPlaceHub == false)
		{
			sectorHasHub[sector.first] = false;
			canPlaceHub = true;
			continue;
		}

		for (auto& id : sector.second)
		{
			if (m_rooms.at(id).type != ROOM_TYPE::NONE)
			{
				continue;
			}

			if (m_rooms.at(id).drawable.rect.w < HUB_MIN_WIDTH && m_rooms.at(id).drawable.rect.h < HUB_MIN_HEIGHT)
			{
				continue;
			}

			if (collRectRect(wrap(m_rooms.at(id).drawable.rect), wrap(m_rooms.at(m_rooms.size() - 1).drawable.rect)) == true)										// we are out of inner circle
			{
				continue;
			}

			if (distance(m_rooms.at(id).getCenter(), m_rooms.at(m_rooms.size() - 1).getCenter()) < DUNGEON_RADIUS)
			{
				continue;
			}

			bool roomCol = false;

			for (int i = 0; i < m_spawnRoomIDs.size(); i++)
			{
				if (collRectRect(wrap(m_rooms.at(id).drawable.rect), wrap(m_rooms.at(m_spawnRoomIDs[i]).drawable.rect)) == true)										// we are out of inner circle
				{
					roomCol = true;
					break;
				}
			}

			if (roomCol)
			{
				continue;
			}

			roomCol = false;

			for (int i = 0; i < m_hubRoomIDs.size(); i++)
			{
				if (collRectRect(wrap(m_rooms.at(id).drawable.rect), wrap(m_rooms.at(m_hubRoomIDs[i]).drawable.rect)) == true)										// we are out of inner circle
				{
					roomCol = true;
					break;
				}
			}

			if (roomCol)
			{
				continue;
			}

			// the room doesn't collide with spawn or main or hub, is big enough and isn't occupied

			m_rooms.at(id).type = ROOM_TYPE::HUB;
			m_rooms.at(id).drawable.texture = m_hubRoomTexture;
			m_hubRoomIDs.push_back(id);
			sectorHasHub[sector.first] = true;

			canPlaceHub = false;


			break;

		}
		
		drawForContent();

		if (canPlaceHub)
		{
			sectorHasHub[sector.first] = false;
		}

	}

	for (auto& sector : sectors)
	{
		int maxNumberOfEventRooms;

		if (sectorHasHub.at(sector.first))
		{
			maxNumberOfEventRooms = 1;
		}
		else
		{
			maxNumberOfEventRooms = 2;
		}

		for (auto& id : sector.second)
		{

			if (maxNumberOfEventRooms <= 0)
			{
				break;
			}

			if (m_rooms.at(id).type != ROOM_TYPE::NONE)
			{
				continue;
			}

			if (collRectRect(wrap(m_rooms.at(id).drawable.rect), wrap(m_rooms.at(m_rooms.size() - 1).drawable.rect)) == true)										// we are out of inner circle
			{
				continue;
			}

			if (distance(m_rooms.at(id).getCenter(), m_rooms.at(m_rooms.size() - 1).getCenter()) < DUNGEON_RADIUS)
			{
				continue;
			}

			bool roomCol = false;

			for (int i = 0; i < m_spawnRoomIDs.size(); i++)
			{
				if (collRectRect(wrap(m_rooms.at(id).drawable.rect), wrap(m_rooms.at(m_spawnRoomIDs[i]).drawable.rect)) == true)										// we are out of inner circle
				{
					roomCol = true;
					break;
				}
			}

			if (roomCol)
			{
				continue;
			}

			roomCol = false;

			for (int i = 0; i < m_hubRoomIDs.size(); i++)
			{
				if (collRectRect(wrap(m_rooms.at(id).drawable.rect), wrap(m_rooms.at(m_hubRoomIDs[i]).drawable.rect)) == true)										// we are out of inner circle
				{
					roomCol = true;
					break;
				}
			}

			if (roomCol)
			{
				continue;
			}			
			
			roomCol = false;

			for (int i = 0; i < m_eventRoomIDs.size(); i++)
			{
				if (collRectRect(wrap(m_rooms.at(id).drawable.rect), wrap(m_rooms.at(m_eventRoomIDs[i]).drawable.rect)) == true)										// we are out of inner circle
				{
					roomCol = true;
					break;
				}
			}

			if (roomCol)
			{
				continue;
			}

			m_rooms.at(id).type = ROOM_TYPE::EVENT;
			m_rooms.at(id).drawable.texture = m_eventRoomTexture;
			m_eventRoomIDs.push_back(id);
			maxNumberOfEventRooms--;

			drawForContent();
		}
	}

	//add some default rooms in the mix
	//in every sector get the default room closest to the mid zone and add it

	for (auto& sector : sectors)	// mediator rooms closest to mid zone but inside it
	{

		int bestID = -1;
		int bestDist = outerZone;

		for (auto& id : sector.second)
		{
			if (m_rooms.at(id).type != ROOM_TYPE::NONE)
			{
				continue;
			}
			int currDist = distance(m_rooms.at(id).getCenter(), m_rooms.at(m_rooms.size() - 1).getCenter());

			//if (currDist > midZone)
			//{
			//	continue;
			//}

			if (abs(bestDist - midZone) > abs(currDist - midZone))
			{
				bestID = id;
				bestDist = currDist;
			}

		}

		if (bestID != -1)
		{
			m_mediatorRoomIDs.push_back(bestID);
			m_rooms.at(bestID).type = ROOM_TYPE::DEFAULT;
			m_rooms.at(bestID).drawable.texture = m_mediatorRoomTextureMid;
		}
	}
	
	for (auto& sector : sectors)	// mediator rooms between mid and inner zone, closest to the distance in between
	{

		int bestID = -1;
		int bestDist = midZone;

		for (auto& id : sector.second)
		{
			if (m_rooms.at(id).type != ROOM_TYPE::NONE)
			{
				continue;
			}
			int currDist = distance(m_rooms.at(id).getCenter(), m_rooms.at(m_rooms.size() - 1).getCenter());

			if (currDist < DUNGEON_RADIUS || currDist > midZone/2)
			{
				continue;
			}

			if (abs(bestDist - (midDist + DUNGEON_RADIUS)/2) > abs(currDist - (midDist + DUNGEON_RADIUS)/2))
			{
				bestID = id;
				bestDist = currDist;
			}
		}

		if (bestID != -1)
		{
			m_mediatorRoomIDs.push_back(bestID);
			m_rooms.at(bestID).type = ROOM_TYPE::DEFAULT;
			m_rooms.at(bestID).drawable.texture = m_mediatorRoomTextureIn;

			drawForContent();

		}
	}

	for (auto& sector : sectors)	// mediator rooms in outer zone, furthest
	{

		int bestID = -1;
		int bestDist = midZone;

		for (auto& id : sector.second)
		{
			if (m_rooms.at(id).type != ROOM_TYPE::NONE)
			{
				continue;
			}
			int currDist = distance(m_rooms.at(id).getCenter(), m_rooms.at(m_rooms.size() - 1).getCenter());

			if (currDist < midZone)
			{
				continue;
			}

			if (bestDist < currDist)
			{
				bestID = id;
				bestDist = currDist;
			}
		}

		if (bestID != -1)
		{
			m_mediatorRoomIDs.push_back(bestID);
			m_rooms.at(bestID).type = ROOM_TYPE::DEFAULT;
			m_rooms.at(bestID).drawable.texture = m_mediatorRoomTextureOut;

			drawForContent();

		}
	}	
	
	for (auto& sector : sectors)	// mediator rooms in outer zone, closest to mid
	{

		int bestID = -1;
		int bestDist = midZone;

		for (auto& id : sector.second)
		{
			if (m_rooms.at(id).type != ROOM_TYPE::NONE)
			{
				continue;
			}
			int currDist = distance(m_rooms.at(id).getCenter(), m_rooms.at(m_rooms.size() - 1).getCenter());

			if (currDist < midZone)
			{
				continue;
			}

			if (bestDist > currDist)
			{
				bestID = id;
				bestDist = currDist;
			}
		}

		if (bestID != -1)
		{
			m_mediatorRoomIDs.push_back(bestID);
			m_rooms.at(bestID).type = ROOM_TYPE::DEFAULT;
			m_rooms.at(bestID).drawable.texture = m_mediatorRoomTextureOut;

			drawForContent();

		}
	}

	//update list of parents

}

void Dungeon::delunayTriangulation()
{

	vector<int> buffer;

	for (int i = 0; i < m_spawnRoomIDs.size(); i++)
	{
		buffer.push_back(m_spawnRoomIDs[i]);
	}

	for (int i = 0; i < m_hubRoomIDs.size(); i++)
	{
		buffer.push_back(m_hubRoomIDs[i]);
	}

	for (int i = 0; i < m_eventRoomIDs.size(); i++)
	{
		buffer.push_back(m_eventRoomIDs[i]);
	}

	for (int i = 0; i < m_mediatorRoomIDs.size(); i++)
	{
		buffer.push_back(m_mediatorRoomIDs[i]);
	}

	buffer.push_back(mainRoomID);

	int POINTS = buffer.size();

	typedef double MyCoord;

	struct MyPoint
	{
		MyCoord x;
		MyCoord y;
		int ID;
	};

	struct Edge
	{
		int startID;
		int finishID;
	};

	MyPoint* cloud = new MyPoint[POINTS];

	for (int i = 0; i < buffer.size(); i++)
	{
		cloud[i].x = m_rooms.at(buffer[i]).getCenter().x;
		cloud[i].y = m_rooms.at(buffer[i]).getCenter().y;
		cloud[i].ID = buffer[i];
	}

	IDelaBella2<MyCoord>* idb = IDelaBella2<MyCoord>::Create();

	int verts = idb->Triangulate(POINTS, &cloud->x, &cloud->y, sizeof(MyPoint));

	// if positive, all ok 
	if (verts > 0)
	{
		int tris = idb->GetNumPolygons();
		const IDelaBella2<MyCoord>::Simplex* dela = idb->GetFirstDelaunaySimplex();
		

		int amount = rand() % 10;
		vector<int> ids = getRandomIndexes(0, tris, amount);
		
		for (int i = 0; i < tris; i++)
		{
			// do something with dela triangle 
			// ...

			m_graph[cloud[dela->v[0]->i].ID].insert(make_pair(cloud[dela->v[1]->i].ID, &m_rooms.at(cloud[dela->v[1]->i].ID)));
			m_graph[cloud[dela->v[0]->i].ID].insert(make_pair(cloud[dela->v[2]->i].ID, &m_rooms.at(cloud[dela->v[2]->i].ID)));
			m_graph[cloud[dela->v[1]->i].ID].insert(make_pair(cloud[dela->v[0]->i].ID, &m_rooms.at(cloud[dela->v[0]->i].ID)));
			m_graph[cloud[dela->v[1]->i].ID].insert(make_pair(cloud[dela->v[2]->i].ID, &m_rooms.at(cloud[dela->v[2]->i].ID)));
			m_graph[cloud[dela->v[2]->i].ID].insert(make_pair(cloud[dela->v[0]->i].ID, &m_rooms.at(cloud[dela->v[0]->i].ID)));
			m_graph[cloud[dela->v[2]->i].ID].insert(make_pair(cloud[dela->v[1]->i].ID, &m_rooms.at(cloud[dela->v[1]->i].ID)));

			dela = dela->next;
		}
	}
	else
	{
		// no points given or all points are colinear
		// make emergency call ...
	}

	delete[] cloud;
	idb->Destroy();

	map<int, Node> dag;
	dag[mainRoomID] = Node(-1, 0);
	
	vector<int> BFSq;
	BFSq.push_back(mainRoomID);

	while (!BFSq.empty())
	{
		int currID = BFSq.front();
		BFSq.erase(BFSq.begin());

		for (auto& neighbour : m_graph.at(currID))
		{
			if (dag.find(neighbour.first) == dag.end())
			{
				dag.insert(make_pair(neighbour.first, Node(currID, dag.at(currID).criteria +  distance(m_rooms.at(neighbour.first).getCenter(), m_rooms.at(currID).getCenter()))));
				BFSq.push_back(neighbour.first);
			}
		}

		for (int i = 0; i < BFSq.size(); i++)
		{
			int swapID = i;

			for (int j = i; j < BFSq.size(); j++)
			{
				if (dag.at(BFSq[i]).criteria > dag.at(BFSq[j]).criteria)
				{
					swapID = j;
				}
			}
			
			swap(BFSq[i], BFSq[swapID]);
		}

	}


	//tuka imam dag

	vector<int> ids;

	vector<pair<int, int>> edges;

	for (auto& itr1 : m_graph)
	{
		for (auto& itr2 : itr1.second)
		{
			edges.push_back(make_pair(itr1.first, itr2.first));
		}
	}

	ids = getRandomIndexes(0, edges.size() - 1, ((rand() % 15) + 5));

	//tuk imam random edges


	//tuka clearvam graph
	m_graph.clear();

	//i sq she go pulnim pak

	vector<lineOfIDs> bridges;

	for (int i = 0; i < ids.size(); i++)
	{
		queue<lineOfIDs> connectQ;

		lineOfIDs init;

		init.startID = edges[ids[i]].first;
		init.finishID = edges[ids[i]].second;

		connectQ.push(init);

		while (!connectQ.empty())
		{
			line con;
			
			vector<SDL_FRect> corridors;

			corridors = makeCorridorv2(m_rooms.at(connectQ.front().startID).drawable.rect, m_rooms.at(connectQ.front().finishID).drawable.rect);

			con.start = m_rooms.at(connectQ.front().startID).getCenter();
			con.finish = m_rooms.at(connectQ.front().finishID).getCenter();

			bool hasColl = false;

			for (auto& room : m_rooms)
			{
				if (room.first == connectQ.front().startID || room.first == connectQ.front().finishID)
				{
					continue;
				}

				for (SDL_FRect& rect : corridors)
				{
					if (collRectRectF(rect, room.second.drawable.rect))
					{

						lineOfIDs one;
						lineOfIDs two;

						one.startID = connectQ.front().startID;
						one.finishID = room.first;

						two.startID = connectQ.front().finishID;
						two.finishID = room.first;

						m_graph[one.startID].insert(make_pair(one.finishID, &m_rooms.at(one.finishID)));
						m_graph[one.finishID].insert(make_pair(one.startID, &m_rooms.at(one.startID)));

						m_graph[two.startID].insert(make_pair(two.finishID, &m_rooms.at(two.finishID)));
						m_graph[two.finishID].insert(make_pair(two.startID, &m_rooms.at(two.startID)));

						connectQ.push(one);
						connectQ.push(two);

						hasColl = true;

						break;
					}
				}
				if (hasColl)
				{
					break;
				}
			}

			if (!hasColl)
			{
				m_graph[connectQ.front().startID].insert(make_pair(connectQ.front().finishID, &m_rooms.at(connectQ.front().finishID)));
				m_graph[connectQ.front().finishID].insert(make_pair(connectQ.front().startID, &m_rooms.at(connectQ.front().startID)));
			}

			connectQ.pop();
		}
	}

	for (auto& child : dag)
	{
		if (child.second.ID == -1)
		{
			continue;
		}

		queue<lineOfIDs> connectQ;
	
		lineOfIDs init;
		init.startID = child.first;
		init.finishID = child.second.ID;

		connectQ.push(init);

		while (!connectQ.empty())
		{
			line con;

			vector<SDL_FRect> corridors;

			corridors = makeCorridorv2(m_rooms.at(connectQ.front().startID).drawable.rect, m_rooms.at(connectQ.front().finishID).drawable.rect);

			con.start = m_rooms.at(connectQ.front().startID).getCenter();
			con.finish = m_rooms.at(connectQ.front().finishID).getCenter();

			bool hasColl = false;

			for (auto& room : m_rooms)
			{
				if (room.first == connectQ.front().startID || room.first == connectQ.front().finishID)
				{
					continue;
				}
				for (SDL_FRect& rect : corridors)
				{

					if (collRectRectF(rect, room.second.drawable.rect))
					{

						lineOfIDs one;
						lineOfIDs two;

						one.startID = connectQ.front().startID;
						one.finishID = room.first;

						two.startID = connectQ.front().finishID;
						two.finishID = room.first;

						m_graph[one.startID].insert(make_pair(one.finishID, &m_rooms.at(one.finishID)));
						m_graph[one.finishID].insert(make_pair(one.startID, &m_rooms.at(one.startID)));

						m_graph[two.startID].insert(make_pair(two.finishID, &m_rooms.at(two.finishID)));
						m_graph[two.finishID].insert(make_pair(two.startID, &m_rooms.at(two.startID)));

						connectQ.push(one);
						connectQ.push(two);

						hasColl = true;

						break;
					}
				}
				if (hasColl)
				{
					break;
				}
			}

			if (!hasColl)
			{
				m_graph[connectQ.front().startID].insert(make_pair(connectQ.front().finishID, &m_rooms.at(connectQ.front().finishID)));
				m_graph[connectQ.front().finishID].insert(make_pair(connectQ.front().startID, &m_rooms.at(connectQ.front().startID)));
			}

			connectQ.pop();
		}
	}


	map<int, map<int, bool>> hasBridge;

	for (auto& id : m_graph)
	{
		for (auto& neighbour : id.second)
		{
			if (hasBridge[id.first].find(neighbour.first) == hasBridge[id.first].end())
			{
				lineOfIDs bridge;
				bridge.startID = id.first;
				bridge.finishID = neighbour.first;

				bridges.push_back(bridge);

				hasBridge[id.first].insert(make_pair(neighbour.first, true));
				hasBridge[neighbour.first].insert(make_pair(id.first, true));
			}
		}
	}


	for (auto& id : m_graph)
	{
		for (auto& neighbour : id.second)
		{
			lineC con;
			con.r = 37;
			con.g = 150;
			con.b = 190;

			con.start = m_rooms.at(id.first).getCenter();
			con.finish = m_rooms.at(neighbour.first).getCenter();

			connections.push_back(con);
		}
	}



	for (int i = 0; i < bridges.size(); i++)
	{
		vector<SDL_FRect> cor = makeCorridorv2(m_rooms.at(bridges[i].startID).drawable.rect,m_rooms.at(bridges[i].finishID).drawable.rect);

		
		for (SDL_FRect& rect : cor)
		{
			Corridor newCor;
			assert(rect.w != 0 && rect.h != 0);
			newCor.drawable.rect = rect;
			newCor.drawable.texture = m_corridorRoomTexture;
			newCor.id = i;

			m_corridors.insert(make_pair(i, newCor));
		}

		drawForDebug();
	}


	// ...

	//for (auto& head : m_graph)
	//{
	//	for (auto& neighbour : head.second)
	//	{
	//		line connect;
	//		connect.start = m_rooms.at(head.first).getCenter();
	//		connect.finish = m_rooms.at(neighbour.first).getCenter();
	//		connections.push_back(connect);	
	//	}
	//}

	vector<int> todelete;


	for (line& con : connections)
	{
		for (auto& room : m_rooms)
		{
			if (collLineAndSDL_FRect(con, room.second.drawable.rect) && room.second.type == ROOM_TYPE::NONE)
			{
				room.second.type = ROOM_TYPE::DEFAULT;
			}
		}
	}

	for (auto& room : m_rooms)
	{
		if (room.second.type == ROOM_TYPE::NONE)
		{
			todelete.push_back(room.first);
		}
	}


	for (int& id : todelete)
	{
		m_rooms.erase(id);
	}

}

void Dungeon::delunayTriangulationv2()
{

	vector<int> buffer;

	for (int i = 0; i < m_spawnRoomIDs.size(); i++)
	{
		buffer.push_back(m_spawnRoomIDs[i]);
	}

	for (int i = 0; i < m_hubRoomIDs.size(); i++)
	{
		buffer.push_back(m_hubRoomIDs[i]);
	}

	for (int i = 0; i < m_eventRoomIDs.size(); i++)
	{
		buffer.push_back(m_eventRoomIDs[i]);
	}

	for (int i = 0; i < m_mediatorRoomIDs.size(); i++)
	{
		buffer.push_back(m_mediatorRoomIDs[i]);
	}

	buffer.push_back(mainRoomID);

	int POINTS = buffer.size();

	typedef double MyCoord;

	struct MyPoint
	{
		MyCoord x;
		MyCoord y;
		int ID;
	};

	struct Edge
	{
		int startID;
		int finishID;
	};

	MyPoint* cloud = new MyPoint[POINTS];

	for (int i = 0; i < buffer.size(); i++)
	{
		cloud[i].x = m_rooms.at(buffer[i]).getCenter().x;
		cloud[i].y = m_rooms.at(buffer[i]).getCenter().y;
		cloud[i].ID = buffer[i];
	}

	IDelaBella2<MyCoord>* idb = IDelaBella2<MyCoord>::Create();

	int verts = idb->Triangulate(POINTS, &cloud->x, &cloud->y, sizeof(MyPoint));

	// if positive, all ok 
	if (verts > 0)
	{
		int tris = idb->GetNumPolygons();
		const IDelaBella2<MyCoord>::Simplex* dela = idb->GetFirstDelaunaySimplex();


		int amount = rand() % 10;
		vector<int> ids = getRandomIndexes(0, tris, amount);

		for (int i = 0; i < tris; i++)
		{
			// do something with dela triangle 
			// ...

			m_graph[cloud[dela->v[0]->i].ID].insert(make_pair(cloud[dela->v[1]->i].ID, &m_rooms.at(cloud[dela->v[1]->i].ID)));
			m_graph[cloud[dela->v[0]->i].ID].insert(make_pair(cloud[dela->v[2]->i].ID, &m_rooms.at(cloud[dela->v[2]->i].ID)));
			m_graph[cloud[dela->v[1]->i].ID].insert(make_pair(cloud[dela->v[0]->i].ID, &m_rooms.at(cloud[dela->v[0]->i].ID)));
			m_graph[cloud[dela->v[1]->i].ID].insert(make_pair(cloud[dela->v[2]->i].ID, &m_rooms.at(cloud[dela->v[2]->i].ID)));
			m_graph[cloud[dela->v[2]->i].ID].insert(make_pair(cloud[dela->v[0]->i].ID, &m_rooms.at(cloud[dela->v[0]->i].ID)));
			m_graph[cloud[dela->v[2]->i].ID].insert(make_pair(cloud[dela->v[1]->i].ID, &m_rooms.at(cloud[dela->v[1]->i].ID)));

			//matrix[cloud[dela->v[0]->i].ID][cloud[dela->v[1]->i].ID];
			//matrix[cloud[dela->v[0]->i].ID][cloud[dela->v[2]->i].ID];
			//matrix[cloud[dela->v[1]->i].ID][cloud[dela->v[0]->i].ID];
			//matrix[cloud[dela->v[1]->i].ID][cloud[dela->v[2]->i].ID];
			//matrix[cloud[dela->v[2]->i].ID][cloud[dela->v[0]->i].ID];
			//matrix[cloud[dela->v[2]->i].ID][cloud[dela->v[1]->i].ID];

			dela = dela->next;
		}
	}
	else
	{
		// no points given or all points are colinear
		// make emergency call ...
	}

	delete[] cloud;
	idb->Destroy();

	map<int, Node> dag;
	dag[mainRoomID] = Node(-1, 0);

	vector<int> BFSq;
	BFSq.push_back(mainRoomID);

	while (!BFSq.empty())
	{
		int currID = BFSq.front();
		BFSq.erase(BFSq.begin());

		for (auto& neighbour : m_graph.at(currID))
		{
			if (dag.find(neighbour.first) == dag.end())
			{
				dag.insert(make_pair(neighbour.first, Node(currID, dag.at(currID).criteria + distance(m_rooms.at(neighbour.first).getCenter(), m_rooms.at(currID).getCenter()))));
				BFSq.push_back(neighbour.first);
			}
		}

		for (int i = 0; i < BFSq.size(); i++)
		{
			int swapID = i;

			for (int j = i; j < BFSq.size(); j++)
			{
				if (dag.at(BFSq[i]).criteria > dag.at(BFSq[j]).criteria)
				{
					swapID = j;
				}
			}

			swap(BFSq[i], BFSq[swapID]);
		}

	}

	//tuka imam dag

	vector<int> ids;

	vector<pair<int, int>> edges;

	for (auto& itr1 : m_graph)
	{
		for (auto& itr2 : itr1.second)
		{
			edges.push_back(make_pair(itr1.first, itr2.first));
		}
	}

	ids = getRandomIndexes(0, edges.size() - 1, ((rand() % 15) + 5));

	//tuk imam random edges


	//filling matrix

	vector<vector<int>> matrix;

	matrix.resize(m_rooms.size());
	
	for (int i = 0; i < matrix.size(); i++)
	{
		matrix[i].resize(m_rooms.size());

		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (i == j)
			{
				matrix[i][j] = true;
			}
			else
			{
				matrix[i][j] = false;
			}
		}
	}

	for (int& id : ids)
	{
		matrix[edges[id].first][edges[id].second] = true;
		matrix[edges[id].second][edges[id].first] = true;
	}

	for (auto& child : dag)
	{
		if (child.second.ID == -1)
		{
			continue;
		}

		matrix[child.first][child.second.ID] = true;
		matrix[child.second.ID][child.first] = true;
	}


	vector<vector<bool>> isConnected;
	isConnected.resize(m_rooms.size());
	for (int i = 0; i < isConnected.size(); i++)
	{
		isConnected[i].resize(m_rooms.size());
		for (int j = 0; j < isConnected[i].size(); j++)
		{
			if (i == j)
			{
				isConnected[i][j] = true;
			}
			else
			{
				isConnected[i][j] = false;
			}
		}
	}

	vector<SDL_FRect> res;

	//lets check them lines ffs
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (matrix[i][j])
			{
				/*lineC con;
				con.start = m_rooms.at(i).getCenter();
				con.finish = m_rooms.at(j).getCenter();*/

				queue<pair<int, int>> connectQ;

				connectQ.push(make_pair(i, j));

				while (connectQ.empty() == false)
				{
					auto con = connectQ.front();
					connectQ.pop();

					//first we check for line coll
					//if yes we put in the Q
					//if no we check for bridge coll
					//if bridge coll we 

					if (isConnected[con.first][con.second])
					{
						continue;
					}


					line cLine;
					cLine.start = m_rooms.at(con.first).getCenter();
					cLine.finish = m_rooms.at(con.second).getCenter();

					bool hasColl = false;

					for (auto& room : m_rooms)
					{
						if (room.first == con.first || room.first == con.second)
						{
							continue;
						}

						if (collLineAndSDL_FRect(cLine, room.second.drawable.rect))
						{
							room.second.type = ROOM_TYPE::DEFAULT;

							connectQ.push(make_pair(con.first, room.first));
							connectQ.push(make_pair(con.second, room.first));

							hasColl = true;
							break;
						}
					}

					if (hasColl)
					{
						continue;
					}

					vector<SDL_FRect> bridge;
					bridge = makeCorridorv2(m_rooms.at(con.first).drawable.rect, m_rooms.at(con.second).drawable.rect);

					for (auto& room : m_rooms)
					{
						if (room.first == con.first || room.first == con.second)
						{
							continue;
						}

						for (SDL_FRect& rect : bridge)
						{
							if (collRectRectF(rect, room.second.drawable.rect))
							{
								room.second.type = ROOM_TYPE::DEFAULT;

								connectQ.push(make_pair(con.first, room.first));
								connectQ.push(make_pair(con.second, room.first));

								hasColl = true;
								break;
							}
						}

						if (hasColl)
						{
							break;
						}
					}

					if (hasColl)
					{
						continue;
					}


					isConnected[con.first][con.second] = true;
					isConnected[con.second][con.first] = true;

					for (SDL_FRect& rect : bridge)
					{
						res.push_back(rect);
					}

				}

			}
		}
	}


	//tuka clearvam graph
	m_graph.clear();

	//i sq she go pulnim pak



	for (int i = 0; i < res.size(); i++)
	{
		Corridor newCor;
		newCor.drawable.rect = res[i];
		newCor.drawable.texture = m_corridorRoomTexture;
		newCor.id = i;

		m_corridors.insert(make_pair(i, newCor));
	}

}

void Dungeon::deleteRooms()
{
	vector<int> toDelete;

	for (auto& room : m_rooms)
	{
		if (room.second.type == ROOM_TYPE::NONE)
		{
			toDelete.push_back(room.first);
		}
	}

	for (int& id : toDelete)
	{
		m_rooms.erase(id);
	}

	drawForContent();
}

void Dungeon::scale(float p)
{
		for (auto& itr : m_rooms)
		{
			itr.second.drawable.rect.x *= p;
			itr.second.drawable.rect.y *= p;
			itr.second.drawable.rect.w *= p;
			itr.second.drawable.rect.h *= p;
		}

		for (auto& itr : m_corridors)
		{
			itr.second.drawable.rect.x *= p;
			itr.second.drawable.rect.y *= p;
			itr.second.drawable.rect.w *= p;
			itr.second.drawable.rect.h *= p;
		}

		for (auto& itr : m_tiles)
		{
			itr.rect.x *= p;
			itr.rect.y *= p;
			itr.rect.w *= p;
			itr.rect.h *= p;
		}

		for (auto& itr : m_walls)
		{
			itr.rect.x *= p;
			itr.rect.y *= p;
			itr.rect.w *= p;
			itr.rect.h *= p;
		}

		int2 move1;
		move1.x = 960 - m_rooms[mainRoomID].getCenter().x;
		move1.y = 540 - m_rooms[mainRoomID].getCenter().y;

		move1.x *= -1;
		move1.y *= -1;

		move(move1);

		drawForContent();

	TILE_SIZE *= SCALE;
	WALL_HEIGHT *= SCALE;
	WALL_WIDTH *= SCALE;
}

void Dungeon::castToTile()
{
}

void Dungeon::generateTextures()
{
	for (auto& itr : m_corridors)
	{

		for (int i = 0; i < itr.second.drawable.rect.w / TILE_SIZE; i++)
		{
			for (int j = 0; j < itr.second.drawable.rect.h / TILE_SIZE; j++)
			{
				DrawableF tile;
				tile.rect.x = itr.second.drawable.rect.x + i * TILE_SIZE;
				tile.rect.y = itr.second.drawable.rect.y + j * TILE_SIZE;
				tile.rect.w = TILE_SIZE;
				tile.rect.h = TILE_SIZE;
				tile.texture = m_tileTexture;

				m_tiles.push_back(tile);


			}

		}
		drawForContent();
	}

	for (auto& itr : m_rooms)
	{

		for (int i = 0; i < itr.second.drawable.rect.w / TILE_SIZE; i++)
		{
			for (int j = 0; j < itr.second.drawable.rect.h / TILE_SIZE; j++)
			{
				DrawableF tile;
				tile.rect.x = itr.second.drawable.rect.x + i * TILE_SIZE;
				tile.rect.y = itr.second.drawable.rect.y + j * TILE_SIZE;

				tile.rect.w = TILE_SIZE;
				tile.rect.h = TILE_SIZE;
				tile.texture = m_tileTexture;

				m_tiles.push_back(tile);

			}

		}
		drawForContent();
	}


	for (auto& itr : m_rooms)
	{
		for (int i = -1; i < itr.second.drawable.rect.w / TILE_SIZE + 1; i++)
		{
			Wall botWall;

			botWall.rect.x = itr.second.drawable.rect.x + i * TILE_SIZE;
			botWall.rect.y = itr.second.drawable.rect.y + itr.second.drawable.rect.h;
			botWall.rect.w = TILE_SIZE;
			botWall.rect.h = WALL_HEIGHT;
			botWall.texture = m_wallTexture;
			botWall.z = itr.second.drawable.rect.y + itr.second.drawable.rect.h / 2;


			m_walls.push_back(botWall);

		}
		drawForContent();
	}

	for (auto& itr : m_corridors)
	{
		for (int i = 0; i < itr.second.drawable.rect.w / TILE_SIZE; i++)
		{

			Wall botWall;

			botWall.rect.x = itr.second.drawable.rect.x + i * TILE_SIZE;
			botWall.rect.y = itr.second.drawable.rect.y + itr.second.drawable.rect.h;
			botWall.rect.w = TILE_SIZE;
			botWall.rect.h = WALL_HEIGHT;
			botWall.texture = m_wallTexture;

			m_walls.push_back(botWall);

		}
		drawForContent();

	}

	for (auto& itr : m_corridors)
	{
		for (int j = -1; j < itr.second.drawable.rect.h / TILE_SIZE; j++)
		{
			Wall leftWall;

			leftWall.rect.x = itr.second.drawable.rect.x - WALL_WIDTH;
			leftWall.rect.y = itr.second.drawable.rect.y + j * TILE_SIZE;
			leftWall.rect.w = WALL_WIDTH;
			leftWall.rect.h = TILE_SIZE;
			leftWall.texture = m_wallSideTexture;
			leftWall.z = itr.second.drawable.rect.y + itr.second.drawable.rect.h / 2;

			Wall rightWall;

			rightWall.rect.x = itr.second.drawable.rect.x + itr.second.drawable.rect.w;
			rightWall.rect.y = itr.second.drawable.rect.y + j * TILE_SIZE;
			rightWall.rect.w = WALL_WIDTH;
			rightWall.rect.h = TILE_SIZE;
			rightWall.texture = m_wallSideTexture;
			rightWall.z = itr.second.drawable.rect.y + itr.second.drawable.rect.h / 2;


			m_walls.push_back(leftWall);
			m_walls.push_back(rightWall);
		}
		drawForContent();


	}
	
	for (auto& itr : m_rooms)
	{
		for (int i = -1; i < itr.second.drawable.rect.w / TILE_SIZE + 1; i++)
		{
			Wall topWall;

			topWall.rect.x = itr.second.drawable.rect.x + i * TILE_SIZE;
			topWall.rect.y = itr.second.drawable.rect.y - WALL_HEIGHT;
			topWall.rect.w = TILE_SIZE;
			topWall.rect.h = WALL_HEIGHT;
			topWall.texture = m_wallTexture;
			topWall.z = itr.second.drawable.rect.y + itr.second.drawable.rect.h / 2;


			m_walls.push_back(topWall);

		}
		drawForContent();

	}

	for (auto& itr : m_rooms)
	{
		for (int j = -1; j < itr.second.drawable.rect.h / TILE_SIZE; j++)
		{
			Wall leftWall;

			leftWall.rect.x = itr.second.drawable.rect.x - WALL_WIDTH;
			leftWall.rect.y = itr.second.drawable.rect.y + j * TILE_SIZE;
			leftWall.rect.w = WALL_WIDTH;
			leftWall.rect.h = TILE_SIZE;
			leftWall.texture = m_wallSideTexture;
			leftWall.z = itr.second.drawable.rect.y + itr.second.drawable.rect.h / 2;


			Wall rightWall;

			rightWall.rect.x = itr.second.drawable.rect.x + itr.second.drawable.rect.w;
			rightWall.rect.y = itr.second.drawable.rect.y + j * TILE_SIZE;
			rightWall.rect.w = WALL_WIDTH;
			rightWall.rect.h = TILE_SIZE;
			rightWall.texture = m_wallSideTexture;
			rightWall.z = itr.second.drawable.rect.y + itr.second.drawable.rect.h / 2;


			m_walls.push_back(leftWall);
			m_walls.push_back(rightWall);
		}
		drawForContent();

	}

	for (auto& itr : m_corridors)
	{
		for (int i = 0; i < itr.second.drawable.rect.w / TILE_SIZE; i++)
		{
			Wall topWall;

			topWall.rect.x = itr.second.drawable.rect.x + i * TILE_SIZE;
			topWall.rect.y = itr.second.drawable.rect.y - WALL_HEIGHT;
			topWall.rect.w = TILE_SIZE;
			topWall.rect.h = WALL_HEIGHT;
			topWall.texture = m_wallTexture;
			topWall.z = itr.second.drawable.rect.y + itr.second.drawable.rect.h / 2;
			m_walls.push_back(topWall);

		}
		drawForContent();

	}


	/*for (int i = 0; i < m_walls.size(); i++)
	{
		int maxID = i;

		for (int j = i; j < m_walls.size(); j++)
		{
			if (m_walls[maxID].z > m_walls[j].z)
			{
				maxID = j;
			}
		}

		swap(m_walls[i], m_walls[maxID]);
	}*/

}

void Dungeon::drawForContent()
{
	SDL_RenderClear(World::m_renderer);

	//World::drawObject(m_background);

	for (auto& itr : m_walls)
	{
		World::drawObject(itr);
	}

	for (auto& itr : m_corridors)
	{
		World::drawObject(itr.second.drawable);
	}
	for (auto& itr : m_rooms)
	{
		World::drawObject(itr.second.drawable);
	}

	for (auto& itr : m_tiles)
	{
		World::drawObject(itr);
	}

	//for (auto& itr : sectorCircles)
	//{
	//	World::drawCircle(itr.coordinates.x, itr.coordinates.y, itr.radius);
	//}
	for (auto& itr : nodes)
	{
		World::drawCircle(itr.coordinates.x, itr.coordinates.y, itr.radius);
	}

	for (auto& itr : connections)
	{
		World::drawLine(itr, 100, 100, 100, 255);
	}

	SDL_RenderPresent(World::m_renderer);
}

void Dungeon::drawForDebug()
{
	SDL_RenderClear(World::m_renderer);

//World::drawObject(m_background);

for (auto& itr : m_walls)
{
	World::drawObject(itr);
}

for (auto& itr : m_corridors)
{
	World::drawObject(itr.second.drawable);
}
for (auto& itr : m_rooms)
{
	World::drawObject(itr.second.drawable);
}

for (auto& itr : m_tiles)
{
	World::drawObject(itr);
}

for (auto& itr : sectorCircles)
{
	World::drawCircle(itr.coordinates.x, itr.coordinates.y, itr.radius);
}
for (auto& itr : nodes)
{
	World::drawCircle(itr.coordinates.x, itr.coordinates.y, itr.radius);
}

for (auto& itr : connections)
{
	World::drawLine(itr, 100, 100, 100, 255);
}

SDL_RenderPresent(World::m_renderer);
}

vector<line> Dungeon::generateCorridor(SDL_FRect start, SDL_FRect finish)
{
	if (centerRect(start).x > finish.x && centerRect(start).x < finish.x + finish.w
		&& centerRect(finish).x > start.x && centerRect(finish).x < start.x + start.w)
	{
		if (abs(centerRect(start).x - centerRect(finish).x) >= CORRIDOR_WIDTH)
		{
			line corridor;
			corridor.start.x = (centerRect(start).x + centerRect(finish).x) / 2;
			corridor.finish.x = corridor.start.x;
			corridor.start.y = centerRect(start).y;
			corridor.finish.y = centerRect(finish).y;

			Corridor a;

			float2 coord;
			float2 dim;

			a.id = m_corridors.size();
			coord.x = (centerRect(start).x + centerRect(finish).x) / 2 - CORRIDOR_WIDTH / 2;
			dim.x = CORRIDOR_WIDTH;
			coord.y = min(centerRect(start).y,centerRect(finish).y);
			dim.y = abs(centerRect(start).y - centerRect(finish).y);
			a.drawable.texture = m_corridorRoomTexture;

			coord = roundToTile(coord);
			dim = roundToTile(dim);

			a.drawable.rect = { coord.x, coord.y, dim.x, dim.y };

			m_corridors.insert(make_pair(a.id, a));
	
			return vector<line>{corridor};
		}
	}

	if (centerRect(start).y > finish.y && centerRect(start).y < finish.y + finish.h
		&& centerRect(finish).y > start.y && centerRect(finish).y < start.y + start.y)
	{
		if (abs(centerRect(start).y - centerRect(finish).y) >= CORRIDOR_WIDTH)
		{
			line corridor;
			corridor.start.y = (centerRect(start).y + centerRect(finish).y) / 2;
			corridor.finish.y = corridor.start.y;
			corridor.start.x = centerRect(start).x;
			corridor.finish.x = centerRect(finish).x;

			float2 coord;
			float2 dim;

			Corridor a;
			a.id = m_corridors.size();
			coord.y = (centerRect(start).y + centerRect(finish).y) / 2 - CORRIDOR_WIDTH / 2;
			dim.y = CORRIDOR_WIDTH;
			coord.x = min(centerRect(start).x, centerRect(finish).x);
			dim.x = abs(centerRect(start).x - centerRect(finish).x);
			a.drawable.texture = m_corridorRoomTexture;

			coord = roundToTile(coord);
			dim = roundToTile(dim);

			a.drawable.rect = { coord.x, coord.y, dim.x, dim.y };

			m_corridors.insert(make_pair(a.id, a));
			
			return vector<line>{corridor};
		}
	}

	line a, b;
	a.start = centerRect(start);
	b.finish = centerRect(finish);

	Corridor one, two;
	float2 pivot;

	if (distance(float2{ centerRect(start).x,centerRect(finish).y }, m_rooms.at(mainRoomID).getCenter())
		< distance(float2{ centerRect(finish).x,centerRect(start).y }, m_rooms.at(mainRoomID).getCenter()))
	{
		a.finish = float2{ centerRect(start).x,centerRect(finish).y };
		b.start = a.finish;

		pivot = a.finish;

	}
	else
	{
		a.finish = float2{ centerRect(finish).x,centerRect(start).y };
		b.start = a.finish;

		pivot = a.finish;
	};

	float2 coord;
	float2 dim;

	if (centerRect(start).y == pivot.y)
	{
		coord.x = min(centerRect(start).x, pivot.x) - CORRIDOR_WIDTH/2;
		coord.y = pivot.y - CORRIDOR_WIDTH/2;
		dim.x = abs(centerRect(start).x - pivot.x) + CORRIDOR_WIDTH / 2;
		dim.y = CORRIDOR_WIDTH;


		coord = roundToTile(coord);
		dim = roundToTile(dim);

		one.drawable.rect = { coord.x, coord.y, dim.x, dim.y };

		coord.y = min(centerRect(finish).y, pivot.y) - CORRIDOR_WIDTH / 2;
		coord.x = pivot.x - CORRIDOR_WIDTH/2;
		dim.y = abs(centerRect(finish).y - pivot.y);
		dim.x = CORRIDOR_WIDTH;


		coord = roundToTile(coord);
		dim = roundToTile(dim);

		two.drawable.rect = { coord.x, coord.y, dim.x, dim.y };

	}
	else
	{
		coord.x = min(centerRect(finish).x, pivot.x) - CORRIDOR_WIDTH / 2;
		coord.y = pivot.y - CORRIDOR_WIDTH / 2;
		dim.x = abs(centerRect(finish).x - pivot.x) + CORRIDOR_WIDTH / 2;
		dim.y = CORRIDOR_WIDTH;


		coord = roundToTile(coord);
		dim = roundToTile(dim);

		two.drawable.rect = { coord.x, coord.y, dim.x, dim.y };

		coord.y = min(centerRect(start).y, pivot.y) - CORRIDOR_WIDTH / 2;
		coord.x = pivot.x - CORRIDOR_WIDTH / 2;
		dim.y = abs(centerRect(start).y - pivot.y);
		dim.x = CORRIDOR_WIDTH;


		coord = roundToTile(coord);
		dim = roundToTile(dim);

		one.drawable.rect = { coord.x, coord.y, dim.x, dim.y };
	}


	one.id = m_corridors.size();
	one.drawable.texture = m_corridorRoomTexture;
	m_corridors.insert(make_pair(one.id, one));


	two.id = m_corridors.size();
	two.drawable.texture = m_corridorRoomTexture;
	m_corridors.insert(make_pair(two.id, two));

	return vector<line>{a, b};
}



int Dungeon::random()
{
	return rand();
}

int2 Dungeon::roundToTile(float2 point)
{
	int2 res;

	res.x = round(point.x / TILE_SIZE) * TILE_SIZE;
	res.y = round(point.y / TILE_SIZE) * TILE_SIZE;

	return res;
}



float2 Dungeon::getRandomPointInCircle(int radius, int2 center)
{
	
	double theta = 2 * M_PI * double(random()) / RAND_MAX;
	double r = radius * sqrt(double(random()) / RAND_MAX);

	float2 res;
	res.x = center.x + r * cos(theta);
	res.y = center.y + r * sin(theta);

	return res;
}

SDL_Rect Dungeon::castToRect(SDL_FRect rect)
{
	return SDL_Rect{ int(round(rect.x)),int(round(rect.y)),int(round(rect.w)),int(round(rect.h)) };
}

float2 Dungeon::centerRect(SDL_FRect rect)
{
	return float2{ rect.x + rect.w / 2, rect.y + rect.h / 2 };
}

int2 Dungeon::centerRect(SDL_Rect rect)
{
	return int2{ rect.x + rect.w / 2, rect.y + rect.h / 2 };

}

SDL_Rect Dungeon::wrap(SDL_FRect rect)
{
	//return SDL_Rect{ int(rect.x),int(rect.y),int(rect.w),int(rect.h) };
	return SDL_Rect{ int(rect.x - TILE_SIZE), int(rect.y - TILE_SIZE), int(rect.w + TILE_SIZE), int(rect.h + TILE_SIZE) };
}

SDL_FRect Dungeon::wrapF(SDL_FRect rect)
{
	return SDL_FRect{ (rect.x - TILE_SIZE), (rect.y - TILE_SIZE), (rect.w + TILE_SIZE), (rect.h + TILE_SIZE) };

}

bool Dungeon::collLineLine(line a, line b)
{
	float uA = ((b.finish.x - b.start.x) * (a.start.y - b.start.y) - (b.finish.y - b.start.y) * (a.start.x - b.start.x)) / ((b.finish.y - b.start.y) * (a.finish.x - a.start.x) - (b.finish.x - b.start.x) * (a.finish.y - a.start.y));
	float uB = ((a.finish.x - a.start.x) * (a.start.y - b.start.y) - (a.finish.y - a.start.y) * (a.start.x - b.start.x)) / ((b.finish.y - b.start.y) * (a.finish.x - a.start.x) - (b.finish.x - b.start.x) * (a.finish.y - a.start.y));

	//cout << (b.finish.x - b.start.x) * (a.finish.y - a.start.y) << endl;

	if (uA >= 0 && uB >= 0 && uA <= 1 && uB <= 1)
	{
		return true;
	}

	return false;
}

bool Dungeon::collRectRect(SDL_Rect a, SDL_Rect b)
{
	if (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y)
		return true;

	return false;
}

bool Dungeon::collRectRectF(SDL_FRect a, SDL_FRect b)
{
	if (a.x <= b.x + b.w && a.x + a.w >= b.x && a.y <= b.y + b.h && a.y + a.h >= b.y)
		return true;

	return false;
}

bool Dungeon::collRectRectF_NOTPERFECT(SDL_FRect a, SDL_FRect b)
{
	if (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y)
		return true;

	return false;
}

bool Dungeon::collLineAndSDL_FRect(line mainLine, SDL_FRect a)
{
	line top = { top.start.x = a.x , top.start.y = a.y , top.finish.x = a.x + a.w , top.finish.y = a.y };
	line left = { left.start.x = a.x , left.start.y = a.y , left.finish.x = a.x , left.finish.y = a.y + a.h };
	line bottom = { bottom.start.x = a.x , bottom.start.y = a.y + a.h , bottom.finish.x = a.x + a.w , bottom.finish.y = a.y + a.h };
	line right = { right.start.x = a.x + a.w , right.start.y = a.y , right.finish.x = a.x + a.w , right.finish.y = a.y + a.h };

	if (collLineLine(mainLine, top) ||
		collLineLine(mainLine, bottom) ||
		collLineLine(mainLine, left) ||
		collLineLine(mainLine, right))
	{
		return true;
	}

	return false;
}

float Dungeon::pitagoreanTheorem(float a, float b)
{
	return sqrt(pow(a,2) + pow(b,2));
}

float2 Dungeon::returnCoordinatesByAngle(float angle)
{

		angle -= 90;

		float2 direction;

		direction.y = sin(angle * M_PI / 180);
		direction.x = cos(angle * M_PI / 180);

		return direction;
	
}

float Dungeon::distance(float2 coordA, float2 coordB)
{
	return sqrt(pow(coordA.x - coordB.x, 2) + pow(coordA.y - coordB.y, 2));
}

float Dungeon::returnAngleByCoordinates(int2 direction)
{
	return atan2(direction.x, -1 * direction.y) * 180 / M_PI;
}

float Dungeon::fix(float a)
{
	if (a >= 0)
	{
		return floor(a);
	}
	else
	{
		return ceil(a);
	}
}

vector<int> Dungeon::getRandomIndexes(int start, int finish, int amount)
{
	vector<int> copy;

	for (int i = start; i <= finish; i++)
	{
		copy.push_back(i);
	}

	vector<int> result;

	while (result.size() != amount)
	{
		if (copy.size() == 0)
		{
			break;
		}

		int randomIndex = 0 + rand() % copy.size();

		result.push_back(copy[randomIndex]);

		copy.erase(copy.begin() + randomIndex);
	}

	return result;
}
