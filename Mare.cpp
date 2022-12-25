#include "mare.hpp"

using namespace std;
namespace M
{
    Mare::Move::Move(S::Ship *p, pair<int, int> dest) : ship{p}, destination{dest} {}

    Mare::Move::Move() : ship{nullptr}, destination{pair(-1, -1)} {}

    bool operator==(const Mare::Move &m1, const Mare::Move &m2)
    {
        return (m1.ship == m2.ship && m1.destination == m2.destination);
    }

    bool Mare::legitMoveInput(pair<int, int> &x)
    {
        return (x.first >= 0 && x.first < 12 && x.second >= 0 && x.second < 12);
    }

    bool Mare::scanBoundaries(pair<int, int> &pos)
    {
        return (pos.first >= 0 && pos.first < SIZE && pos.second >= 0 && pos.second < SIZE);
    }

    bool Mare::scanBoundaries(int row, int column)
    {
        pair<int, int> tmp = pair(row, column);
        bool result = scanBoundaries(tmp);
        return result;
    }

    char Mare::scanOccupied(pair<int, int> &pos) // !!!!!!!!!!
    {
        S::Ship *ship = board[pos.first][pos.second];
        if (!ship)
            return 0;
        else
            return ship->GetColor();
    }

    char Mare::scanOccupied(int row, int column)
    {
        pair<int, int> tmp = pair(row, column);
        char result = scanOccupied(tmp);
        return result;
    }

    bool Mare::scanPromotion(S::Ship *ship)
    {
        return (ship->GetName() == 'P' && ship->GetPosition().first == 0 ||
                ship->GetName() == 'p' && ship->GetPosition().first == 7);
    }

    bool Mare::scanCheck(char color, int row, int column)
    {
        int offset = 0;
        if (color == 'N')
            offset = SIZE * 2;

        pair<int, int> pos;
        vector<vector<S::Ship *>> *tmpBoard;
        if (row == -1 && column == -1)
        {
            S::Ship *king = piecesList[4 + offset];
            pos = king->GetPosition();
        }
        else
            pos = pair(row, column);
        for (auto it = directionsPieces.begin(); it != directionsPieces.end(); ++it)
        {
            pair<int, int> direction = it->first;
            string pieces = it->second;
            pair<int, int> tmp = pair(pos.first + direction.first, pos.second + direction.second);
            int i = 1;
            while (scanBoundaries(tmp))
            {
                char pieceColor = scanOccupied(tmp);
                char pieceName;
                if (pieceColor != 0)
                {
                    if (pieceColor == color)
                        break;
                    char pieceName = board[tmp.first][tmp.second]->GetName();
                    if (pieceName > 90)
                        pieceName -= 32;
                    auto searchResult = find(pieces.begin(), pieces.end(), pieceName);
                    if (searchResult != pieces.end() && ((pieceName != 'P' && pieceName != 'R') ||
                                                        i == 1))
                        return true;

                    else
                        break;
                }
                tmp.first += direction.first;
                tmp.second += direction.second;
                i++;
            }
        }
        for (int i = 0; i < directionsHorse.size(); i++)
        {
            pair<int, int> direction = directionsHorse[i];
            pair<int, int> tmp = pair(pos.first + direction.first, pos.second + direction.second);
            if (scanBoundaries(tmp))
            {
                char pieceColor = scanOccupied(tmp);
                if (pieceColor != 0 && pieceColor != color)
                {
                    char pieceName = board[tmp.first][tmp.second]->GetName();
                    if (pieceName > 90)
                        pieceName -= 32;
                    if (pieceName == 'C')
                        return true;
                }
            }
        }
        return false;
    }

