#pragma once
#include "AppObject.h"
#include "Vertex.h"

class RectangleObject : public AppObject
{
public:
    RectangleObject(bool initialize = false);
    RectangleObject(Vertex* vertices, int topLeftHPos, int topLeftVPos, int width, int height, bool initialize = false);

    void setVisible(bool isVisible);
    void setVertices(Vertex* vertices);
    void update(int topLeftHPos, int topLeftVPos, int width, int height);
    void updatePos(int topLeftHPos, int topLeftVPos);
    void updateSize(int width, int height);
    void updateWidth(int width);
    void updateHeight(int height);
    bool incrementHPos(int offset);
    bool incrementVPos(int offset);

protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();

private:
    // pointer to top-left corner of dynamically allocated vertex, which can be treated like a dynamic array
    Vertex* _vertices;
    int _topLeftHPos, _topLeftVPos, _width, _height;
    bool _isVisible;
    void updateVertices();
};