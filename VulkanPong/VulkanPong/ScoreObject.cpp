#pragma once
#include "ScoreObject.h"


ScoreObject::ScoreObject(Vertex* vertices, bool initialize)
{
	_p1Score = 0;
	_p2Score = 0;

	p1ScoreText = new RectangleObject[9];
	p2ScoreText = new RectangleObject[9];

	// map vertices to rectangle
	for (int i = 0; i < 9; i++)
	{
		p1ScoreText[i].setVertices(vertices + (i * 4));
		p2ScoreText[i].setVertices(vertices + (i * 4) + 36);
	}

	// set up pos for p1
	int offset1 = (PIXEL_WIDTH / 4);
	p1ScoreText[0].updatePos(offset1 - 6, 4);
	p1ScoreText[1].updatePos(offset1 - 6, 10);
	p1ScoreText[2].updatePos(offset1 + 3, 4);
	p1ScoreText[3].updatePos(offset1 + 3, 10);
	p1ScoreText[4].updatePos(offset1 + 7, 4);
	p1ScoreText[5].updatePos(offset1 + 7, 10);
	p1ScoreText[6].updatePos(offset1 + 3, 4);
	p1ScoreText[7].updatePos(offset1 + 3, 10);
	p1ScoreText[8].updatePos(offset1 + 3, 18);

	// set up pos for p2
	int offset2 = offset1 * 3;
	p2ScoreText[0].updatePos(offset2 - 6, 4);
	p2ScoreText[1].updatePos(offset2 - 6, 10);
	p2ScoreText[2].updatePos(offset2 + 3, 4);
	p2ScoreText[3].updatePos(offset2 + 3, 10);
	p2ScoreText[4].updatePos(offset2 + 7, 4);
	p2ScoreText[5].updatePos(offset2 + 7, 10);
	p2ScoreText[6].updatePos(offset2 + 3, 4);
	p2ScoreText[7].updatePos(offset2 + 3, 10);
	p2ScoreText[8].updatePos(offset2 + 3, 18);

	// set up size for p1
	p1ScoreText[0].updateSize(2, 8);
	p1ScoreText[1].updateSize(2, 10);
	p1ScoreText[2].updateSize(2, 8);
	p1ScoreText[3].updateSize(2, 10);
	p1ScoreText[4].updateSize(2, 8);
	p1ScoreText[5].updateSize(2, 10);
	p1ScoreText[6].updateSize(6, 2);
	p1ScoreText[7].updateSize(6, 2);
	p1ScoreText[8].updateSize(6, 2);

	// set up size for p2
	p2ScoreText[0].updateSize(2, 8);
	p2ScoreText[1].updateSize(2, 10);
	p2ScoreText[2].updateSize(2, 8);
	p2ScoreText[3].updateSize(2, 10);
	p2ScoreText[4].updateSize(2, 8);
	p2ScoreText[5].updateSize(2, 10);
	p2ScoreText[6].updateSize(6, 2);
	p2ScoreText[7].updateSize(6, 2);
	p2ScoreText[8].updateSize(6, 2);

	// initialize rectangles
	for (int i = 0; i < 9; i++)
	{
		p1ScoreText[i].Initialize();
		p1ScoreText[i].setVisible(true);
		p2ScoreText[i].Initialize();
		p2ScoreText[i].setVisible(true);
	}

	// if initialize, initialize text
	if (initialize)
		this->initialize();
}
// returns true if p1 is winner
bool ScoreObject::incrementP1Score(int score)
{
	_p1Score++;
	return _p1Score >= MAXSCORE ? true : false;
}

// returns true if p2 is winner
bool ScoreObject::incrementP2Score(int score)
{
	_p2Score++;
	return _p2Score >= MAXSCORE ? true : false;
}
void ScoreObject::resetScores()
{
	_p1Score = 0;
	_p2Score = 0;
	setScoreText(p1ScoreText, _p1Score);
	setScoreText(p2ScoreText, _p2Score);
}

void ScoreObject::initialize()
{
	// map vertices to rectangle
	for (int i = 0; i < 9; i++)
	{
		p1ScoreText[i].Initialize();
		p2ScoreText[i].Initialize();
	}

	setScoreText(p1ScoreText, _p1Score);
	setScoreText(p2ScoreText, _p2Score);
}

void ScoreObject::update()
{
	setScoreText(p1ScoreText, _p1Score);
	setScoreText(p2ScoreText, _p2Score);
}

void ScoreObject::clean()
{
}

