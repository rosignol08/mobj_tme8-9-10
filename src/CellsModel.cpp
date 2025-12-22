#include "CellsModel.h"
#include "Cell.h"

namespace Netlist {
  CellsModel::CellsModel(QObject* parent)
    :QAbstractTableModel(parent)
  {}

  CellsModel::~CellsModel ()
  {}

  Cell* CellsModel::getModel(int row)
  {
    if (row < 0 || row >= (int)liste_modeles.size()) return NULL;
    return liste_modeles[row];
  }

  int CellsModel::rowCount(const QModelIndex& parent) const
  {
    return liste_modeles.size();
  }

  int CellsModel::columnCount(const QModelIndex& parent) const
  { return 1; }

  QVariant CellsModel::data(const QModelIndex& index, int role) const
  {
    if (index.isValid() && role == Qt::DisplayRole){
      int row = index.row();
      return liste_modeles[row]->getName().c_str();
    }
    return QVariant();
  }

  QVariant CellsModel::headerData(int section, Qt::Orientation orientation, int role) const
  {
    if (orientation == Qt::Vertical) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();
    return "Cell";
  }

  void CellsModel::updateData(){
    
    //met à jour la liste des modèles de cellules
        beginResetModel();//pr dire que ça change
        liste_modeles.clear();//le vecteur de models on le vide ici mec
        //copie des pointeurs dans le vecteur
        const std::vector<Cell*>& modeles = Cell::getAllCells();
        
        
        for (Cell* cell : modeles) {
            if(cell != nullptr){
                liste_modeles.push_back(cell);//stocke le pointeur
            }
        }
        //TODO voir si on a vrament besoin d'une boucle pour ça
        endResetModel();
        std::cout << "mise à jour faite : " << liste_modeles.size() << " modèles disponibles." << std::endl;
        //envoie du signal que les données ont changé
        //emit dataChanged(index(0,0), index(rowCount()-1, columnCount()-1));
        return;
    }
}