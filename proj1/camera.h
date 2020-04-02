#ifndef _CSCI441_CAMERA_H_
#define _CSCI441_CAMERA_H_

#include <csci441/matrix4.h>
#include <csci441/vector4.h>

class Camera {
public:
    Matrix4 projection;
    Vector4 eye;
    Vector4 gaze;
    Vector4 up;

    Camera() : eye(0,0,0), gaze(0,0,0), up(0,0,0) {}

    Matrix4 look_at() const {
        Matrix4 mat;
        mat.look_at(eye, gaze, up);
        return mat;
    }
};

#endif
