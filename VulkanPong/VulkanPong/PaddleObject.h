#pragma once
#include "RectangleObject.h"

class PaddleObject : public RectangleObject
{
public:
    PaddleObject(Vertex* vertices, bool isP1, bool initialize = false);
    //void setUpHold(bool upHold);
    //void setDownHold(bool downHold);
    void move(bool upHold, bool downHold);
protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();
private:
    bool _isP1;
};