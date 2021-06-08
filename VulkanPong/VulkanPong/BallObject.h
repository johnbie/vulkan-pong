#pragma once
#include "RectangleObject.h"

class BallObject : public RectangleObject
{
public:
    BallObject(Vertex* vertices, bool initialize = false);

protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();
private:
    bool _goUp, _goRight;
    void bounceBall();
};