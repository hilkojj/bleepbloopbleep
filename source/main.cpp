
#include <gu/game_utils.h>
#include "level_screen.cpp"

int main()
{
    gu::Config config;
    config.width = 1600;
    config.height = 900;
    config.title = "My game";
    config.showFPSInTitleBar = true; // note: this option will hide the default title.
    config.vsync = false;
    config.printOpenGLMessages = false;
    if (!gu::init(config))
        return -1;

    std::cout << "Running game with OpenGL version: " << glGetString(GL_VERSION) << "\n";

    LevelScreen scr;

    gu::setScreen(&scr);

    gu::run();

    std::cout << sizeof(Chunk) << '\n';
    
    return 0;
}
