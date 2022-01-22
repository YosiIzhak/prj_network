#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"
#include "client_app.h"


int resFromMenu = 0;

int main()
{	
	resFromMenu = MainMenu();
	RunMainMenu(resFromMenu);       	
}
