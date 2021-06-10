#pragma once
#include "RectangleObject.h"

class BallObject : public RectangleObject
{
public:
    BallObject(Vertex* vertices, bool initialize = false);
    void startBall(int verticalPos, bool goRight, int verticalVelocity);
    void changeDirection(int verticalVelocity);
    bool moveBall();

protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();
private:
    bool _goUp;
    bool _goRight;
    int _verticalVelocity;
};