
#include <gu/screen.h>
#include <math.h>
#include <utils/math_utils.h>
#include <graphics/3d/debug_line_renderer.h>
#include <graphics/3d/perspective_camera.h>
#include <utils/camera/flying_camera_controller.h>

class LevelScreen : public Screen
{
    double time = 0;

    DebugLineRenderer lineRenderer;
    PerspectiveCamera cam;
    FlyingCameraController camController;

  public:

    LevelScreen()
            :
            cam(.1, 1000, 1, 1, 55),
            camController(&cam)
    {
        cam.position = vec3(6);
        cam.lookAt(mu::ZERO_3);
        camController.speedMultiplier = 10;
    }

    void render(double deltaTime) override
    {
        time += deltaTime;

        camController.update(deltaTime);

        lineRenderer.projection = cam.combined;

        const static vec3 p0 = vec3(1.5, 0, 1), p1 = vec3(-2, 1, 1), p2 = vec3(1, 1, -1);

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
            std::cout << to_string(bc) << "\n";
            float intersectY = bc.x * p0.y + bc.y * p1.y + bc.z * p2.y;

            lineRenderer.line(vec3(line.x, intersectY, line.y), p0, mu::X);
            lineRenderer.axes(vec3(line.x, intersectY, line.y), .2, mu::Z);
            lineRenderer.line(vec3(line.x, intersectY, line.y), p1, mu::X);
            lineRenderer.line(vec3(line.x, intersectY, line.y), p2, mu::X);
        }

    }

    void onResize() override
    {
        cam.viewportWidth = gu::widthPixels;
        cam.viewportHeight = gu::heightPixels;
    }
};
