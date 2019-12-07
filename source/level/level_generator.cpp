
#include <utils/math_utils.h>
#include "level_generator.h"

ChunkColumn *LevelGenerator::generateColumn(ivec2 &loc)
{
    std::cout << "generating column " << to_string(loc) << "\n";

    auto col = new ChunkColumn(loc);
    for (int y = 0; y < ChunkColumn::NR_OF_CHUNKS; y++)
        generateChunk(col, y);
    return col;
}


Chunk *LevelGenerator::generateChunk(ChunkColumn *col, int y)
{
    Chunk *chunk = new Chunk(ivec3(col->levelLocation.x, y, col->levelLocation.y));
    col->chunks[y] = chunk;

    mu::loop3d(Chunk::SIZE, [&](int x, int y, int z) {

        auto worldPos = ivec3(x, y, z);
        chunk->toWorldPosition(worldPos);

        chunk->bitmap[x][y][z] = noise.GetSimplex(worldPos.x * 2, worldPos.y * 2, worldPos.z * 2) > .5;
        return true;
    });
    return chunk;
}