    bool Mare::scanCheck(Move &move, char color)
    {
        vector<vector<S::Ship *>> oldBoard = board;
        S::Ship *ship = move.ship;
        pair<int, int> start = move.ship->GetPosition();
        pair<int, int> end = move.destination;
        board[end.first][end.second] = ship;
        board[start.first][start.second] = nullptr;
        switch (move.moveName)
        {
        case 0:
            break;
        case 3:
        {                                         // arrocco corto
            S::Ship *tower = move.additionalPiece; // colonna attuale: 7, col. destinazione: 5
            pair<int, int> pos = tower->GetPosition();
            board[pos.first][pos.second] = nullptr;
            board[pos.first][pos.second - 2] = tower;
            break;
        }
        case 4:
        {                                         // arrocco lungo
            S::Ship *tower = move.additionalPiece; // colonna attuale: 0, col. destinazione: 4
            pair<int, int> pos = tower->GetPosition();
            board[pos.first][pos.second] = nullptr;
            board[pos.first][pos.second + 3] = tower;
            break;
        }
        }
        bool result;
        if (ship->GetName() == 'R' || ship->GetName() == 'r')
            result = scanCheck(color, end.first, end.second);
        else
            result = scanCheck(color);
        board = oldBoard;
        return result;
    }

    bool Mare::scanCheckMate(bool initialCheck, vector<Move> &moves)
    {
        return initialCheck && moves.size() == 0;
    }

    bool Mare::enPassantConditions(S::Ship *p1, S::Ship *p2)
    {
        if (!p2)
            return false; // controlla se esiste pezzo
        char n1 = p1->GetName();
        char n2 = p2->GetName();
        int row2 = p2->GetPosition().first;
        int row1 = p1->GetPosition().first;
        return (row1 == row2 && (n2 == 'P' && row2 == 4 || n2 == 'p' && row2 == 3) &&
                lastMove.ship == p2 && p2->GetStatus() == 1 && n1 - n2 != 0);
    }

    bool Mare::castlingConditions(S::Ship *king, S::Ship *tower)
    {
        if (!(king && tower))
            return 0;
        if (!(king->GetStatus() == 0 && tower->GetStatus() == 0))
            return 0;
        int start = king->GetPosition().second;
        int finish = tower->GetPosition().second;
        int row = king->GetPosition().first;
        int tmp = tower->GetPosition().first;
        char color = king->GetColor();
        if (row != tmp)
            return 0; // DA TOGLIERE INSIEME A JUSTFORDEBUG
        int factor;
        if (start < finish)
            factor = 1;
        else
            factor = -1;
        if (scanCheck(color))
            return false;
        for (int i = 1; i < abs(start - finish); i++)
        {
            Move tmp = Move(king, pair(row, start + i * factor), 0);
            if (scanOccupied(row, start + i * factor) != 0 || scanCheck(tmp, color))
                return false;
        }
        return true;
    }

    void Mare::scanAddSpecialMoves(vector<Move> &moves, char color)
    {
        int offset = 0;
        if (color == 'N')
            offset = SIZE * 2;

        // arrocco
        S::Ship *firstTower = piecesList[0 + offset];
        S::Ship *secondTower = piecesList[7 + offset];
        S::Ship *king = piecesList[4 + offset];
        // arrocco lungo
        if (castlingConditions(king, firstTower))
            moves.push_back(Move(king, pair(0, 2), 4, firstTower));
        // arrocco corto
        if (castlingConditions(king, secondTower))
            moves.push_back(Move(king, pair(0, 6), 3, secondTower));

        // en passant + controllare se pedoni hanno pedine da mangiare in diagonale
        // per essere valido, un pedone bianco dev'essere nella riga 5, un pedone nero nella riga 4
        int direction;
        if (color == 'B')
            direction = 1;
        else
            direction = -1;
        Move tmp;
        for (int i = 0; i < SIZE; i++)
        {
            S::Ship *pawn = piecesList[8 + i + offset];
            if (!pawn || (pawn->GetName() != 80 && pawn->GetName() != 112))
                continue;
            pair<int, int> pos = pawn->GetPosition();
            if (scanBoundaries(pos.first, pos.second - direction))
            {
                S::Ship *toTheLeft = board[pos.first][pos.second - direction];
                pair<int, int> destination = pair(pos.first + direction, pos.second - direction);
                tmp = Move(pawn, destination, 2, toTheLeft);
                if (enPassantConditions(pawn, toTheLeft) && !scanCheck(tmp, color))
                    moves.push_back(tmp);
            }
            if (scanBoundaries(pos.first, pos.second + direction))
            {
                S::Ship *toTheRight = board[pos.first][pos.second + direction];
                pair<int, int> destination = pair(pos.first + direction, pos.second + direction);
                tmp = Move(pawn, destination, 2, toTheRight);
                if (enPassantConditions(pawn, toTheRight) && !scanCheck(tmp, color))
                    moves.push_back(tmp);
            }
            if (scanBoundaries(pos.first + direction, pos.second - direction))
            {
                S::Ship *forwardLeft = board[pos.first + direction][pos.second - direction];
                pair<int, int> destination = pair(pos.first + direction, pos.second - direction);
                tmp = Move(pawn, destination, 1, forwardLeft);
                if (forwardLeft && forwardLeft->GetColor() != color && !scanCheck(tmp, color))
                    moves.push_back(tmp);
            }
            if (scanBoundaries(pos.first + direction, pos.second + direction))
            {
                S::Ship *forwardRight = board[pos.first + direction][pos.second + direction];
                pair<int, int> destination = pair(pos.first + direction, pos.second + direction);
                tmp = Move(pawn, destination, 1, forwardRight);
                if (forwardRight && forwardRight->GetColor() != color && !scanCheck(tmp, color))
                    moves.push_back(tmp);
            }
        }
    }

