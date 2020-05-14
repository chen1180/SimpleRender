#include "Texture.h"

bool Texture::load(const QString& filePath) {
    //load texture
    QImage img;

    if (!img.load(filePath)) {

        qDebug() << "Image loading failed";
        qDebug() << filePath;
        return false;
    }

    QImage t = (img.convertToFormat(QImage::Format_RGB888)).mirrored();
    texture = new QOpenGLTexture(t);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    id = texture->textureId();

    texture->bind();
    return true;
}
bool Texture::load(const QString& filePath, QOpenGLFunctions* f) {
    //create texture
    f->glGenTextures(1, &id);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    f->glBindTexture(GL_TEXTURE_2D, id);
    QImage img(filePath);
    if (img.isNull()) {
        qFatal("image is null");
        return false;
    }
    //naive solution, better encapsulate the format in a function
    if (img.format() == QImage::Format_Indexed8) {
        f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, img.width(), img.height(), 0, GL_RED, GL_UNSIGNED_BYTE, img.scanLine(0));
    }
    else if (img.format() == QImage::Format_RGB888) {
        f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width(), img.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.scanLine(0));
    }
    else {
        QImage temp = img.convertToFormat(QImage::Format_RGB888);
        f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width(), img.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, temp.scanLine(0));
    }

    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    f->glBindTexture(GL_TEXTURE_2D, 0);
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