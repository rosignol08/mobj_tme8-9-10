#include "CellsLib.h"
namespace Netlist{
    CellsLib       ( QWidget* parent=NULL ):QWidget (parent), CellViewer*  cellViewer_(NULL),
    CellsModel*  baseModel_(NULL),
    QTableView*  view_(NULL),
    QPushButton* load_(NULL) {};

    void setCellViewer  ( CellViewer* );
    //comme pour elle
    /* QModelIndexList selecteds = view_->selectionModel()->selection().indexes();
    if (selected.empty()) return -1;
    return selecteds.first().row();
    */
    int getSelectedRow () const{
        return 
    }

    inline CellsModel* getBaseModel   (){
        return this->basemodel_;
    }

}