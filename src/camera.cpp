#include "camera.h"

Camera::Camera(QVector3D startPosition, QVector3D startUp, float startYaw, float startPitch)
    : position(startPosition), worldUp(startUp), yaw(startYaw), pitch(startPitch), fov(45.0f), aspect(1.0f), angleX(0.0f), angleY(0.0f), distance(5.0f) {
    updateCameraVectors();
}

QVector3D Camera::getPosition() const{
    return position;
}

QVector3D Camera::getTarget() const{
    return target;
}

QMatrix4x4 Camera::getView() {
    QMatrix4x4 view;
    float radX = qDegreesToRadians(angleX);
    float radY = qDegreesToRadians(angleY);

    QVector3D cameraPos(
        distance * cos(radY) * sin(radX),
        distance * sin(radY),
        distance * cos(radY) * cos(radX)
        );

    position = cameraPos;

    view.lookAt(position, target, up);
    return view;
}

QMatrix4x4 Camera::getProjection() const{
    QMatrix4x4 proj;

    proj.perspective(fov, aspect, 1.0f, 100.0f);
    return proj;
}

float Camera::getAngleX() const {
    return angleX;
}

float Camera::getAngleY() const {
    return angleY;
}

void Camera::setPosition(QVector3D pos) {
    position = pos;
    updateCameraVectors();
}

void Camera::setTarget(QVector3D target) {
    target = target;
    updateCameraVectors();
}

void Camera::setAspect(float width, float height) {
    aspect = width / height;
}

void Camera::setAspect(float aspect) {
    aspect = aspect;
}

void Camera::setAngleX(float x) {
    angleX = x;
}

void Camera::setAngleY(float y) {
    angleY = y;
}

void Camera::initialize(QVector3D center, float radius) {
    setTarget(center);
    QVector3D initPos = center + QVector3D(0, 0, radius * 1.5f + 1);
    setPosition(initPos);
    distance = (initPos - center).length();
    setAngleX(0);
    setAngleY(0);

    updateCameraVectors();
}

void Camera::orbit(float xoffset, float yoffset, bool constrainPitch) {
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if(pitch > 89.0f) pitch = 89.0f;
        if(pitch < -89.0f) pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::zoom(float yoffset) {
    fov -= yoffset;
    if(fov < 1.0f) fov = 1.0f;
    if(fov > 90.0f) fov = 90.0f;
}

void Camera::updateCameraVectors() {
    QVector3D f;
    f.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    f.setY(sin(qDegreesToRadians(pitch)));
    f.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));

    front = f.normalized();
    right = QVector3D::crossProduct(front, worldUp).normalized();
    up = QVector3D::crossProduct(right, front).normalized();
}
