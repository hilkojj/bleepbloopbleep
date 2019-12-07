#ifndef GAME_CHUNK_MANAGER_H
#define GAME_CHUNK_MANAGER_H

#include "chunk.h"
#include "chunk_loader.h"
#include "level_generator.h"

class Level;

class ChunkManager
{
    std::vector<ChunkColumn *> columns;
    std::vector<ChunkColumn *> loadedColumns;

    std::vector<ChunkLoader *> loaders;

    LevelGenerator generator;

    void update(float deltaTime);

    void load(ChunkColumn *col);
    void unload(ChunkColumn *col);

    ChunkManager(Level *level);

    friend Level;

  public:

    Chunk *get(const int &x, const int &y, const int &z) const;

    ChunkColumn *getColumn(const int &x, const int &z) const;

    ChunkLoader *createLoader();

    const std::vector<ChunkColumn *> &getLoadedColumns() const;

};


#endif