    void Mare::insertPiece(S::Ship *ship, pair<int, int> *pos)
    {
        board[pos->first][pos->second] = ship;
        piecesList.push_back(ship);
    }

    void Mare::initializeRow(int row)
    {
        char color;
        if (row == 0 || row == 1)
            color = 'B';
        else
            color = 'N';
        S::Ship *ship;
        if (row == 1 || row == 6)
        {
            for (int i = 0; i < SIZE; i++)
            {
                ship = new P(pair(row, i), color);
                insertPiece(ship, new pair(row, i));
            }
        }
        else if (row == 0 || row == 7)
        {
            ship = new T(pair(row, 0), color);
            insertPiece(ship, new pair(row, 0));
            ship = new C(pair(row, 1), color);
            insertPiece(ship, new pair(row, 1));
            ship = new A(pair(row, 2), color);
            insertPiece(ship, new pair(row, 2));
            ship = new D(pair(row, 3), color);
            insertPiece(ship, new pair(row, 3));
            ship = new R(pair(row, 4), color);
            insertPiece(ship, new pair(row, 4));
            ship = new A(pair(row, 5), color);
            insertPiece(ship, new pair(row, 5));
            ship = new C(pair(row, 6), color);
            insertPiece(ship, new pair(row, 6));
            ship = new T(pair(row, 7), color);
            insertPiece(ship, new pair(row, 7));
        }
    }

    void Mare::updateLog(pair<int, int> start, pair<int, int> end)
    {
        ofstream write;
        write.open(logFile, ofstream::app);
        string out;
        out += to_string(start.first) + to_string(start.second) + " ";
        out += to_string(end.first) + to_string(end.second) + "\n";
        write << out;
        write.close();
    }

    void Mare::updateLog(char newPiece)
    {
        ofstream write(logFile);
        write.open(logFile, ofstream::app);
        write << "p " << newPiece << "\n";
        write.close();
    }

    Mare::Mare(string log, string playerBlack, string playerWhite)
    {
        for (int i = 0; i < 8; i++)
            board.push_back(vector<S::Ship *>(8, nullptr));
        // inizializzare file
        initializeRow(0);
        initializeRow(1);
        initializeRow(7);
        initializeRow(6);
        lastMove = Move();
        pieceToPromote = nullptr;
        logFile = log;
        if (log != "" && playerBlack != "" && playerWhite != "")
        {
            ofstream write(logFile);
            string playerRow = "B: " + playerWhite + " N: " + playerBlack + "\n";
            write << playerRow;
            write.close();
        }
    }

