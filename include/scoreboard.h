#include <string>

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

class scoreboard
{
    public:
        scoreboard(int win_req);
        std::string printScoreBoard();
        void bumpLScore();
        void bumpRScore();
    private:
        int lScore;
        int rScore;
        int WIN_REQ;
        bool lWin;
        bool rWin;
};

#endif // SCOREBOARD_H
