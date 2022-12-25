#include "Corazzata.hpp"

namespace S
{
    Corazzata::Corazzata(std::pair<std::pair<int, int>, std::pair<int, int>> pos_)
    {
        this -> structure = {'C', 'C', 'C', 'C', 'C'};
        this -> tipo = 'C';
        this -> pos = pos_;
        this -> dimensione = 5;
        this -> corazza = 5;
        this -> pMedio;
        this -> state;
    }

    void Corazzata::setStructure(std::vector<char> structure_)
    {this -> structure = structure_;}
    void Corazzata::setTipo(char tipo_)
    {this -> tipo = tipo_;}
    void Corazzata::setPos(std::pair<std::pair<int, int>, std::pair<int, int>> pos_)
    {this -> pos = pos_;}
    void Corazzata::setPMedio(std::pair<char, int> pMedio_)
    {this -> pMedio = pMedio_;}
    void Corazzata::setDimensione(int dimensione_)
    {this -> dimensione = dimensione_;}
    void Corazzata::setCorazza(int corazza_)
    {this -> corazza = corazza_;}

    std::vector<char> Corazzata::getStructure() const
    {return this -> structure;}
    char Corazzata::getTipo() const
    {return this -> tipo;}
    std::pair<std::pair<int, int>, std::pair<int, int>> Corazzata::getPos() const
    {return this -> pos;}
    std::pair<int, int> Corazzata::getPMedio() const
    {return this -> pMedio;}
    int Corazzata::getDimensione() const
    {return this -> dimensione;}
    int Corazzata::getCorazza() const
    {return this -> corazza;}

    void Corazzata::Damage()
    {
       corazza = corazza - 1;
    }

    void Corazzata::Damage()
    {
       corazza = 5;
    }
}
