#pragma once
#include "RectangleObject.h"
#include "AppObject.h"

class ScoreObject : public AppObject
{
public:
    // needs 
    ScoreObject(Vertex* vertices, bool initialize = false);
    bool incrementP1Score(int score); // returns true if p1 is winner
    bool incrementP2Score(int score); // returns true if p2 is winner
    void resetScores();

protected:
    virtual void initialize();
    virtual void update();
    virtual void clean();
private:
    static const int MAXSCORE = 11;
    int _p1Score, _p2Score;
    RectangleObject* p1ScoreText;
    RectangleObject* p2ScoreText;
    static void setScoreText(RectangleObject* scoreText, int score);
};