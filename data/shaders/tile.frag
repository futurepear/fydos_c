#version 300 es

layout(std140) uniform Data {
    highp mat3 perspective;
    highp vec2 resolution;
};

uniform highp vec4 color;

out highp vec4 FragColor;

void main(){
    FragColor = color;
    // vec4(gl_FragCoord.xy/resolution, 0.0, 1.0);
}