void ScoreObject::setScoreText(RectangleObject* scoreText, int score)
{
	switch (score)
	{
		case 0:
			scoreText[0].setVisible(false);
			scoreText[1].setVisible(false);
			scoreText[2].setVisible(true);
			scoreText[3].setVisible(true);
			scoreText[4].setVisible(true);
			scoreText[5].setVisible(true);
			scoreText[6].setVisible(true);
			scoreText[7].setVisible(false);
			scoreText[8].setVisible(true);
			break;
		case 1:
			scoreText[0].setVisible(false);
			scoreText[1].setVisible(false);
			scoreText[2].setVisible(false);
			scoreText[3].setVisible(false);
			scoreText[4].setVisible(true);
			scoreText[5].setVisible(true);
			scoreText[6].setVisible(false);
			scoreText[7].setVisible(false);
			scoreText[8].setVisible(false);
			break;
		case 2:
			scoreText[0].setVisible(false);
			scoreText[1].setVisible(false);
			scoreText[2].setVisible(false);
			scoreText[3].setVisible(true);
			scoreText[4].setVisible(true);
			scoreText[5].setVisible(false);
			scoreText[6].setVisible(true);
			scoreText[7].setVisible(true);
			scoreText[8].setVisible(true);
			break;
		case 3:
			scoreText[0].setVisible(false);
			scoreText[1].setVisible(false);
			scoreText[2].setVisible(false);
			scoreText[3].setVisible(false);
			scoreText[4].setVisible(true);
			scoreText[5].setVisible(true);
			scoreText[6].setVisible(true);
			scoreText[7].setVisible(true);
			scoreText[8].setVisible(true);
			break;
		case 4:
			scoreText[0].setVisible(false);
			scoreText[1].setVisible(false);
			scoreText[2].setVisible(true);
			scoreText[3].setVisible(false);
			scoreText[4].setVisible(true);
			scoreText[5].setVisible(true);
			scoreText[6].setVisible(false);
			scoreText[7].setVisible(true);
			scoreText[8].setVisible(false);
			break;
		case 5:
			scoreText[0].setVisible(false);
			scoreText[1].setVisible(false);
			scoreText[2].setVisible(true);
			scoreText[3].setVisible(false);
			scoreText[4].setVisible(false);
			scoreText[5].setVisible(true);
			scoreText[6].setVisible(true);
			scoreText[7].setVisible(true);
			scoreText[8].setVisible(true);
			break;
		case 6:
			scoreText[0].setVisible(false);
			scoreText[1].setVisible(false);
			scoreText[2].setVisible(true);
			scoreText[3].setVisible(true);
			scoreText[4].setVisible(false);
			scoreText[5].setVisible(true);
			scoreText[6].setVisible(false);
			scoreText[7].setVisible(true);
			scoreText[8].setVisible(true);
			break;
		case 7:
			scoreText[0].setVisible(false);
			scoreText[1].setVisible(false);
			scoreText[2].setVisible(false);
			scoreText[3].setVisible(false);
			scoreText[4].setVisible(true);
			scoreText[5].setVisible(true);
			scoreText[6].setVisible(true);
			scoreText[7].setVisible(false);
			scoreText[8].setVisible(false);
			break;
		case 8:
			scoreText[0].setVisible(false);
			scoreText[1].setVisible(false);
			scoreText[2].setVisible(true);
			scoreText[3].setVisible(true);
			scoreText[4].setVisible(true);
			scoreText[5].setVisible(true);
			scoreText[6].setVisible(true);
			scoreText[7].setVisible(true);
			scoreText[8].setVisible(true);
			break;
		case 9:
			scoreText[0].setVisible(false);
			scoreText[1].setVisible(false);
			scoreText[2].setVisible(true);
			scoreText[3].setVisible(false);
			scoreText[4].setVisible(true);
			scoreText[5].setVisible(true);
			scoreText[6].setVisible(true);
			scoreText[7].setVisible(true);
			scoreText[8].setVisible(false);
			break;
		case 10:
			scoreText[0].setVisible(true);
			scoreText[1].setVisible(true);
			scoreText[2].setVisible(true);
			scoreText[3].setVisible(true);
			scoreText[4].setVisible(true);
			scoreText[5].setVisible(true);
			scoreText[6].setVisible(true);
			scoreText[7].setVisible(false);
			scoreText[8].setVisible(true);
			break;
		case 11:
			scoreText[0].setVisible(true);
			scoreText[1].setVisible(true);
			scoreText[2].setVisible(false);
			scoreText[3].setVisible(false);
			scoreText[4].setVisible(true);
			scoreText[5].setVisible(true);
			scoreText[6].setVisible(false);
			scoreText[7].setVisible(false);
			scoreText[8].setVisible(false);
			break;
		default:
			scoreText[0].setVisible(false);
			scoreText[1].setVisible(false);
			scoreText[2].setVisible(false);
			scoreText[3].setVisible(false);
			scoreText[4].setVisible(false);
			scoreText[5].setVisible(false);
			scoreText[6].setVisible(false);
			scoreText[7].setVisible(false);
			scoreText[8].setVisible(false);
			break;
	}
}