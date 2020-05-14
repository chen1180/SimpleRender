#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include<qstring.h>
#include<qopengltexture.h>
#include<qdebug.h>
class Texture
{
public:
    Texture() {}
    bool load(const QString& filePath);
    void bind();
    void bind(int);
    void release();
    ~Texture();
    QOpenGLTexture* texture;
    bool isLoaded=false;
    QString name;
    GLuint id;
};
#endif TEXTURE_H


