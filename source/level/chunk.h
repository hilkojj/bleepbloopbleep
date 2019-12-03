#ifndef GAME_CHUNK_H
#define GAME_CHUNK_H


#include <graphics/3d/mesh.h>

class Chunk
{

  public:
    static const int SIZE = 200;

    bool bitmap[SIZE][SIZE][SIZE] = { false };

    SharedMesh mesh;

    void generateMesh();

    int getCubeIndex(int x, int y, int z);

};


#endif
