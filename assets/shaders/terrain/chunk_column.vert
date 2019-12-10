
#version 430 core
layout(location = 0) in vec2 a_pos;

out VS_OUT {
    ivec2 pos;
} vs_out;

void main()
{
    vs_out.pos = ivec2(a_pos.x, a_pos.y);
}


