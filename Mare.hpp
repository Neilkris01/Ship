#ifndef MARE_HPP
#define MARE_HPP

#include <algorithm>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <random>
#include "Ship.hpp"

using namespace std;

namespace M
{
    class Mare
    {
        public:
        //nave, destinazione, codice della mossa
        struct Move
        {
            //nave mossa
            S::Ship* ship;
            //posizione di arrivo
            pair<int, int> destination;
            Move(S::Ship* p, pair<int, int> dest);
            Move();
        };
        //ritorna vettore con tutte le posizioni possibili per il pMedio della nave
        vector<Move> posAvailable(S::Ship* pos);
        //metodo generale
        //ritorna true se è possibile la mossa
        bool performMove();
        //metodo per computer: mossa presa automaticamente da nextPlayerMoves
        //ritorna true se è possibile muovere
        bool performMove();
        //metodo per giocatore: fornire posizioni
        //ritorna true se è possibile muovere
        bool performMove(char tipo, pair<int, int>& start, pair<int, int>& destination);
        //stampa la griglia
        string printMare();
        //aggiorna log con informazioni su vittoria
        //0 se vittoria, 1 se stallo, 2 se patta per numero di mosse,
        //3 se patta per raggiungimento limite mosse in partita tra bot
        void updateLogVictory(int ending);
        //aggiorna log con tipo di partita
        void updateLogGameType(string type);
        //----------------------------------------------------------------------
        //TOGLIERLA ALLA FINE DI TUTTO
        //importa board da file                         !!!!!!!!!!!!!!!!!!!!!!!!
        void justForDebug(string fileName);
        //----------------------------------------------------------------------
        //eccezione: input non valido
        class InvalidInputException {};
        //eccezione: mossa non valida
        class InvalidMoveException {};
    private:
        //ogni vettore rappresenta una riga
        vector<vector<S::Ship*>> grill;
        //ordine: C, S, E
        vector<S::Ship*> ShipList;
        //ultima mossa effettuata
        Move lastMove;
        //nome del file su cui effettuare il log
        string logFile;
        //salva la condizione di giocatore attuale
        int condition = -1;
        //vettore contenente le posizioni disponibili della nave attuale
        vector<Move> nextShypMoves;
        //mappa che salva le configurazioni apparse e il numero di apparizioni di ciascuna
        map<string, int> positions;
        //numero di mosse effettuate
        int drawMoves;
        //numero di righe e colonne della scacchiera
        const int SIZE = 12;
        bool legitMoveInput(pair<int, int>& x);
        bool scanBoundaries(pair<int, int>& pos);
        bool scanBoundaries(int row, int column);
        ///se casella occupata, ritorna 1
        //altrimenti, ritorna 0
        char scanOccupied(pair<int, int>& pos);
        char scanOccupied(int row, int column);
        void initializeRow(int row);
        void insertShip(S::Ship* piece, pair<int, int>* pos);
        void scanAddSpecialMoves(vector<Move>& moves);
        //row e column: eventuali coordinate modificate del re
        bool scanCheckmateImpossibility();
        //s1 = Nds, s2 = Altra nave
        bool healConditions(S::Ship* p1, S::Ship* p2);
        //aggiorna log con mossa
        void updateLogMove(pair<int, int> start, pair<int, int> finish);
        //aggiorna log con hitMarker
        void updateLogHit();
        //aggiorna log con scacco
        void updateLogCheck();
    };
    //operatore di confronto per Move
    //controlla se pezzo e posizione di arrivo sono uguali
    bool operator==(const Mare::Move& m1, const Mare::Move& m2);
}


#endif