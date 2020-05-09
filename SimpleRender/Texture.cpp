#include "Texture.h"
Texture::Texture(const QString& filePath) {
    //load texture
    QImage img;

    if (!img.load(filePath)) {

        qDebug() << "Image loading failed";
        return;
    }

    QImage t = (img.convertToFormat(QImage::Format_RGBA8888)).mirrored();
    texture = new QOpenGLTexture(t);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
}
void Texture::bind() {
    texture->bind();
}
void Texture::release() {
    texture->release();
}
Texture::~Texture() {
    texture->destroy();
}