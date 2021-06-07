#pragma once
#include "AppObject.h"
#include "Vertex.h"

class RectangleObject : public AppObject
{
public:
    RectangleObject(Vertex* vertices, int topLeftHPos, int topLeftVPos, int width, int height, bool initialize = false);

protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();

    void setVisible(bool isVisible);
    void update(int topLeftHPos, int topLeftVPos, int width, int height);
    void updatePos(int topLeftHPos, int topLeftVPos);
    void updateSize(int width, int height);
    void updateWidth(int width);
    void updateHeight(int height);
private:
    // pointer to top-left corner of dynamically allocated vertex, which can be treated like a dynamic array
    Vertex* vertices;
    int topLeftHPos, topLeftVPos, width, height;
    bool isVisible;
    void updateVertices();
};