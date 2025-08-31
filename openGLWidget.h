#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>

#include "camera.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;


    GLuint VAO;
    GLuint VBO;
    QOpenGLShaderProgram *shaderProgram;

    Camera camera;
    QPointF lastMousePos;
    bool leftPressed;
    bool middlePressed;

};

#endif // OPENGLWIDGET_H
