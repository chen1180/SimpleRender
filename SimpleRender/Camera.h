#pragma once
#ifndef Camera_H
#include<qvector3d.h>
#include<qmatrix4x4.h>
class Camera
{
public:
	Camera(QVector3D eye_pos, QVector3D center, QVector3D worldUp);
	~Camera();
	QVector3D eye_pos;
	QVector3D eye_front;
	QVector3D eye_up;
	QVector3D worldUp;
	QVector3D center;
	QMatrix4x4 viewMat;
};
#endif Camera_H



