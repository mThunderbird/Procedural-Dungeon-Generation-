#pragma once

#include <iostream>
#include <string>

#include <map>
#include <vector>
#include <queue>
#include <cmath>

#include <assert.h>


#include "SDL.h"

using namespace std;


struct Drawable
{
	SDL_Rect rect;
	SDL_Texture* texture;
};

struct DrawableF
{
    SDL_FRect rect;
    SDL_Texture* texture;
};

struct int2
{
    int x = 0;
    int y = 0;

    friend ostream& operator<<(ostream& os, const int2& i2)
    {
        os << i2.x << " " << i2.y;
        return os;
    }

    void normalise()
    {
        int mag = sqrt(pow(x, 2) + pow(y, 2));
        if (mag == 0)
        {
            return;
        }
        x /= mag;
        y /= mag;
    }

    void reset()
    {
        x = 0;
        y = 0;
    }

    template <typename T>
    void operator*=(T a)
    {
        x *= a;
        y *= a;
    }

    template <typename T>
    void operator/=(T a)
    {
        x /= a;
        y /= a;
    }

    template <typename T>
    int2 operator*(T a)
    {
        x *= a;
        y *= a;

        return *this;
    }

    template <typename T>
    int2 operator/(T a)
    {
        x /= a;
        y /= a;

        return *this;
    }

    // used in the animator
    template <typename T>
    bool operator!=(T a)
    {
        return (x != a || y != a);
    }
};

struct float2
{
    float x = 0;
    float y = 0;

    friend ostream& operator<<(ostream& os, const float2& f2)
    {
        os << f2.x << " " << f2.y;
        return os;
    }

    void multiply(float a)
    {
        x *= a;
        y *= a;
    }

    void normalise()
    {
        if (x == 0.0f && y == 0.0f)
        {
            return;
        }
   
        float mag = sqrt(pow(x, 2) + pow(y, 2));
        x /= mag;
        y /= mag;
    }

    void reset()
    {
        x = 0;
        y = 0;
    }

    void operator=(int2 a)
    {
        x = a.x;
        y = a.y;
    }

    void operator+=(float2 a)
    {
        x += a.x;
        y += a.y;
    }

    void operator-=(float2 a)
    {
        x -= a.x;
        y -= a.y;
    }

    template <typename T>
    void operator*=(T a)
    {
        x *= a;
        y *= a;
    }

    template <typename T>
    void operator/=(T a)
    {
        x /= a;
        y /= a;
    }

    template <typename T>
    float2 operator*(T a)
    {
        x *= a;
        y *= a;

        return *this;
    }

    template <typename T>
    float2 operator/(T a)
    {
        x /= a;
        y /= a;

        return *this;
    }

    // used in the animator

    template <typename T>
    bool operator!=(T a)
    {
        return (x != a || y != a);
    }

    template <typename T>
    bool operator>(T a)
    {
        return (x > a || y > a);
    }

    template <typename T>
    bool operator<(T a)
    {
        return (x < a || y < a);
    }

    template <typename T>
    bool operator==(T a)
    {
        return (x == a && y == a);
    }

    operator int2() 
    {
        int2 res;
        res.x = this->x;
        res.y = this->y;
        return res;
    }

};

enum class FONT
{
    NONE = 0,
    ARCADE_CLASSIC = 1, // used in MonTu
    ADVENT_PRO = 2 // used in Raven
};

enum class COLOR
{
    NONE = 0,
    LIGHT = 1,
    DARK = 2
};


static int mp = 1;
static int SCALE = 8;

static int TILE_SIZE = 4 * mp;
static int WIDTH_MAX = 100 * mp;
static int WIDTH_MIN = 30 * mp;
static int HEIGTH_MAX = 100 * mp;
static int HEIGTH_MIN = 30 * mp;
const static int ROOM_NUMBER = 150;
static int DUNGEON_RADIUS = 100 * mp;
static float HUB_ROOM_SIZE = 0.8;
static float  ROOM_WRAP = 0 * mp;
static int CORRIDOR_WIDTH = 20 * mp;
static int NUMBER_OF_TEAMS = 10;
static int angleGap = 5;
static int sectorGap = 5;
static int2 DUNGEON_CENTER = { 960 ,540};

static int HUB_MAX_WIDTH = 90 * mp;
static int HUB_MAX_HEIGHT = 90 * mp;

static int HUB_MIN_WIDTH = 80 * mp;
static int HUB_MIN_HEIGHT = 80 * mp;

static int DEF_MAX_WIDTH = 60 * mp;
static int DEF_MAX_HEIGHT = 60 * mp;

static int DEF_MIN_WIDTH = 30 * mp;
static int DEF_MIN_HEIGHT = 30 * mp;

static int WALL_HEIGHT = 4 * mp;
static int WALL_WIDTH = 4 * mp;
