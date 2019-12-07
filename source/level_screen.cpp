
#include <gu/screen.h>
#include <utils/math_utils.h>
#include <graphics/3d/debug_line_renderer.h>
#include <graphics/3d/perspective_camera.h>
#include <utils/camera/flying_camera_controller.h>
#include <input/mouse_input.h>
#include <gu/game_utils.h>
#include <FastNoise.h>
#include <graphics/frame_buffer.h>
#include <utils/quad_renderer.h>
#include "level/level.h"

class LevelScreen : public Screen
{
    double time = 0;

    DebugLineRenderer lineRenderer;
    PerspectiveCamera cam;
    FlyingCameraController camController;

    ShaderProgram terrainShader;

    Level level;

    FrameBuffer *sceneBuffer = NULL;

    ChunkLoader *loader = NULL;

  public:

    LevelScreen()
            :
            cam(.1, 1000, 1, 1, 55),
            camController(&cam),
            terrainShader(ShaderProgram::fromFiles("terrainShader", "assets/shaders/test.vert", "assets/shaders/normaltest.frag"))
    {
        MouseInput::setLockedMode(false);

        cam.position = vec3(6);
        cam.lookAt(mu::ZERO_3);
        camController.speedMultiplier = 10;

        loader = level.chunks.createLoader();
        loader->range = 10;

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void render(double deltaTime) override
    {
        time += deltaTime;

        camController.update(deltaTime);
        loader->pos = cam.position;

        level.update(deltaTime);

        MouseInput::setLockedMode(true);

        lineRenderer.projection = cam.combined;

        const static vec3 p0 = vec3(1.5, 0, 1), p1 = vec3(-2, 1, 1), p2 = vec3(1, 1, -1);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sceneBuffer->bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(.1, .15, .29, 1);

        lineRenderer.line(glm::vec3(-300, 0, 0), glm::vec3(300, 0, 0), mu::X);
        lineRenderer.line(glm::vec3(0, -300, 0), glm::vec3(0, 300, 0), mu::Y);
        lineRenderer.line(glm::vec3(0, 0, -300), glm::vec3(0, 0, 300), mu::Z);

        lineRenderer.line(p0, p1, mu::X);
        lineRenderer.line(p1, p2, mu::X);
        lineRenderer.line(p2, p0, mu::X);

        vec3 rayDir = cam.getRayDirection(gu::width * .5, gu::height * .5);;



        float yDiff = 0 - cam.position.y;

        float factor = yDiff / rayDir.y;

        vec3 line3 = cam.position + rayDir * factor;
        static vec2 line;
        if (MouseInput::pressed(GLFW_MOUSE_BUTTON_LEFT))
            line = vec2(line3.x, line3.z);

        vec3 bc;
        mu::barycentricCoords(line, vec2(p0.x, p0.z), vec2(p1.x, p1.z), vec2(p2.x, p2.z), bc);

        lineRenderer.line(vec3(line.x, 0, line.y), vec3(line.x, 1, line.y), mu::Y);

        if (mu::allGreaterOrEqualTo(bc, 0))
        {
//            std::cout << to_string(bc) << "\n";
            float intersectY = bc.x * p0.y + bc.y * p1.y + bc.z * p2.y;

            lineRenderer.line(vec3(line.x, intersectY, line.y), p0, mu::X);
            lineRenderer.axes(vec3(line.x, intersectY, line.y), .2, mu::Z);
            lineRenderer.line(vec3(line.x, intersectY, line.y), p1, mu::X);
            lineRenderer.line(vec3(line.x, intersectY, line.y), p2, mu::X);
        }
        terrainShader.use();
        glUniformMatrix4fv(terrainShader.location("MVP"), 1, GL_FALSE, &cam.combined[0][0]);

        for (auto &c : level.chunks.getLoadedColumns())
            for (int y = 0; y < ChunkColumn::NR_OF_CHUNKS; y++)
            {

                c->get(y)->mesh->renderArrays();
//                lineRenderer.line(
//                        vec3(c->levelLocation.x * Chunk::SIZE, 0, c->levelLocation.y * Chunk::SIZE),
//                        vec3(c->levelLocation.x * Chunk::SIZE, ChunkColumn::NR_OF_CHUNKS * Chunk::SIZE, c->levelLocation.y * Chunk::SIZE),
//                        mu::X
//                );
//                lineRenderer.line(
//                        vec3((c->levelLocation.x + 1) * Chunk::SIZE, 0, c->levelLocation.y * Chunk::SIZE),
//                        vec3((c->levelLocation.x + 1) * Chunk::SIZE, ChunkColumn::NR_OF_CHUNKS * Chunk::SIZE, c->levelLocation.y * Chunk::SIZE),
//                        mu::X
//                );
//                lineRenderer.line(
//                        vec3((c->levelLocation.x + 1) * Chunk::SIZE, 0, (c->levelLocation.y + 1) * Chunk::SIZE),
//                        vec3((c->levelLocation.x + 1) * Chunk::SIZE, ChunkColumn::NR_OF_CHUNKS * Chunk::SIZE, (c->levelLocation.y + 1) * Chunk::SIZE),
//                        mu::X
//                );
//                lineRenderer.line(
//                        vec3(c->levelLocation.x * Chunk::SIZE, 0, (c->levelLocation.y + 1) * Chunk::SIZE),
//                        vec3(c->levelLocation.x * Chunk::SIZE, ChunkColumn::NR_OF_CHUNKS * Chunk::SIZE, (c->levelLocation.y + 1) * Chunk::SIZE),
//                        mu::X
//                );
            }

        sceneBuffer->unbind();

        QuadRenderer::render(sceneBuffer->colorTexture);
    }

    void onResize() override
    {
        cam.viewportWidth = gu::widthPixels;
        cam.viewportHeight = gu::heightPixels;

        delete sceneBuffer;
        sceneBuffer = new FrameBuffer(gu::widthPixels / 4, gu::heightPixels / 4, 0);
        sceneBuffer->addColorTexture(GL_RGB, GL_NEAREST, GL_NEAREST);
        sceneBuffer->addDepthTexture(GL_NEAREST, GL_NEAREST);
    }
};