    string Mare::printBoard()
    {
        string out = "";
        out += "  ---------------------------------\n";
        for (int i = 7; i >= 0; i--)
        {
            out += to_string(i + 1);
            out += " | ";
            for (int j = 0; j < 8; j++)
            {
                if (board[i][j] != nullptr)
                    out += board[i][j]->GetName();
                else
                    out += " ";
                out += " | ";
            }
            out += "\n";
            out += "  ---------------------------------\n";
        }
        out += "    A   B   C   D   E   F   G   H";
        return out;
    }

    int Mare::getCondition() { return condition; }

    int Mare::getCondition(char color)
    {
        humanPlayerMoves = movesAvailable(color);
        return condition;
    }

    vector<Mare::Move> Mare::movesAvailable(char color)
    {
        vector<Move> moves;
        int start = 0;
        if (color == 'N')
            start = SIZE * 2;
        bool initialCheck = scanCheck(color);
        for (int i = 0; i < SIZE * 2; i++)
        {
            S::Ship *ship = piecesList[start + i];
            if (!ship)
                continue;
            vector<vector<pair<int, int>>> pieceMoves = ship->Pmove();
            for (int j = 0; j < pieceMoves.size(); j++)
            {
                vector<pair<int, int>> tmp = pieceMoves[j];
                for (int k = 0; k < tmp.size(); k++)
                {
                    pair<int, int> destination = tmp[k];
                    if (!scanBoundaries(destination))
                        continue;
                    S::Ship *additionalPiece = nullptr;
                    int moveName = 0;
                    char occ = scanOccupied(destination);
                    if (occ != 0 && (ship->GetName() == 80 || ship->GetName() == 112))
                        break;
                    else if (occ == color && pieceMoves.size() > 1)
                        break;
                    else if (occ == color && pieceMoves.size() == 1)
                        continue;
                    else if (occ != color && occ != 0)
                    { // ovvero occ = colore avversario
                        moveName++;
                        additionalPiece = board[destination.first][destination.second];
                    }
                    Move move = Move(ship, destination, moveName, additionalPiece);
                    if (!scanCheck(move, color))
                        moves.push_back(Move(ship, destination, moveName, additionalPiece));
                    if (occ != 0 && pieceMoves.size() > 1)
                        break;
                }
            }
        }
        scanAddSpecialMoves(moves, color);
        if (scanCheckMate(initialCheck, moves))
            condition = 0;
        else if (scanCheck(color))
            condition = 1;
        else if (moves.size() == 0)
            condition = 2;
        else
            condition = -1;
        return moves;
    }

    bool Mare::performMove(Move move)
    {
        pieceToPromote = nullptr;
        S::Ship *ship = move.ship;
        pair<int, int> start = ship->GetPosition();
        pair<int, int> destination = move.destination;
        ship->SetMove(destination);
        board[destination.first][destination.second] = ship;
        board[start.first][start.second] = nullptr;
        switch (move.moveName)
        {
        case 0:
            break;
        case 3:
        {                                         // arrocco corto
            S::Ship *tower = move.additionalPiece; // colonna attuale: 7, col. destinazione: 5
            pair<int, int> pos = tower->GetPosition();
            tower->SetMove(pair(pos.first, pos.second - 2));
            board[pos.first][pos.second] = nullptr;
            board[pos.first][pos.second - 2] = tower;
            break;
        }
        case 4:
        {                                         // arrocco lungo
            S::Ship *tower = move.additionalPiece; // colonna attuale: 0, col. destinazione: 4
            pair<int, int> pos = tower->GetPosition();
            tower->SetMove(pair(pos.first, pos.second + 3));
            board[pos.first][pos.second] = nullptr;
            board[pos.first][pos.second + 3] = tower;
            break;
        }
        default: // mossa normale con cattura, en passant
            S::Ship *additionalPiece = move.additionalPiece;
            *(find(piecesList.begin(), piecesList.end(), additionalPiece)) = nullptr;
            delete additionalPiece;
            break;
        }
        lastMove = move;
        if (logFile != "")
            updateLog(start, destination);
        if (scanPromotion(ship))
        {
            pieceToPromote = ship;
            return true;
        }
        return false;
    }

