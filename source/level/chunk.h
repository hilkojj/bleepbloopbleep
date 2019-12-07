#ifndef GAME_CHUNK_H
#define GAME_CHUNK_H

#include <graphics/3d/mesh.h>

class Chunk
{
  public:
    static const int SIZE = 32;

    Chunk(ivec3 levelLocation) : levelLocation(levelLocation) {}

    const ivec3 levelLocation;

    bool bitmap[SIZE][SIZE][SIZE] = { false };

    SharedMesh mesh;

    void generateMesh();

    void toWorldPosition(ivec3 &pos);

    void toLocalPosition(ivec3 &pos);

    int getTriTableIndex(int x, int y, int z) const;

};

class ChunkManager;
class LevelGenerator;

class ChunkColumn
{
  public:
    static const int NR_OF_CHUNKS = 8;

    const ivec2 levelLocation;

    Chunk *get(int y) { return chunks[y]; }

  private:
    int loadedBy = 0;

    Chunk *chunks[NR_OF_CHUNKS] = { NULL };

    ChunkColumn(ivec2 loc) : levelLocation(loc) {}

    friend ChunkManager;
    friend LevelGenerator;
};

#endif
