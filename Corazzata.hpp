#ifndef CORAZZATA_HPP
#define CORAZZATA_HPP

#include "Ship.hpp"
#include <iostream>

namespace S
{
    class Corazzata : public Ship
    {
        private:

        std::vector<char> structure;
        char tipo;
        std::pair<std::pair<int, int>, std::pair<int, int>> pos;
        std::pair<int, int> pMedio;
        int dimensione;
        int corazza;
        bool state;

        public:

        Corazzata(std::pair<std::pair<int, int>, std::pair<int, int>> pos);

        void setStructure(std::vector<char> structure_);
        void setTipo(char tipo);
        void setPos(std::pair<std::pair<int, int>, std::pair<int, int>> pos_);
        void setPMedio(std::pair<char, int> pMedio_);
        void setDimensione(int dimensione_);
        void setCorazza(int corazza_);

        std::vector<char> getStructure() const;
        char getTipo() const;
        std::pair<std::pair<int, int>, std::pair<int, int>> getPos() const;
        std::pair<int, int> getPMedio() const;
        int getDimensione() const;
        int getCorazza() const;

        void Damage();

        void Heal();
    };
}
#endif