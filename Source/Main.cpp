#include "Engine.h"
using namespace std;

int main(int argc, char* args[]) {
	Engine engine = Engine(args[0], glm::vec2(360, 360));
	engine.Run();
	return 0;
}