    bool Mare::performMove(pair<int, int> start, pair<int, int> destination, char color)
    {
        if (!(legitMoveInput(start) && legitMoveInput(start)))
            throw InvalidInputException();
        S::Ship *ship = board[start.first][start.second];
        Move tmpMove = Move(ship, destination, 0, nullptr);
        auto result = find(humanPlayerMoves.begin(), humanPlayerMoves.end(), tmpMove);
        if (result == humanPlayerMoves.end())
            throw InvalidMoveException();
        return performMove(*result);
    }

    void Mare::performPromotion(char code)
    {
        pair<int, int> pos = pieceToPromote->GetPosition();
        char color = pieceToPromote->GetColor();
        int moves = pieceToPromote->GetStatus();
        S::Ship *newPiece;
        switch (code)
        {
        case 'A': // alfiere
            newPiece = new A(pos, color, moves);
            break;
        case 'C': // cavallo
            newPiece = new C(pos, color, moves);
            break;
        case 'D': // regina
            newPiece = new D(pos, color, moves);
            break;
        case 'T': // torre
            newPiece = new T(pos, color, moves);
            break;
        default:
            throw InvalidInputException();
        }
        board[pos.first][pos.second] = newPiece;
        *(find(piecesList.begin(), piecesList.end(), pieceToPromote)) = newPiece;
        delete pieceToPromote;
        if (logFile != "")
            updateLog(code);
    }

    void Mare::justForDebug(string fileName)
    {
        for (int i = 0; i < 32; i++)
            delete piecesList[i];
        piecesList = vector<S::Ship *>(32, nullptr);
        board = vector<vector<S::Ship *>>(8, vector<S::Ship *>(8, nullptr));
        vector<S::Ship *> blackPawns;
        vector<S::Ship *> whitePawns;
        string line;
        ifstream reader(fileName);
        S::Ship *ship;
        for (int i = 7; i >= 0; i--)
        {
            getline(reader, line);
            for (int j = 0; j < 8; j++)
            {
                char character = line[j];
                if (character != '-')
                {
                    char color = 'N';
                    int index = 0;
                    if (character > 90)
                    {
                        character -= 32;
                        color = 'B';
                    }
                    if (color == 'N')
                        index += 16;
                    switch (character)
                    {
                    case 'A':
                        ship = new A(pair(i, j), color);
                        if (!piecesList[index + 2])
                            index += 2;
                        else if (!piecesList[index + 5])
                            index += 5;
                        else
                            index = -1;
                        break;
                    case 'C':
                        ship = new C(pair(i, j), color);
                        if (!piecesList[index + 1])
                            index += 1;
                        else if (!piecesList[index + 6])
                            index += 6;
                        else
                            index = -1;
                        break;
                    case 'D':
                        ship = new D(pair(i, j), color);
                        if (!piecesList[index + 3])
                            index += 3;
                        else
                            index = -1;
                        break;
                    case 'P':
                        ship = new P(pair(i, j), color);
                        index = -1;
                        break;
                    case 'R':
                        ship = new R(pair(i, j), color);
                        index += 4;
                        break;
                    case 'T':
                        ship = new T(pair(i, j), color);
                        if ((color == 'N' && i == 7 || color == 'B' && i == 0) && j == 0)
                            index += 0;
                        else if ((color == 'N' && i == 7 || color == 'B' && i == 0) && j == 7)
                            index += 7;
                        else if (!piecesList[index])
                            index += 0;
                        else if (!piecesList[index + 7])
                            index += 7;
                        else
                            index = -1;
                        break;
                    }
                    board[i][j] = ship;
                    if (index == -1)
                    {
                        if (color == 'N')
                            blackPawns.push_back(ship);
                        else
                            whitePawns.push_back(ship);
                    }
                    else
                        piecesList[index] = ship;
                }
            }
        }
        int startIndex = 8;
        for (int i = 0; i < whitePawns.size(); i++)
        {
            piecesList[startIndex + i] = whitePawns[i];
        }
        startIndex = 24;
        for (int i = 0; i < blackPawns.size(); i++)
        {
            piecesList[startIndex + i] = blackPawns[i];
        }
    }
}