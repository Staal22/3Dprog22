#ifndef OBJECTGROUP_H
#define OBJECTGROUP_H

#include "qopenglshaderprogram.h"
#include "visualobject.h"

class ObjectGroup {
public:
    ObjectGroup(QOpenGLShaderProgram* shaderProgram) : m_shaderProgram(shaderProgram) {}

    QOpenGLShaderProgram* m_shaderProgram;

    void addObject(VisualObject* object);
    void render(class RenderWindow* window, const GLint& model);

private:
    QList<VisualObject*> m_objects;

};

#endif // OBJECTGROUP_H
