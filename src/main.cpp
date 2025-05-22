#include<iostream>

#include "World.h"

#undef main

using namespace std;


int main(int* argc, char* argv[])
{
	// Welcome to our Masterclass :) - Nite Games


	World world;
	
	world.init();

	while (world.run())
	{
		world.update();
		world.draw();
	}

	world.exit();

	return 0;
}