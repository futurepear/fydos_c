#version 300 es
layout(location = 0) in vec2 aPos;

layout(std140) uniform Data {
    mat3 perspective;
    vec2 resolution;
};

uniform vec4 color;
uniform mat3 transform;
//out vec4 vertexColor;


void main(){
    float chunkSize = 15.0;

   vec2 position = (perspective * transform * vec3(aPos, 1.0)).xy;

    position /= resolution;
    position *= 2.0;
    position -= 1.0;


   //highp vec2 correctedPos = vec2(aPos.x - mod(aTexture.x, chunkSize), aPos.y - floor(aTexture.x / chunkSize));
   // textureCoord = (textureMatrix * vec3((vec2(correctedPos) + vec2(1.0, 1.0)) / 2.0, 1.0)).xy;
    // textureCoord.x += 0.02441;  

    gl_Position = vec4(position * vec2(1, -1), 0.0, 1.0);
    //vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}