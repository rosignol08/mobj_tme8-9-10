#include "InstancesModel.h"
#include "Cell.h"
#include "Instance.h"

namespace Netlist{
    InstancesModel::InstancesModel(InstancesWidget* parent)
      : QAbstractTableModel(parent)
      , cell_(nullptr) {

      }//string nom_instance, string nom_model
    Instance InstancesModel::getModel(); //TODO à voir
    void InstancesModel::setCell(Cell * cell){
        this->cell_ = cell;
    }
    int QAbstractItemModel::rowCount(const QModelIndex& parent) const{
        return cell_->getInstances().size();
    }
    int QAbstractItemModel::columnCount(const QModelIndex& parent) const{
        return 2; //psk 2 collones
    }
    QVariant QAbstractItemModel::data(const QModelIndex& parent, int) const{
        //le nom de la cell ou de la master cell
        std::string = cell_->getInstances.name();
        //ou
        std::string = cell_->getOwner().name();
    }
}