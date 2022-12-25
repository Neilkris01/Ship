#include "Sde.hpp"

namespace S
{
    Sde::Sde(std::pair<std::pair<int, int>, std::pair<int, int>> pos_)
    {
        this -> structure = {'S', 'S', 'S'};
        this -> tipo = 'S';
        this -> pos = pos_;
        this -> dimensione = 3;
        this -> corazza = 3;
        this -> pMedio;
        this -> state;
    }

    void Sde::setStructure(std::vector<char> structure_)
    {this -> structure = structure_;}
    void Sde::setTipo(char tipo_)
    {this -> tipo = tipo_;}
    void Sde::setPos(std::pair<std::pair<int, int>, std::pair<int, int>> pos_)
    {this -> pos = pos_;}
    void Sde::setPMedio(std::pair<char, int> pMedio_)
    {this -> pMedio = pMedio_;}
    void Sde::setDimensione(int dimensione_)
    {this -> dimensione = dimensione_;}
    void Sde::setCorazza(int corazza_)
    {this -> corazza = corazza_;}

    std::vector<char> Sde::getStructure() const
    {return this -> structure;}
    char Sde::getTipo() const
    {return this -> tipo;}
    std::pair<std::pair<int, int>, std::pair<int, int>> Sde::getPos() const
    {return this -> pos;}
    std::pair<int, int> Sde::getPMedio() const
    {return this -> pMedio;}
    int Sde::getDimensione() const
    {return this -> dimensione;}
    int Sde::getCorazza() const
    {return this -> corazza;}

    //0 verticale, 1 se orizzontale
    int Sde::Direction(std::pair<std::pair<int, int>, std::pair<int, int>>  pos)
    {
        if(pos.first.second == pos.second.second)
        return 0;
        else
        return 1;
    }

    //possibili posizioni del pMedio della Sde
    std::vector<std::vector<std::pair<int, int> >> Sde::Moves()
    {
        std::vector<std::vector<std::pair<int, int> >> Moves;
        int y = 1;
        int x = 1;
        std::vector<std::pair<int, int> > m1;
        while (x <12)
        {
            while (y < 12)
            {
                m1.push_back(std::pair<int, int>(x, y));
                y++;
            }
            x++;
        }
        Moves.push_back(m1);
    }

    void Sde::Damage()
    {
       corazza = corazza - 1;
    }
}
