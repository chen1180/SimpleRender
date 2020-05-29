#pragma once
#include<qvector3d.h>
#include<qvector2d.h>
#include<qmatrix4x4.h>
#include<qquaternion.h>
class Arcball
{// We store the unmodified look at matrix along with
    // decomposed translation and rotation components
    QMatrix4x4 center_translation, translation;
    QQuaternion rotation;
    // camera is the full camera transform,
    // inv_camera is stored as well to easily compute
    // eye position and world space rotation axes
    QMatrix4x4 camera, inv_camera;

public:
    /* Create an arcball camera focused on some center point
     * screen: [win_width, win_height]
     */
    Arcball(const QVector3D& eye, const QVector3D& center, const QVector3D& up);

    /* Rotate the camera from the previous mouse position to the current
     * one. Mouse positions should be in normalized device coordinates
     */
    void rotate(QVector2D prev_mouse, QVector2D cur_mouse);

    /* Pan the camera given the translation vector. Mouse
     * delta amount should be in normalized device coordinates
     */
    void pan(QVector2D mouse_delta);

    /* Zoom the camera given the zoom amount to (i.e., the scroll amount).
     * Positive values zoom in, negative will zoom out.
     */
    void zoom(const float zoom_amount);

    // Get the camera transformation matrix
    const QMatrix4x4& transform() const;

    // Get the camera's inverse transformation matrix
    const QMatrix4x4& inv_transform() const;

    // Get the eye position of the camera in world space
    QVector3D eye() const;

    // Get the eye direction of the camera in world space
    QVector3D dir() const;

    // Get the up direction of the camera in world space
    QVector3D up() const;

    bool isMoving = false;

private:
    void update_camera();
};

