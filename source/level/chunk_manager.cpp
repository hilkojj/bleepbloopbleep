#include <utils/math_utils.h>
#include "chunk_manager.h"
#include "level.h"

Chunk *ChunkManager::get(const int &x, const int &y, const int &z) const
{
    auto col = getColumn(x, z);
    return col ? col->chunks[y] : NULL;
}

ChunkColumn *ChunkManager::getColumn(const int &x, const int &z) const
{
    // todo: optimize
    for (auto c : columns) if (c->levelLocation.x == x && c->levelLocation.y == z) return c;
    return nullptr;
}

ChunkLoader *ChunkManager::createLoader()
{
    loaders.push_back(new ChunkLoader());
    return loaders.back();
}

ChunkManager::ChunkManager(Level *level) : generator(level->seed)
{
}

void ChunkManager::update(float deltaTime)
{
    for (int i = loaders.size() - 1; i >= 0; i--)
    {
        auto loader = loaders[i];
        if (loader->toBeRemoved)
        {
            loaders[i] = loaders.back();
            loaders.pop_back();
            delete loader;
            continue;
        }
        ivec2 loc(loader->pos.x / Chunk::SIZE, loader->pos.z / Chunk::SIZE);

        if (
                loader->initialized
                && loc == loader->prevChunkLocation
                && loader->prevRange == loader->range
            ) continue;

        // load chunks in range of the loader:
        mu::spiral(loader->range, [&](ivec2 pos){
            ivec2 cl = loc + pos;
            auto col = getColumn(cl.x, cl.y);
            if (!col)
            {
                col = generator.generateColumn(cl);
                columns.push_back(col);
            }
            if (col->loadedBy++ == 0)
                load(col);
            return true;
        });
        // unload chunks that were previously loaded by the loader:
        mu::spiral(loader->prevRange, [&](ivec2 pos){
            ivec2 cl = loader->prevChunkLocation + pos;
            auto col = getColumn(cl.x, cl.y);
            if (!col) return true;

            col->loadedBy--;
            return true;
        });
        loader->prevChunkLocation = loc;
        loader->prevRange = loader->range;
        loader->initialized = true;
    }

    for (int i = loadedColumns.size() - 1; i >= 0; i--)
    {
        auto col = loadedColumns[i];
        if (col->loadedBy == 0)
        {
            loadedColumns[i] = loadedColumns.back();
            loadedColumns.pop_back();
            unload(col);
            continue;
        }
    }
}

void ChunkManager::load(ChunkColumn *col)
{
    std::cout << "loading column " << to_string(col->levelLocation) << '\n';

    // todo: loading chunks shouldn't mean that a mesh needs to be created and rendered.
    for (auto c : col->chunks) c->generateMesh();
    loadedColumns.push_back(col);
}

void ChunkManager::unload(ChunkColumn *col)
{
    for (auto c : col->chunks) c->mesh = NULL;
    // todo: col is not removed from this.loadedColumns in this function.
}

const std::vector<ChunkColumn *> &ChunkManager::getLoadedColumns() const
{
    return loadedColumns;
}
