#ifndef REPLAY_HPP
#define REPLAY_HPP

#include <iostream>
#include "Mare.hpp"

namespace R
{

    class replayBoard
    {
    public:
        // replay su file, video e entrambi
        void replayVideo();
        void replayFile(string file);
        replayBoard(string log);

    private:
        // puntatore scacchiera, il nome del file di log
        M::Mare *replay;
        string replayLog;
    };
}


#endif