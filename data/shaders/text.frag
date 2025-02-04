#version 330 core

layout(std140) uniform Data {
    mat3 perspective;
    vec2 resolution;
};

uniform vec4 color;
uniform vec2 cutoff;
uniform vec4 character; //x, y, width, height
uniform vec4 styles; //scale, edge, borderEdge, shadow

in vec4 container;

out vec4 FragColor;

uniform sampler2D fontTexture;

void main(){
    vec2 g_pos = vec2(gl_FragCoord.x, -gl_FragCoord.y + resolution.y);
    vec2 pos = (g_pos - container.xy);

    float yClip = (sign(g_pos.y - cutoff[0]) + 1.0) * (-sign(g_pos.y - cutoff[1]) + 1.0);

    vec4 font = texture(fontTexture, pos * character.zw / (container.zw * 512.0) + character.xy / 512.0);
    vec4 shadow = texture(fontTexture, (vec2(0.0, -4.0) + pos) * character.zw / (container.zw * 512.0) + character.xy / 512.0);

    float inner = clamp(2.0*smoothstep(0.35, 0.9, font[0]), 0.0, 1.0);//smoothstep(0.44, 0.54, font[0]);
    float outer = smoothstep(0.44, 0.44, font[0]);
    float border = 0;  //outer - inner;

    //FragColor = vec4(0.0, 0.0, 0.0, shadow[0]*0.5);
    FragColor = vec4(color.rgb + vec3(0.0, 0.0, 0.0) * border, (inner) * color.w * yClip / 2.0);// + (1.0 - inner) * vec4(0.0, 0.0, 0.0, shadow[0]*0.4);
}