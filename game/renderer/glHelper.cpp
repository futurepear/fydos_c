#include <iostream>
#include <string>


#include <fstream>
#include <sstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glhelper.h"
#include "util/glmath.h"
#include <cmath>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

void RenderStateObject::createProgram(const char* name, std::string vertFile, std::string fragFile) {
    programs[name] = createShaderProgram(vertFile, fragFile);
}


void RenderStateObject::addBasicGeometry(int start, int length) {
    basicGeometry.push_back(BasicGeometryIndex{ start, length });
};

void RenderStateObject::shader(const char* shader) {
    activeShader = shader;
    activeProgram = programs[shader];
    glUseProgram(activeProgram);
}

void RenderStateObject::cropTexture(float x, float y, float width, float height) {
    glm::mat3 textureMatrix{};
    textureMatrix.set(x / width, y / height, 0, width / activeTexture->width, height / activeTexture->height);

    glUniformMatrix3fv(getUniformLocation("textureMatrix"), 1, GL_TRUE, textureMatrix.ptr());
}

void RenderStateObject::setTexture(const char* name) {
    glBindTexture(GL_TEXTURE_2D, textures[name].tex);
    activeTexture = &textures[name];
}

GLuint RenderStateObject::getUniformLocation(const char* uniform) {
    return glGetUniformLocation(activeProgram, uniform);
}

void RenderStateObject::setTransform(glm::mat3& transformMatrix) {
    glUniformMatrix3fv(getUniformLocation("transform"), 1, GL_TRUE, transformMatrix.ptr());
}


void loadTexture(RenderStateObject& RenderState, const char* location, const char* name) {
    int width, height, nrchannels;
    unsigned char* data = stbi_load(location, &width, &height, &nrchannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load image: " << stbi_failure_reason() << "\n";
    }

    RenderState.textures[name] = Texture{ width, height, texture };

    stbi_image_free(data);
}



GLuint positionVAO(float vertices[], int size) {
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    return VAO;
}

void createPositionBuffers(AttributeBuffer& buf, float vertices[], int size) {
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    buf.position = VAO;
    buf.VBO = VBO;
}

//TODO: find way to reduce all this repetitive buffer making
GLuint GUIElementsVAO(float vertices[], int size, unsigned int indices[], int sizeI) {
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeI, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return VAO;
}

GLuint lightingLayerVAO(float vertices[], int size, unsigned int indices[], int sizeI) {
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeI, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //colors
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return VAO;
}

void dynamicVBO(AttributeBuffer& buf, int size) {
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size * 2 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    buf.position = VAO;
    buf.VBO = VBO;
    buf.size = size;
}

GLuint colorsVAO(float vertices[], int size) {
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    return VAO;
}

GLuint initPositionBuffer(float vertices[], int size) {
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    return VBO;
}

GLuint loadShader(std::string file, GLenum type) {
    const char* source = file.c_str();
    unsigned int vertexShader;
    vertexShader = glCreateShader(type);
    glShaderSource(vertexShader, 1, &source, NULL);
    glCompileShader(vertexShader);
    //check
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "SHADER error lol L   " << type << "  " << infoLog << "  " << file << "-----------------------------------------------------------\n";
    }
    return vertexShader;
}

std::string readFile(std::string path) {
    std::ifstream fileSource;
    std::string fileContent;
    fileSource.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        fileSource.open(path);
        std::stringstream fileStream;
        fileStream << fileSource.rdbuf();
        fileSource.close();
        fileContent = fileStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    return fileContent;
}

GLuint initShaderProgram(GLuint& vertexShader, GLuint& fragmentShader) {
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    return shaderProgram;
}

GLuint createShaderProgram(std::string vertFile, std::string fragFile) {
    auto vertShader = loadShader(readFile(vertFile), GL_VERTEX_SHADER);
    auto fragShader = loadShader(readFile(fragFile), GL_FRAGMENT_SHADER);

    return initShaderProgram(vertShader, fragShader);
}

void setPositionAttribute(AttributeBuffer& buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer.position);
    unsigned int VAO; //vertex buffer object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //glsetpositionattribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}
void setColorAttribute(AttributeBuffer& buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer.color);
    unsigned int VAO; //vertex buffer object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //glsetpositionattribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
}