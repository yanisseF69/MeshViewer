#include "openGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent), VAO(0), VBO(0), shaderProgram(nullptr), leftPressed(false), middlePressed(false){

}

OpenGLWidget::~OpenGLWidget() {
    makeCurrent();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete shaderProgram;
    doneCurrent();
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
    float vertices[] = {
        0.0f,  0.5f, 0.0f, 1.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f,  1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shaderProgram = new QOpenGLShaderProgram(this);
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vertex.glsl");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fragment.glsl");
    shaderProgram->link();
}

void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0.f, 0.f, w, h);

    camera.setAspect(w, h);
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 model, view, projection;
    model.setToIdentity();
    view = camera.getView();
    projection = camera.getProjection();


    shaderProgram->bind();
    shaderProgram->setUniformValue("model", model);
    shaderProgram->setUniformValue("view", view);
    shaderProgram->setUniformValue("projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
    glBindVertexArray(0);
    shaderProgram->release();
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
    lastMousePos = event->position();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    QPointF delta = event->position() - lastMousePos;
    lastMousePos = event->position();

    float angleX = camera.getAngleX();
    float angleY = camera.getAngleY();

    if (event->buttons() & Qt::LeftButton) {
        angleX += delta.x() * 0.5f;
        angleY -= delta.y() * 0.5f;
        if (angleY > 89.0f) angleY = 89.0f;
        if (angleY < -89.0f) angleY = -89.0f;
        camera.setAngleX(angleX);
        camera.setAngleY(angleY);
        update();
    }

}

void OpenGLWidget::wheelEvent(QWheelEvent *event) {
    camera.zoom(event->angleDelta().y() / 120.0f);
    update();
}
