#include "InstancesModel.h"
#include "Cell.h"
#include "Instance.h"

namespace Netlist{
    InstancesModel::InstancesModel(QObject* parent)
      :QAbstractTableModel(parent)
      ,cell_(NULL) 
    { }//string nom_instance, string nom_model

    InstancesModel::~InstancesModel()
    { }

    void InstancesModel::setCell(Cell * cell){
      emit layoutAboutToBeChanged();  
      cell_ = cell;
      emit layoutChanged();
    }

    Cell* InstancesModel::getModel(int row)
    {
      if (not cell_) return NULL;
      if (row >= (int)cell_->getInstances().size()) return NULL;
      return cell_->getInstances()[row]->getMasterCell();
    }

    int InstancesModel::rowCount(const QModelIndex& parent) const{
      // if (cell_ == nullptr) return 0;
      // return static_cast<int>(cell_->getInstances().size());//TODO checker si ça marche
      return (cell_) ? cell_->getInstances().size() : 0;
    }

    int InstancesModel::columnCount(const QModelIndex& parent) const
    { return 2; }

    QVariant InstancesModel::data(const QModelIndex& index, int role) const
    {
      if (not cell_ or not index.isValid()) return QVariant();
      if (role == Qt::DisplayRole){
        int row = index.row();
        switch (index.column()){
          case 0: return cell_->getInstances()[row]->getName().c_str();
          case 1: return cell_->getInstances()[row]
                              ->getMasterCell()->getName().c_str();
        }
      }
      return QVariant();
      //le nom de la cell ou de la master cell
      // if (not parent.isValid()) return QVariant();
      // if (parent.row() >= (int)cell_->getInstances().size()) return QVariant();
      // Instance* instance = cell_->getInstances()[parent.row()];
      // if (parent.column() == 0) {
      //     return QString::fromStdString(instance->getName());
      // } else if (parent.column() == 1) {
      //     return QString::fromStdString(instance->getMasterCell()->getName());
      // }
      // return QVariant();
    }

    QVariant InstancesModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
      if (orientation == Qt::Vertical) return QVariant();
      if (role != Qt::DisplayRole) return QVariant();
    
      switch(section){
        case 0: return "Instance";
        case 1: return "MasterCell";
      }
      return QVariant();
    }
}