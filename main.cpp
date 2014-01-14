#include "Artemis\Artemis.h"
#include <Windows.h>
#include <stdio.h>

#include "Game.h"
#include "Global.h"


void fileReading()
{	
	//
	system("PAUSE");
}

void game()
{
	Game* g = new Game();
	g->Start();
}

int main(int argc, char **argv)
{
	game();
	return 0;
}