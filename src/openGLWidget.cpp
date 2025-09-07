#include "openGLWidget.h"
#include "config.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent), VAO(0), VBO(0), EBO(0), shaderProgram(nullptr), leftPressed(false), middlePressed(false), wireframe(false) {

}

OpenGLWidget::~OpenGLWidget() {
    makeCurrent();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete shaderProgram;
    doneCurrent();
}

int OpenGLWidget::loadMesh(const char *link) {
    int ok = mesh.loadFile(link);
    if (ok > 0) {
        mesh.clear();
    }
    QVector3D center = mesh.getCenter();
    float radius = mesh.getBoundingRadius();
    qDebug() << center << "  " << radius << "\n";
    camera.initialize(center, radius);

    updateMeshBuffers();
    return ok;
}


void OpenGLWidget::updateMeshBuffers() {
    makeCurrent();
    glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);

    auto vertices = mesh.getVertices();
    auto indices = mesh.getIndices();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    doneCurrent();

    emit verticesChanged(vertices.size());
    emit trianglesChanged(indices.size() / 3);

    update();
}

void OpenGLWidget::setWireframe(bool enabled) {
    wireframe = enabled;
    update();
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

    QString shaderDir = QString(MESHVIEWER_DATA_DIR) + "/shaders/";

    shaderProgram = new QOpenGLShaderProgram(this);
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, shaderDir + "vertex.glsl");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, shaderDir + "fragment.glsl");
    shaderProgram->link();

    updateMeshBuffers();

    emit verticesChanged(0);
    emit trianglesChanged(0);
}

void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0.f, 0.f, w, h);

    camera.setAspect(w, h);
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    auto indices = mesh.getIndices();

    QMatrix4x4 model, view, projection;
    model.setToIdentity();
    view = camera.getView();
    projection = camera.getProjection();


    shaderProgram->bind();
    shaderProgram->setUniformValue("model", model);
    shaderProgram->setUniformValue("view", view);
    shaderProgram->setUniformValue("projection", projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    shaderProgram->release();
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
    lastMousePos = event->position();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    QPointF delta = event->position() - lastMousePos;
    lastMousePos = event->position();

    if (event->buttons() & Qt::LeftButton) {
        camera.orbit(delta.x(), delta.y());
        update();
    }

    if (event->buttons() & Qt::RightButton) {
        camera.pan(delta.x(), delta.y());
        update();
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent *event) {
    camera.zoom(event->angleDelta().y() / 120.0f);
    update();
}
