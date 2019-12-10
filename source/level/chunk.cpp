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

    mesh = SharedMesh(new Mesh(meshName, 0, 0, VertAttributes().add_(VertAttributes::POSITION)));//.add_(VertAttributes::NORMAL)));

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
//                normal.x, normal.y, normal.z,

                p2.x, p2.y, p2.z,
//                normal.x, normal.y, normal.z,

                p3.x, p3.y, p3.z,
//                normal.x, normal.y, normal.z
            });
            mesh->nrOfVertices += 3;
        }
        return true;
    });
    VertBuffer::uploadSingleMesh(mesh);
    mesh->disposeOfflineData();
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

void ChunkColumn::generateTexture()
{
    typedef uint16 pixel;

    const static int

        BLOCKS_PER_PIXEL = sizeof(pixel) * 8,                               // = 16

        PIXELS_PER_CHUNK = pow(Chunk::SIZE + 1, 3) / BLOCKS_PER_PIXEL,  // = 16 * 16 = 256

        NR_OF_PIXELS = PIXELS_PER_CHUNK * NR_OF_CHUNKS,           // = 256 * 16 = 64 * 64

        WIDTH = sqrt(NR_OF_PIXELS);                                 // = 64

    assert(BLOCKS_PER_PIXEL == 16);
    assert(PIXELS_PER_CHUNK == 256);
    assert(NR_OF_PIXELS == 64 * 64);
    assert(WIDTH * WIDTH == NR_OF_PIXELS);

    pixel pixels[NR_OF_PIXELS];

    uint pixelIndex = 0, bitIndex =  0;

    for (auto chunk : chunks)
    {
        mu::loop3d(Chunk::SIZE + 1, [&](int z, int x, int y) {

//            std::cout << "x" << x << ", y" << y << ", z" << z << " dataI " << pixelIndex << " bitIndex " << bitIndex << '\n';

            if (bitIndex == 0)
                pixels[pixelIndex] = 0;

            auto currentChunk = chunk;
            if (x == Chunk::SIZE)
                currentChunk = chunk->xPosNeighbour;
            if (y == Chunk::SIZE)
                currentChunk = chunk->yPosNeighbour;
            if (z == Chunk::SIZE)
                currentChunk = chunk->zPosNeighbour;

            if (currentChunk && currentChunk->bitmap[x][y][z])
                pixels[pixelIndex] |= pixel(1u << bitIndex);

//            std::cout << std::bitset<16>(pixels[pixelIndex]) << "\n";

            if (++bitIndex == BLOCKS_PER_PIXEL)
            {
                pixelIndex++;

                bitIndex = 0;
            }
            return true;
        });
    }

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, WIDTH, WIDTH, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    texture = SharedTexture(new Texture(texId, WIDTH, WIDTH));
}

SharedMesh ChunkColumn::meshForGeometryShader()
{
    static SharedMesh mesh;

    if (mesh) return mesh;

    mesh = SharedMesh(new Mesh("chunk_column", Chunk::SIZE * Chunk::SIZE * NR_OF_CHUNKS, 0, VertAttributes().add_({"POS", 2})));

    int i = 0;
    for (int y = 0; y < Chunk::SIZE * NR_OF_CHUNKS; y++)
        for (int x = 0; x < Chunk::SIZE; x++)
            mesh->set(vec2(x, y), i++, 0);

    mesh->mode = GL_POINTS;
    VertBuffer::uploadSingleMesh(mesh);
    mesh->disposeOfflineData();
    return mesh;
}
