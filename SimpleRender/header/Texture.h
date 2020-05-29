#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include<qstring.h>
#include<qopengltexture.h>
#include<qdebug.h>
#include<qopenglfunctions.h>
#include<qopenglfunctions_4_3_core.h>
class Texture
{
public:
    Texture() {}
    bool load(const QString& filePath);
    bool load(const QString& filePath, QOpenGLFunctions_4_3_Core* f);
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


