#ifndef GAME_CHUNK_H
#define GAME_CHUNK_H

#include <graphics/3d/mesh.h>
#include <bitset>
#include <graphics/texture.h>

class Chunk
{
  public:
    static const int SIZE = 16 - 1; // -1 because the neighbours are stored as well in the textures

    Chunk(ivec3 levelLocation) : levelLocation(levelLocation) {}

    const ivec3 levelLocation;

    std::bitset<SIZE> bitmap[SIZE][SIZE];

    SharedMesh mesh;

    Chunk
        *xPosNeighbour = NULL,
        *xNegNeighbour = NULL,
        *yPosNeighbour = NULL,
        *yNegNeighbour = NULL,
        *zPosNeighbour = NULL,
        *zNegNeighbour = NULL;

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
    static const int NR_OF_CHUNKS = 16;

    const ivec2 levelLocation;

    Chunk *get(int y) { return chunks[y]; }

    SharedTexture texture;

    void generateTexture();

    static SharedMesh meshForGeometryShader();

  private:
    int loadedBy = 0;

    Chunk *chunks[NR_OF_CHUNKS] = { NULL };

    ChunkColumn(ivec2 loc) : levelLocation(loc) {}

    friend ChunkManager;
    friend LevelGenerator;
};

#endif
