#include "objectgroup.h"

void ObjectGroup::addObject(VisualObject *object)
{
    m_objects.push_back(object);
}

void ObjectGroup::render()
{
    m_shaderProgram->bind();
    for (VisualObject* object : m_objects)
    {
        object->draw();
    }
    m_shaderProgram->release();
}
