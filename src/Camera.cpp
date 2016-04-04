//
// Created by Engin Manap on 17.02.2016.
//

#include "Camera.h"


Camera::Camera():
        dirty(false),
        position(startPosition),
        center(glm::vec3(0,0,-1)),
        up(glm::vec3(0,1,0)),
        right(glm::vec3(-1,0,0)),
        view(glm::quat(0,0,0,-1)),
        rayCallback(btCollisionWorld::ClosestRayResultCallback(BulletGLMConverter::GLMToBlt(startPosition + glm::vec3(0,-1.01f,0)),
                                                               BulletGLMConverter::GLMToBlt(startPosition + glm::vec3(0,-2.01f,0)))),
        onAir(true){
    cameraTransformMatrix = glm::lookAt(position, center, up);
    btCollisionShape* capsuleShape = new btCapsuleShape(1,2);
    btDefaultMotionState *boxMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                                                                BulletGLMConverter::GLMToBlt(startPosition)));
    btVector3 fallInertia(0, 0, 0);
    capsuleShape->calculateLocalInertia(1, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo
            boxRigidBodyCI(1, boxMotionState, capsuleShape, fallInertia);
    player = new btRigidBody(boxRigidBodyCI);
    player->setAngularFactor(0);
    player->setFriction(1);

}

void Camera::move(moveDirections direction) {
    if(direction == NONE) {
        return;
    }
    if(onAir){
        return;
    }
    dirty=true;
    switch (direction) {
        case UP:
            player->setLinearVelocity(player->getLinearVelocity() + BulletGLMConverter::GLMToBlt(up* jumpFactor));
            break;
        case LEFT_BACKWARD:
            //position -= moveSpeed * center;
            //position -= moveSpeed * right;
            player->setLinearVelocity(BulletGLMConverter::GLMToBlt(-1.0f * (right + center )* moveSpeed));
            break;
        case LEFT_FORWARD:
            //position += moveSpeed * center;
            //position -= moveSpeed * right;
            player->setLinearVelocity(BulletGLMConverter::GLMToBlt((-1.0f * right + center )* moveSpeed));
            break;
        case LEFT:
            //position -= moveSpeed * right;
            player->setLinearVelocity(BulletGLMConverter::GLMToBlt(right * -1.0f * moveSpeed));
            break;
        case RIGHT_BACKWARD:
            //position -= moveSpeed * center;
            //position += moveSpeed * right;
            player->setLinearVelocity(BulletGLMConverter::GLMToBlt((right + -1.0f * center )* moveSpeed));
            break;
        case RIGHT_FORWARD:
            //position += moveSpeed * center;
            //position += moveSpeed * right;
            player->setLinearVelocity(BulletGLMConverter::GLMToBlt((right + center )* moveSpeed));
            break;
        case RIGHT:
            //position += moveSpeed * right;
            player->setLinearVelocity(BulletGLMConverter::GLMToBlt(right * moveSpeed));
            break;
        case BACKWARD:
            //position -= moveSpeed * center;
            player->setLinearVelocity(BulletGLMConverter::GLMToBlt(center * -1.0f * moveSpeed));
            break;
        case FORWARD:
            //position += moveSpeed * center;
            player->setLinearVelocity(BulletGLMConverter::GLMToBlt(center * moveSpeed));
            break;
    }
    //this activates user rigidbody if it moves. Otherwise island management ignores movement.
    player->setActivationState(ACTIVE_TAG);
}

void Camera::rotate(float xChange, float yChange) {
    viewChange = glm::quat(cos(yChange * lookAroundSpeed / 2),
                           right.x * sin(yChange * lookAroundSpeed / 2),
                           right.y * sin(yChange * lookAroundSpeed / 2),
                           right.z * sin(yChange * lookAroundSpeed / 2));

    view = viewChange * view * glm::conjugate(viewChange);
    view = glm::normalize(view);

    viewChange = glm::quat(cos(xChange * lookAroundSpeed / 2),
                           up.x * sin(xChange * lookAroundSpeed / 2),
                           up.y * sin(xChange * lookAroundSpeed / 2),
                           up.z * sin(xChange * lookAroundSpeed / 2));
    view = viewChange * view * glm::conjugate(viewChange);
    view = glm::normalize(view);

    center.x = view.x;
    if (view.y > 1.0f) {
        center.y = 0.9999f;
    } else if (view.y < -1.0f) {
        center.y = -0.9999f;
    } else {
        center.y = view.y;
    }
    center.z = view.z;
    center = glm::normalize(center);
    right = glm::normalize(glm::cross(center, up));
    this->dirty=true;
}

void Camera::updateTransfromFromPhysics(const btDynamicsWorld* world){

    world->rayTest(rayCallback.m_rayFromWorld, rayCallback.m_rayToWorld, rayCallback);

    if(rayCallback.hasHit()) {
        /*
        End = RayCallback.m_hitPointWorld;
        Normal = RayCallback.m_hitNormalWorld;
        */
        // Do some clever stuff here
        //std::cout << "Player is close to land" << std::endl;
        onAir = false;
    } else {
        //std::cout << "Player is on air" << std::endl;
        onAir = true;
    }

    rayCallback.m_closestHitFraction= 1;

    rayCallback.m_collisionObject = 0;



    player->getMotionState()->getWorldTransform(worldTransformHolder);

    position = BulletGLMConverter::BltToGLM(worldTransformHolder.getOrigin());
    position.y +=1;

    rayCallback.m_rayFromWorld = worldTransformHolder.getOrigin() +btVector3(0,-1.01f,0);//the second vector is preventing hitting player capsule
    rayCallback.m_rayToWorld = rayCallback.m_rayFromWorld + btVector3(0,-1,0);

    /*std::cout << "ray details: (" << rayCallback.m_rayFromWorld.getX() << ","
                                << rayCallback.m_rayFromWorld.getY() << ","
                                << rayCallback.m_rayFromWorld.getZ() << ")->("
                                << rayCallback.m_rayToWorld.getX() << ","
                                << rayCallback.m_rayToWorld.getY() << ","
                                << rayCallback.m_rayToWorld.getZ() << ")" << std::endl;*/

    //std::cout << "the objects last position is" << this->translate.x <<","<< this->translate.y <<","<<this->translate.z << std::endl;
    dirty = true;
}