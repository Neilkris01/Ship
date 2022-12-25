#include "Ship.hpp"
#include "Mare.hpp"
namespace S
{

    Ship::Ship()
    {}
    Ship::Ship(std::pair<std::pair<int, int>, std::pair<int, int>> pos_)
    {
        this -> structure;
        this -> tipo;
        this -> pos = pos_;
        this-> dimensione;
        this -> corazza;
        this -> pMedio;
        this -> state;
    }

    void Ship::setStructure(std::vector<char> structure_)
    {this -> structure = structure_;}
    void Ship::setTipo(char tipo_)
    {this -> tipo = tipo_;}
    void Ship::setPos(std::pair<std::pair<int, int>, std::pair<int, int>> pos_)
    {this -> pos = pos_;}
    void Ship::setPMedio(std::pair<int, int> pMedio_)
    {this -> pMedio = pMedio_;}
    void Ship::setDimensione(int dimensione_)
    {this -> dimensione = dimensione_;}
    void Ship::setCorazza(int corazza_)
    {this -> corazza = corazza_;}

    std::vector<char> Ship::getStructure() const
    {return this -> structure;}
    char Ship::getTipo() const
    {return this -> tipo;}
    std::pair<std::pair<char, int>, std::pair<int, int>> Ship::getPos() const
    {return this -> pos;}
    std::pair<int, int> Ship::getPMedio() const
    {return this -> pMedio;}
    int Ship::getDimensione() const
    {return this -> dimensione;}
    int Ship::getCorazza() const
    {return this -> corazza;}

    //0 verticale, 1 se orizzontale
    int Ship::Direction(std::pair<std::pair<int, int>, std::pair<int, int>>  pos)
    {}

    //possibili posizioni del pMedio della Nds
    std::vector<std::vector<std::pair<int, int> >> Ship::Moves()
    {}

    void Ship::Damage()
    {}

    void Heal()
    {}
}