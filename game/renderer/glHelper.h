#pragma once
#include <unordered_map>
#include <string>
#include "util/glmath.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 


GLuint createShaderProgram(std::string vertFile, std::string fragFile);

struct AttributeBuffer {
    unsigned int position = 0;
    unsigned int color = 0;
    unsigned int texture = 0;
    unsigned int VBO = 0;
    unsigned int GUIElements = 0;
    int size = 0;
    int indexSize = 0;
};

struct RenderSettings {
    bool MSAA = true;
};

struct Texture {
    int width{};
    int height{};
    GLuint tex{};
};

typedef struct {
    GLuint count;
    GLuint instanceCount;
    GLuint first;
    GLuint baseInstance;
} DrawArraysIndirectCommand;


struct Point {
    int x;
    int y;
};

struct BasicGeometryIndex {
    int start;
    int length;
};

class RenderStateObject {
public:
    RenderStateObject() {
        activeTexture = nullptr;
        activeProgram = 0;
    }
    std::unordered_map<const char*, GLuint> programs;
    std::unordered_map<std::string, AttributeBuffer> geometry;
    std::unordered_map<const char*, Texture> textures;
    float cameraX = 0;
    float cameraY = 0;
    int width = 800;
    int height = 600;
    bool resolutionChange = true;
    unsigned int uniformData = 0;
    RenderSettings settings;
    std::string activeShader;
    std::vector<BasicGeometryIndex> basicGeometry{};

    void addBasicGeometry(int start, int length);

    //set active shader program with a string identifier 
    void shader(const char* shader);

    //create a new program from the vertex and fragment shader file path along with a string identifier
    void createProgram(const char* name, std::string vertFile, std::string fragFile);

    //set texture
    void setTexture(const char* name);
    GLuint getUniformLocation(const char* uniform);
    void setTransform(glm::mat3& transformMatrix);

    //crop a texture (with pixels) 
    //sets textureMatrix uniform 
    void cropTexture(float x, float y, float width, float height);

private:
    GLuint activeProgram;
    Texture* activeTexture;
};

//Fill an attributebuffer with a VAO and VBO that can be dynamically changed with glBufferSubData
void dynamicVBO(AttributeBuffer& buf, int size);

//load a texture and save it in the RenderState object 
void loadTexture(RenderStateObject& RenderState, const char* location, const char* name);


GLuint initPositionBuffer(float vertices[], int size);

//loads and compiles a shader from a file 
GLuint loadShader(std::string file, GLenum type);

//creates a program from the locations given by loadShader()
GLuint initShaderProgram(GLuint& vertexShader, GLuint& fragmentShader);

//reads a file
std::string readFile(std::string path);

//generate VAO for positions or colors
//TODO: combine position/textures/color etc..
GLuint positionVAO(float vertices[], int size);
GLuint colorsVAO(float vertices[], int size);
GLuint GUIElementsVAO(float vertices[], int size, unsigned int indices[], int sizeI);
void createPositionBuffers(AttributeBuffer& buf, float vertices[], int size);

//use only for lighting layers
GLuint lightingLayerVAO(float vertices[], int size, unsigned int indices[], int sizeI);

//dont use
void setPositionAttribute(AttributeBuffer& buffer);
void setColorAttribute(AttributeBuffer& buffer);