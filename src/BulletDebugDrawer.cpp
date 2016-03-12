//
// Created by engin-evam on 9.03.2016.
//

#include "BulletDebugDrawer.h"


void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color){

    glm::mat4 viewMatrix = glHelper->getProjectionMatrix() * glHelper->getCameraMatrix();
    GLuint viewTransformLocation;
    renderProgram->getUniformLocation("cameraTransformMatrix", viewTransformLocation);
    glHelper->setUniform(renderProgram->getID(),viewTransformLocation,viewMatrix);
    glHelper->drawLine(renderProgram->getID(), vao, vbo, ebo,
                       BulletGLMConverter::BltToGLM(from),
                       BulletGLMConverter::BltToGLM(to),
                       BulletGLMConverter::BltToGLM(color),
                       BulletGLMConverter::BltToGLM(color));
}
