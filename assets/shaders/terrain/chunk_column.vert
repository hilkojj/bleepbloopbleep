
#version 430 core
layout(location = 0) in vec2 a_pos;

uniform mat4 MVP;

out VS_OUT {
    ivec2 pos;
} vs_out;

void main()
{
    gl_Position = MVP * vec4(vec3(a_pos, 0), 1);

    vs_out.pos = ivec2(a_pos.x, a_pos.y);
}


