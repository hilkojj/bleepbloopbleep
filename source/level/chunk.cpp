//
// Created by kaasflip on 2-12-19.
//

#include <utils/math_utils.h>
#include <graphics/3d/vert_buffer.h>
#include "chunk.h"
#include "marching_cubes/lookup_table.h"

/**
 * Stolen from https://github.com/timostrating/TinyPokemonWorld/
 */
void Chunk::generateMesh()
{

    mesh = SharedMesh(new Mesh("chunk_mesh", 0, 0, VertAttributes().add_(VertAttributes::POSITION).add_(VertAttributes::NORMAL)));

    mu::loop3d(SIZE - 1, [&](int x, int y, int z) {

        int cubeIndex = getCubeIndex(x, y, z);

        for (int i = 0; i < 15; i += 3)
        {
            if (TRI_TABLE[cubeIndex][i] == -1) break;

            vec3 p1 = vec3(x, y, z) + EDGE_POINTS[TRI_TABLE[cubeIndex][i + 0]];
            vec3 p2 = vec3(x, y, z) + EDGE_POINTS[TRI_TABLE[cubeIndex][i + 1]];
            vec3 p3 = vec3(x, y, z) + EDGE_POINTS[TRI_TABLE[cubeIndex][i + 2]];

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

int Chunk::getCubeIndex(int x, int y, int z)
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
