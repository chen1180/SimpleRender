#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include<qstring.h>
#include<qopengltexture.h>
#include<qdebug.h>
class Texture
{
public:
    Texture(const QString& filePath);
    void bind();
    void release();
    ~Texture();
    QOpenGLTexture* texture;
};
#endif TEXTURE_H


