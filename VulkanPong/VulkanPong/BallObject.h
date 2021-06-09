#pragma once
#include "RectangleObject.h"

class BallObject : public RectangleObject
{
public:
    BallObject(Vertex* vertices, bool initialize = false);
    void startMoving(bool goRight, int verticalVelocity);
    void bounceBall();

protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();
private:
    bool _goUp;
    bool _goRight;
    int _verticalVelocity;
};