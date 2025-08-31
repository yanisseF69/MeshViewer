#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>

class Camera {

public:

    Camera(QVector3D startPosition = QVector3D(0.0f, 0.0f, 3.0f),
           QVector3D startUp = QVector3D(0.0f, 1.0f, 0.0f),
           float startYaw = -90.0f,
           float startPitch = 0.0f);

    QMatrix4x4 getView() const;
    QMatrix4x4 getProjection() const;
    float getAngleX() const;
    float getAngleY() const;

    void setAspect(float width, float height);
    void setAspect(float aspect);
    void setAngleX(float x);
    void setAngleY(float y);

    void orbit(float xoffset, float yoffset, bool constrainPitch = false);
    void zoom(float yoffset);


protected:

    void updateCameraVectors();

    QVector3D position;
    QVector3D front;
    QVector3D up;
    QVector3D right;
    QVector3D worldUp;
    QVector3D target;

    float yaw;
    float pitch;
    float fov;

    float aspect;
    float angleX;
    float angleY;
    float distance;

};

#endif // CAMERA_H
