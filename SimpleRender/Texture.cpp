#include "Texture.h"

bool Texture::load(const QString& filePath) {
    //load texture
    QImage img;

    if (!img.load(filePath)) {

        qDebug() << "Image loading failed";
        qDebug() << filePath;
        return false;
    }

    QImage t = (img.convertToFormat(QImage::Format_RGBA8888)).mirrored();
    texture = new QOpenGLTexture(t);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    id = texture->textureId();

    texture->bind();
    return true;
}
void Texture::bind() {
    texture->bind();
}
void Texture::bind(int id) {
    texture->bind(id);
}
void Texture::release() {
    texture->release();
}
Texture::~Texture() {
    texture->destroy();
}