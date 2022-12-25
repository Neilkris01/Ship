#include "Replay.hpp"
using namespace std;

replayBoard::replayBoard(string log)
{
    replayLog = log;
    replay = new ChessBoard();
}

void replayBoard::replayVideo()
{
    ifstream scanner(replayLog);
    string replayLine;
    getline(scanner, replayLine);
    string playerWhite;
    string playerBlack;
    // codice per rilevare le reges
    // scrivere a log come finisce la partita
    char color;

    while (getline(scanner, replayLine))
    {
        if (i % 2 == 0)
            color = 'B';
        else
            color = 'N';
        cout << "Turno di ";
        if (color = 'B')
            cout << playerWhite;
        else
            cout << playerBlack;
        cout << endl;
        this_thread::sleep_for(chrono::seconds(0.25));
        if (replayLine.size() == 5)
        {

            pair<int, int> start = pair(replayLine[0], replayLine[1]);
            pair<int, int> end = pair(replayLine[3], replayLine[4]);
            replay.performMove(start, end, color);
        }

        else
        {
            char p = replayLine[2];
            replay.performPromotion(p);
        }
        cout << replay->printBoard() << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
}