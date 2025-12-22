#include <QHeaderView>
#include <QTableView>
#include <QWidget>
#include <QVBoxLayout>
#include "CellsLib.h"
#include "CellViewer.h"
#include "CellsModel.h"


namespace Netlist {
  CellsLib::CellsLib (QWidget* parent)
  :QWidget (parent)
  ,cellViewer_(NULL)
  ,baseModel_(new CellsModel(this)) //(new CellsModel(this)) getBaseModel()
  ,view_(new QTableView(this))
  ,load_(new QPushButton(this)) 
  {
    //config de la fenêtre
    setWindowTitle("Cells Library");
    resize(300, 400);
    
    //config de la vue
    view_->setShowGrid(false);
    view_->setAlternatingRowColors(true);
    view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    view_->setSelectionMode(QAbstractItemView::SingleSelection);
    view_->setModel(baseModel_);
    
    //config du bouton
    load_->setText("Load");
    connect(load_, SIGNAL(clicked()), this, SLOT(load()));
    
    //pour positionner les widgets
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(view_);
    layout->addWidget(load_);
    setLayout(layout);
    
    //maj initiale
    baseModel_->updateData();
  }

  CellsLib::~CellsLib()
  {}

  void CellsLib::setCellViewer  (CellViewer* cellviewer)
  {
    cellViewer_ = cellviewer;
    if(cellViewer_ && baseModel_) {
      connect(cellViewer_, SIGNAL(cellLoaded()), baseModel_, SLOT(updateData()));
      //debug
      std::cout << "CellsLib::setCellViewer: connected cellLoaded signal to baseModel_ updateData slot" << std::endl;
      baseModel_->updateData();
      std::cout << "cellule chargée : " << baseModel_->getModel(0)->getName() << std::endl;
    }
  }

  int CellsLib::getSelectedRow () const
  {
    QModelIndexList selecteds = view_->selectionModel()->selection().indexes();
    if (selecteds.empty()) return -1;
    return selecteds.first().row();
  }

  void CellsLib::load()
  {
    int selectedRow = getSelectedRow();
  
    if(selectedRow < 0) return;
    cellViewer_->setCell(baseModel_->getModel(selectedRow));
  }
}