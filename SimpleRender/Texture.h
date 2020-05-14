#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include<qstring.h>
#include<qopengltexture.h>
#include<qdebug.h>
#include<qopenglfunctions.h>
class Texture
{
public:
    Texture() {}
    bool load(const QString& filePath);
    bool load(const QString& filePath, QOpenGLFunctions* f);
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


