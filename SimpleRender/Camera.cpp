#include "Camera.h"

 Camera::Camera(QVector3D eye_pos, QVector3D center, QVector3D up) : eye_pos(eye_pos), center(center), worldUp(up){
     viewMat.lookAt(eye_pos, center, up);

}