#include "objectgroup.h"
#include "renderwindow.h"

void ObjectGroup::addObject(VisualObject *object)
{
    m_objects.push_back(object);
}

void ObjectGroup::render(RenderWindow* window, const GLint& model)
{
    m_shaderProgram->bind();
    window->mCamera.update();
    for (VisualObject* object : m_objects)
    {
        object->draw(model);
    }
    m_shaderProgram->release();
}
