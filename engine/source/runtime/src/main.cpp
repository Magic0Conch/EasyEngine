#include "Engine.h"

int main(int argc,char** argv) {
	EasyEngine::Engine engine(argv[0]);
	engine.start();
	return 0;
}