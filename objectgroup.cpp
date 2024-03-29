#include "objectgroup.h"
#include "player.h"
#include "renderwindow.h"

void ObjectGroup::addObject(VisualObject *object)
{
    m_objects.push_back(object);
}

void ObjectGroup::render(RenderWindow* window)
{
    window->mCamera.init(window->projectionMatrixUniform, window->viewMatrixUniform);

    window->mCamera.perspective(90.f, 16.0f/9.0f, 0.1f, 100.0f);
    //    mCamera.lookAt(mCamera.mEye, mCamera.mEye + QVector3D::crossProduct(mCamera.left, mCamera.up), mCamera.up);
    Player* player = static_cast<Player*>(window->mMap["player"]);
    QVector3D playerPos = player->getPosition3D();

    if(window->mCamera.thirdPerson)
    {
        window->mCamera.lookAt(window->mCamera.mEye, playerPos, window->mCamera.up);
    }
    else
    {
        window->mCamera.lookAt(window->mCamera.mEye, QVector3D(playerPos.x() + 3, playerPos.y(), playerPos.z()), window->mCamera.up);
    }

    QMatrix4x4 projection = window->mCamera.getProjecionMatrix();
    QMatrix4x4 view = window->mCamera.getViewMatrix();
    m_shaderProgram->bind();
    window->mCamera.update();
    for (VisualObject* object : m_objects)
    {
        m_shaderProgram->setUniformValue("objectColor", object->objectColor);
        m_shaderProgram->setUniformValue("lightColor", window->light->lightColor);
        m_shaderProgram->setUniformValue("lightPos", window->light->getPosition3D());
        m_shaderProgram->setUniformValue("viewPos", window->mCamera.mEye);
        m_shaderProgram->setUniformValue("hasHeightMap", object->hasHeightMap);

        QMatrix4x4 model = object->getModelMatrix();
        m_shaderProgram->setUniformValue("model", model);
        m_shaderProgram->setUniformValue("projection", projection);
        m_shaderProgram->setUniformValue("view", view);

        object->draw(window->modelMatrixUniform);
        if(window->mRotate)
        {

            object->rotate();
        }
    }
    m_shaderProgram->release();
}
