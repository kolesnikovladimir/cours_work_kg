#include "../libwinbgi/src/graphics.h"
#include <stdio.h>

#include <iostream>
#include "controller.h"

#include "Dot.h"
#include "mPolygon.h"
#include "prisma.h"

int main()
{

	Controller* scene = new Controller;

	scene->create_data_call_menu();

	return 0;
}
