
#version 430 core
layout (points) in;
layout (points, max_vertices = 300) out;

in VS_OUT {
    ivec2 pos;
} gs_in[];

out vec3 colorrr;

uniform mat4 MVP;
uniform usampler2D u_bitmap;

const int BLOCKS_PER_PIXEL = 16, TEXTURE_SIZE = 64;

void main()
{

    for (uint z = 0; z < BLOCKS_PER_PIXEL - 1; z++)
    {
        ivec2 pos2d = gs_in[0].pos;

        int pixelIndex = pos2d.x + (pos2d.y + pos2d.y / 16) * BLOCKS_PER_PIXEL;
        uint bitIndex = z;
        ivec2 pixelPos = ivec2(pixelIndex % TEXTURE_SIZE, pixelIndex / TEXTURE_SIZE);

        uint pixel = texelFetch(u_bitmap, pixelPos, 0).r;

        colorrr = vec3(1);

        if ((pixel & 1u << bitIndex) != 0)
        {

            vec3 pos = vec3(pos2d, z);

            vec4 projectedPos = MVP * vec4(pos, 1);

            gl_Position = projectedPos;
            EmitVertex();
            EndPrimitive();
        }


    }

}
