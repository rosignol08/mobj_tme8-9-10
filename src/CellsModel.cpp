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
    Cell::getAllCells()[row];
  }

  int CellsModel::rowCount(const QModelIndex& parent) const
  {
    return Cell::getAllCells().size();
  }

  int CellsModel::columnCount(const QModelIndex& parent) const
  { return 1; }

  QVariant CellsModel::data(const QModelIndex& index, int role) const
  {
    if (index.isValid() && role == Qt::DisplayRole){
      int row = index.row();
      return Cell::getAllCells()[row]->getName().c_str();
    }
    return QVariant();
  }

  QVariant CellsModel::headerData(int section, Qt::Orientation orientation, int role) const
  {
    if (orientation == Qt::Vertical) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();
    return "Cell";
  }

  void CellsModel::updateData()
  {
    
  }
}