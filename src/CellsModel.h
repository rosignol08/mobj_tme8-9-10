//table contenant la liste des Cell actuellement présentes en mémoire
#include <vector>
#include "Cell.h"
namespace Netlist{
    class CellsModel : public QAbstractTableModel{
        public:
            std::vector<Cell> liste_modeles;
            void updateData();
            bool layoutChanged();
    };
}