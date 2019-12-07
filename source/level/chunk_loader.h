#ifndef GAME_CHUNK_LOADER_H
#define GAME_CHUNK_LOADER_H

class ChunkManager;

class ChunkLoader
{

    friend ChunkManager;

    ivec2 prevChunkLocation;
    int prevRange;

    bool initialized = false;
    bool toBeRemoved = false;

  public:

    vec3 pos;
    u_int8_t range;

    void stop()
    {
        toBeRemoved = true;
    }
};

#endif
