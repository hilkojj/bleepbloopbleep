#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include "chunk.h"
#include "chunk_manager.h"

class Level
{
    double time = 0;
  public:

    const int seed;

    Level(int seed=123);

    ChunkManager chunks;

    void update(float deltaTime);


};

#endif
