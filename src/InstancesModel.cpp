#include "InstancesModel.h"
#include "Cell.h"
#include "Instance.h"

namespace Netlist{
    InstancesModel::InstancesModel(InstancesWidget* parent)
      : QAbstractTableModel(parent)
      , cell_(nullptr) {

      }//string nom_instance, string nom_model
    Instance InstancesModel::getModel(){
        //TODO checker si ça marche
        return cell_->getInstances()[row];
    }
    void InstancesModel::setCell(Cell * cell){
        this->cell_ = cell;
    }
    int QAbstractItemModel::rowCount(const QModelIndex& parent) const{
        //return cell_->getInstances().size();
        if (cell_ == nullptr) return 0;
        return static_cast<int>(cell_->getInstances().size());//TODO checker si ça marche
    }
    int QAbstractItemModel::columnCount(const QModelIndex& parent) const{
        return 2; //psk 2 collones
    }
    QVariant QAbstractItemModel::data(const QModelIndex& parent, int) const{
        //le nom de la cell ou de la master cell
        if (not parent.isValid()) return QVariant();
        if (parent.row() >= (int)cell_->getInstances().size()) return QVariant();
        
        Instance* instance = cell_->getInstances()[parent.row()];
        
        if (parent.column() == 0) {
            return QString::fromStdString(instance->getName());
        } else if (parent.column() == 1) {
            return QString::fromStdString(instance->getMasterCell()->getName());
        }
        
        return QVariant();
    }
}