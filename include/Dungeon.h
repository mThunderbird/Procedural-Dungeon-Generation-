#pragma once

#include "defines.h"
#include "delabella.h"
#include <algorithm>

enum class ROOM_TYPE
{
	NONE = 0,

	DEFAULT = 1,
	MAIN = 2,
	HUB = 3,
	EVENT = 4,
	SPAWN = 5,

	END = 6

};

struct Room
{
	int id;
	DrawableF drawable;

	ROOM_TYPE type = ROOM_TYPE::NONE;

	float2 getCenter()
	{
		return float2{ drawable.rect.x + drawable.rect.w / 2, drawable.rect.y + drawable.rect.h / 2 };
	}
	void move(float2 a)
	{
		drawable.rect.x += a.x;
		drawable.rect.y += a.y;
	}
};

struct Corridor
{
	int id;
	DrawableF drawable;

	vector<string> roomsItReaches;
};

struct line
{
	float2 start;
	float2 finish;
};

struct lineC : line
{
	int r;
	int g;
	int b;
};

struct lineOfIDs
{
	int startID;
	int finishID;
};

struct circle
{
	int2 coordinates;
	int radius;
};

struct Node
{
	int ID;
	float criteria;

	Node()
	{

	}

	Node( int a, float b )
	{
		this->ID = a;
		this->criteria = b;
	}

};

struct Wall : DrawableF
{
	int z;
};


/*

what do i want to have after init:

1) map of id -> room + corridors
2) graph structure
3) index of main room
4) indexes of big rooms
5) indexes of spawn rooms
6) indexes of event rooms


*/


class Dungeon
{
public:
	Dungeon();
	~Dungeon();
	
	vector<SDL_FRect> generateCorridorv2(SDL_FRect a, SDL_FRect b);
	vector<SDL_FRect> generateCorridorv3(int id1, int id2);

	SDL_FRect makeHorizontalCorridor(SDL_FRect a, SDL_FRect b);
	SDL_FRect makeVerticalCorridor(SDL_FRect a, SDL_FRect b);

	bool makeCorridor(SDL_FRect a, SDL_FRect b, SDL_FRect& corridor);
	vector<SDL_FRect> makeCorridorv2(SDL_FRect a, SDL_FRect b);

	void init();
	void update();
	void draw();

	void move(int2 move);

private:
	
	SDL_Texture* m_defaultRoomTexture;
	SDL_Texture* m_placedRoomTexture;
	SDL_Texture* m_mainRoomTexture;
	SDL_Texture* m_hubRoomTexture;
	SDL_Texture* m_spawnRoomTexture;
	SDL_Texture* m_mediatorRoomTexture;
	SDL_Texture* m_mediatorRoomTextureIn;
	SDL_Texture* m_mediatorRoomTextureOut;
	SDL_Texture* m_mediatorRoomTextureMid;
	SDL_Texture* m_eventRoomTexture;
	SDL_Texture* m_corridorRoomTexture;
	SDL_Texture* m_tileTexture;
	SDL_Texture* m_wallTexture;
	SDL_Texture* m_wallSideTexture;

	Drawable m_background;

	map<int, Room> m_rooms;
	map<int,Corridor> m_corridors;
	vector<int> m_spawnRoomIDs;
	vector<int> m_hubRoomIDs;
	vector<int> m_eventRoomIDs;
	vector<int> m_mediatorRoomIDs;

	map<int, map<int, Room*>> m_graph;

	vector<DrawableF> m_tiles;
	vector<Wall> m_walls;

	vector<line> graphLines;
	vector<line> sectorLines;
	vector<circle> sectorCircles;
	vector<SDL_FRect> m_delunayLines;
	vector<circle> nodes;
	vector<lineC> connections;

	int mainRoomID;

	void spawnRooms();		///spawns Rooms and clubs them together in a circle
	void stirRooms();		///stirs Rooms so that they do not collide
	void assignRooms(int& outerZone, int& midZone);
	void delunayTriangulation();
	void delunayTriangulationv2();
	void deleteRooms();
	void scale(float p);
	void castToTile();
	void generateTextures();

	void drawForContent();
	void drawForDebug();

	vector<line> generateCorridor(SDL_FRect start, SDL_FRect end);

	int random();
	int2 roundToTile(float2 point);
	float2 getRandomPointInCircle(int radius, int2 center);

	SDL_Rect castToRect(SDL_FRect rect);
	float2 centerRect(SDL_FRect rect);
	int2 centerRect(SDL_Rect rect);
	SDL_Rect wrap(SDL_FRect rect);
	SDL_FRect wrapF(SDL_FRect rect);
	bool collLineLine(line a, line b);
	bool collRectRect(SDL_Rect a, SDL_Rect b);
	bool collRectRectF(SDL_FRect a, SDL_FRect b);
	bool collRectRectF_NOTPERFECT(SDL_FRect a, SDL_FRect b);
	bool collLineAndSDL_FRect(line mainLine, SDL_FRect a);
	float returnAngleByCoordinates(int2 direction);
	float2 returnCoordinatesByAngle(float angle);

	float distance(float2 coordA, float2 coordB);
	float pitagoreanTheorem(float a, float b);
	float fix(float a);

	vector<int> getRandomIndexes(int start, int finish, int amount);
};