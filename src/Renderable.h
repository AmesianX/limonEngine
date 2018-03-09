//
// Created by Engin Manap on 3.03.2016.
//

#ifndef LIMONENGINE_RENDERABLE_H
#define LIMONENGINE_RENDERABLE_H


#include "GameObjects/GameObject.h"
#include "GLHelper.h"
#include "GLSLProgram.h"
#include <btBulletDynamicsCommon.h>
#include <glm/gtx/matrix_decompose.hpp>

class Renderable {
protected:
    GLHelper *glHelper;
    std::vector<uint_fast32_t > bufferObjects;
    uint_fast32_t vao, ebo;
    GLSLProgram *renderProgram;
    glm::mat4 worldTransform, oldWorldTransform;
    glm::vec3 scale, translate;
    glm::quat orientation;
    bool isDirty;
    bool isRotated;

    void generateWorldTransform();

    explicit Renderable(GLHelper *glHelper);


public:
    virtual void addScale(const glm::vec3 &scale) {
        this->scale *= scale;
        isDirty = true;
    }

    virtual void addTranslate(const glm::vec3 &translate) {
        this->translate += translate;
        isDirty = true;
    }

    virtual void addOrientation(const glm::quat &orientation) {
        this->orientation *= orientation;
        this->orientation = glm::normalize(this->orientation);
        isRotated = this->orientation.w > cos(0.1f / 2); //if the total rotation is bigger than 0.1 rad
        isDirty = true;
    }

    const glm::mat4 &getWorldTransform() {
        if (isDirty) {
            generateWorldTransform();
        }
        return worldTransform;
    }

    virtual void render() = 0;

    virtual void setupForTime(long time) = 0;

    virtual ~Renderable() {
        for (unsigned int i = 0; i < bufferObjects.size(); ++i) {
            glHelper->freeBuffer(bufferObjects[i]);
        }
        glHelper->freeBuffer(ebo);
        glHelper->freeVAO(vao);

        //model renderable creates its own
        delete renderProgram;

    }

};


#endif //LIMONENGINE_RENDERABLE_H
