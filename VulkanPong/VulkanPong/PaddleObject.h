#pragma once
#include "RectangleObject.h"

class PaddleObject : public RectangleObject
{
public:
    PaddleObject(Vertex* vertices, bool isP1, bool initialize = false);

protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();
private:
    bool _goUp, _isP1;
    void bounce();
};