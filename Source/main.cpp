#include <iostream>

#include "Engine.h"
#include "gameFuncs.h"

int main( int argc, char** argv ){
  Engine* game_engine = Engine::getInstance();

  while( !game_engine->shouldQuit() ){
    game_engine->idle();
  }

  delete game_engine;

  std::cin.get();

  return 0;
}
