
#ifndef GAME_LEVEL_GENERATOR_H
#define GAME_LEVEL_GENERATOR_H

#include <FastNoise.h>
#include "chunk.h"

class LevelGenerator
{
    FastNoise noise;

  public:
    const int seed;

    LevelGenerator(int seed) : seed(seed) {}

    ChunkColumn *generateColumn(ivec2 &loc);
    Chunk *generateChunk(ChunkColumn *col, int y);

};


#endif
