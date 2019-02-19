#include "Engine.h"

int main(int argc, char* args[]) {
	Engine engine = Engine(args[0]);
	engine.Run();
	return 0;
}