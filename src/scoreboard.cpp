#include "scoreboard.h"

// ctor
scoreboard::scoreboard(int win_req)
{
    this->lScore = 0;
    this->rScore = 0;
    this->WIN_REQ = win_req;
    this->lWin = false;
    this->rWin = false;
}

void scoreboard::bumpLScore()
{
    this->lScore++;
    this->lWin = lScore >= WIN_REQ;
}

void scoreboard::bumpRScore()
{
    this->rScore++;
    this->rWin = rScore >= WIN_REQ;
}

std::string scoreboard::printScoreBoard()
{
    std::string printMe = "";
    printMe += "Left Player  : " + std::to_string(this->lScore) + " | ";
    printMe += "Right Player : " + std::to_string(this->rScore) + " | ";
    printMe += "Points to Win: " + std::to_string(this->WIN_REQ) + "\n";
    return printMe;
}

bool scoreboard::getLWin()
{
    return this->lWin;
}

bool scoreboard::getRWin()
{
    return this->rWin;
}

void scoreboard::reset()
{
    this->rWin = false;
    this->lWin = false;
    this->lScore = 0;
    this->rScore = 0;
}
