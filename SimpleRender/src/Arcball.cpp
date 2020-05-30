#include "Arcball.h"
#include<iostream>
#include<qdebug.h>
// Project the point in [-1, 1] screen space onto the arcball sphere
static QQuaternion screen_to_arcball(const QVector2D& p);

Arcball::Arcball(const QVector3D& eye, const QVector3D& center, const QVector3D& up)
{
	const QVector3D dir = center - eye;
	QVector3D z_axis = dir.normalized();
	QVector3D x_axis = QVector3D::crossProduct(z_axis, up.normalized()).normalized();
	QVector3D y_axis = QVector3D::crossProduct(x_axis, z_axis).normalized();
	x_axis = QVector3D::crossProduct(z_axis, y_axis).normalized();
	center_translation.translate(center);
	center_translation = center_translation.inverted();
	translation.translate(QVector3D(0.f, 0.f, -dir.length()));
	 float value[] = { x_axis.x(),y_axis.x(),-z_axis.x() ,
   x_axis.y(),y_axis.y(),-z_axis.y() ,
	x_axis.z(),y_axis.z(),-z_axis.z() };
	QMatrix3x3 temp_mat(value);
	rotation = rotation.fromRotationMatrix(temp_mat.transposed()).normalized();
	update_camera();
}

void Arcball::rotate(QVector2D prev_mouse, QVector2D cur_mouse)
{
	// Clamp mouse positions to stay in NDC
	//cur_mouse = QVector2D::c(cur_mouse, glm::vec2{ -1, -1 }, glm::vec2{ 1, 1 });
	//prev_mouse = glm::clamp(prev_mouse, glm::vec2{ -1, -1 }, glm::vec2{ 1, 1 });
	if (cur_mouse.x() < -1.0) 
		cur_mouse.setX(-1);
	 else if (cur_mouse.x() >1.0) 
		cur_mouse.setX(1);
	if (cur_mouse.y() < -1.0) 
		cur_mouse.setY(-1);
	else if (cur_mouse.y() > 1.0)
		cur_mouse.setY(1);
	if (prev_mouse.x() < -1.0)
		prev_mouse.setX(-1);
	else if (prev_mouse.x() > 1.0)
		prev_mouse.setX(1);
	if (prev_mouse.y() < -1.0)
		prev_mouse.setY(-1);
	else if (prev_mouse.y() > 1.0)
		prev_mouse.setY(1);
	const QQuaternion mouse_cur_ball = screen_to_arcball(cur_mouse);
	const QQuaternion mouse_prev_ball = screen_to_arcball(prev_mouse);

	rotation = mouse_cur_ball * mouse_prev_ball * rotation;
	update_camera();
}

void Arcball::pan(QVector2D mouse_delta)
{
	const float zoom_amount = std::abs(translation(2,3));
#ifndef NDEBUG
	std::cout << "Debug pan function";
	for (int i = 0;i < 4;i++) {
		for (int j = 0;j < 4;j++) {
			std::cout << translation(i, j)<<" ";
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
#endif
	QVector4D motion(mouse_delta.x() * zoom_amount, mouse_delta.y() * zoom_amount, 0.f, 0.f);
	// Find the panning amount in the world space
	motion = inv_camera * motion;
	QMatrix4x4 translate_mat;
	translate_mat.translate(motion.toVector3D());
	center_translation = translate_mat * center_translation;
	update_camera();
}

void Arcball::zoom(const float zoom_amount)
{
	const QVector3D motion(0.f, 0.f, zoom_amount);
	QMatrix4x4 translate_mat;
	translate_mat.translate(motion);
	translation = translate_mat * translation;
	update_camera();
}

const QMatrix4x4& Arcball::transform() const
{
	return camera;
}

const QMatrix4x4& Arcball::inv_transform() const
{
	return inv_camera;
}

QVector3D Arcball::eye() const
{
	return QVector3D( inv_camera * QVector4D{0, 0, 0, 1} );
}

QVector3D Arcball::dir() const
{
	return (inv_camera * QVector4D{0, 0, -1, 0}).toVector3D().normalized();
}

QVector3D Arcball::up() const
{
	return (inv_camera * QVector4D{ 0, 1, 0, 0 }).toVector3D().normalized();
}

void Arcball::update_camera()
{
	QMatrix4x4 rotation_mat;
	rotation_mat.rotate(rotation);
	camera = translation * rotation_mat * center_translation;
	inv_camera = camera.inverted();
}
QQuaternion screen_to_arcball(const QVector2D& p)
{
	const float dist = p.lengthSquared();
	// If we're on/in the sphere return the point on it
	if (dist <= 1.f) {
		return QQuaternion(0.0, p.x(), p.y(), std::sqrt(1.f - dist));
	}
	else {
		// otherwise we project the point onto the sphere
		const QVector2D proj =p.normalized();
		return QQuaternion(0.0, proj.x(), proj.y(), 0.f);
	}
}