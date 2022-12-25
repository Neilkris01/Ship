#ifndef SHIP_HPP
#define SHIP_HPP

#include <iostream>
#include <vector>

namespace S
{
    class Ship
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

        Ship();
        Ship(std::pair<std::pair<int, int>, std::pair<int, int>> pos);

        void setStructure(std::vector<char> structure_);
        void setTipo(char tipo);
        void setPos(std::pair<std::pair<int, int>, std::pair<int, int>> pos_);
        void setPMedio(std::pair<int, int> pMedio_);
        void setDimensione(int dimensione_);
        void setCorazza(int corazza_);

        std::vector<char> getStructure() const;
        char getTipo() const;
        std::pair<std::pair<char, int>, std::pair<int, int>> getPos() const;
        std::pair<int, int> getPMedio() const;
        int getDimensione() const;
        int getCorazza() const;

        int Ship::Direction(std::pair<std::pair<int, int>, std::pair<int, int>>  pos);

        virtual std::vector<std::vector<std::pair<int, int>>> Moves() = 0;

        void Damage();

        void Heal();
    };
}
#endif