#version 300 es

layout(std140) uniform Data {
    highp mat3 perspective;
    highp vec2 resolution;
};

uniform highp vec4 color;
uniform highp vec2 cutoff;

in highp vec4 container;

out highp vec4 FragColor;

void main(){
    highp vec2 g_pos = vec2(gl_FragCoord.x, -gl_FragCoord.y + resolution.y);
    highp vec2 pos = g_pos - container.xy;

    //highp float fx = pos.x * resolution.x / container.z;
    //highp float fy = pos.y * resolution.y / container.w;


    highp float c = 5.0; //spread
    highp float l = 3.5; //lightness
    
    highp vec2 t = (-abs(pos.xy - (container.zw / 2.0)) + container.zw / 2.0 )/c;
    highp vec2 q = clamp(floor(t), 0.0, 1.0);
    highp vec2 b = clamp(floor((pos.xy - container.zw / 2.0) / c) + 1.0, 0.0, 1.0);
    highp vec2 z1 = t * b - (1.0 - b) * (t - 2.0);
    highp vec2 f = z1 * (1.0 - q) + q;

    highp float a = f.x * f.y;

    highp float yClip = (sign(g_pos.y - cutoff[0]) + 1.0) * (-sign(g_pos.y - cutoff[1]) + 1.0);

    FragColor = vec4(color.xyz * (a / l + (l - 1.0) / l), color.w * yClip/2.0);
}