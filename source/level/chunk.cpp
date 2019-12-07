#include <utils/math_utils.h>
#include <graphics/3d/vert_buffer.h>
#include "chunk.h"
#include "marching_cubes/lookup_table.h"

/**
 * Stolen from https://github.com/timostrating/TinyPokemonWorld/
 */
void Chunk::generateMesh()
{
    auto meshName = "chunk_" + to_string(levelLocation);

    mesh = SharedMesh(new Mesh(meshName, 0, 0, VertAttributes().add_(VertAttributes::POSITION).add_(VertAttributes::NORMAL)));

    mu::loop3d(SIZE - 1, [&](int x, int y, int z) {

        int tableIndex = getTriTableIndex(x, y, z);

        for (int i = 0; i < 15; i += 3)
        {
            if (TRI_TABLE[tableIndex][i] == -1) break;

            vec3 offset = vec3(SIZE) * vec3(levelLocation);

            vec3 p1 = vec3(x, y, z) + EDGE_POINTS[TRI_TABLE[tableIndex][i + 0]] + offset;
            vec3 p2 = vec3(x, y, z) + EDGE_POINTS[TRI_TABLE[tableIndex][i + 1]] + offset;
            vec3 p3 = vec3(x, y, z) + EDGE_POINTS[TRI_TABLE[tableIndex][i + 2]] + offset;

            vec3 normal = mu::calculateNormal(p1, p2, p3);

            mesh->vertices.insert(mesh->vertices.end(), {
                p1.x, p1.y, p1.z,
                normal.x, normal.y, normal.z,

                p2.x, p2.y, p2.z,
                normal.x, normal.y, normal.z,

                p3.x, p3.y, p3.z,
                normal.x, normal.y, normal.z
            });
            mesh->nrOfVertices += 3;
        }
        return true;
    });
    VertBuffer::uploadSingleMesh(mesh);
}

int Chunk::getTriTableIndex(int x, int y, int z) const
{
    int cubeIndex = 0;

    const static int CUBE[8 * 3]{
            0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1,
            0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1,
    };

    for (unsigned int i = 0; i < 8; i++)
    {
        if (bitmap[x + CUBE[i * 3 + 0]][y + CUBE[i * 3 + 1]][z + CUBE[i * 3 + 2]])
            cubeIndex += ((uint) 1 << i);
    }
    return cubeIndex;
}

void Chunk::toWorldPosition(ivec3 &pos)
{
    pos += levelLocation * SIZE;
}

void Chunk::toLocalPosition(ivec3 &pos)
{
    pos -= levelLocation * SIZE;
}
