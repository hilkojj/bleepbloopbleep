#include "level.h"

void Level::update(float deltaTime)
{
    time += deltaTime;

    chunks.update(deltaTime);

}

Level::Level(int seed)
        : chunks(this), seed(seed)
{

}
