//
// Created by Engin Manap on 10.02.2016.
//

#ifndef UBERGAME_GLHELPER_H
#define UBERGAME_GLHELPER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GL/glew.h>
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif/*__APPLE__*/

#include <string>
#include <algorithm>
#include <vector>

#include <fstream>
#include <streambuf>
#include <iostream>

class GLHelper {
    GLenum error;

    float aspect;
    std::vector<GLuint> bufferObjects;
    std::vector<GLuint> vertexArrays;

    glm::mat4 cameraMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 orthogonalProjectionMatrix;
    bool checkErrors(std::string callerFunc);
    GLuint createShader(GLenum, const std::string &);
    GLuint createProgram(const std::vector<GLuint> &);
    GLuint generateBuffer(const GLuint number);
    bool deleteBuffer(const GLuint number, const GLuint bufferID);
    GLuint generateVAO(const GLuint number);
    bool deleteVAO(const GLuint number, const GLuint bufferID);
public:
    GLHelper();
    ~GLHelper();

    GLuint initializeProgram(std::string vertexShaderFile, std::string fragmentShaderFile);

    void bufferVertexData(const std::vector<glm::vec3>& vertices,
                          const std::vector<glm::mediump_uvec3>& faces,
                          GLuint& vao, GLuint& vbo, const GLuint attachPointer, GLuint& ebo);
    void bufferVertexColor(const std::vector<glm::vec4>& colors,
                           GLuint& vao, GLuint& vbo, const GLuint attachPointer);
    void bufferVertexTextureCoordinates(const std::vector<glm::vec2> textureCoordinates,
                          GLuint& vao, GLuint& vbo, const GLuint attachPointer, GLuint& ebo);
    bool freeBuffer(const GLuint bufferID);
    bool freeVAO(const GLuint VAO);
    void setCamera(const glm::mat4&);

    void clearFrame(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void render(const GLuint, const GLuint, const GLuint, const GLuint);
    void reshape(int height, int width);
    GLuint loadTexture(int height, int width, GLenum format, void *data);
    GLuint loadCubeMap(int height, int width, void* right, void* left, void* top, void* bottom, void* back, void* front);
    void attachTexture(GLuint textureID);
    void attachCubeMap(GLuint cubeMapID);

    bool deleteTexture(GLuint textureID);

    bool getUniformLocation(const GLuint programID, const std::string &uniformName, GLuint &location);

    //maybe this should be used by GLSLProgram, instead of model.
    bool setUniform(const GLuint programID, const GLuint uniformID, const glm::mat4 matrix);

    glm::mat4 getCameraMatrix() const {return cameraMatrix;};
    glm::mat4 getProjectionMatrix() const {return projectionMatrix;};
    glm::mat4 getOrthogonalProjectionMatrix() const { return orthogonalProjectionMatrix; }
    void drawLine(const glm::vec3 &from, const glm::vec3 &to,
                  const glm::vec3 &fromColor, const glm::vec3 &toColor, bool willTransform);

    void clearDepthBuffer() {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

};

#endif //UBERGAME_GLHELPER_H
