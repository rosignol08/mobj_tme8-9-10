#include <QHeaderView>
#include <QTableView>
#include <QWidget>
#include "CellsLib.h"
#include "CellViewer.h"
#include "CellsModel.h"


namespace Netlist {
  CellsLib::CellsLib (QWidget* parent)
  :QWidget (parent)
  ,cellViewer_(NULL)
  ,baseModel_(new CellsModel(this))
  ,view_(new QTableView(this))
  ,load_(new QPushButton(this)) 
  {
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, false);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    view_->setShowGrid(false);
    view_->setAlternatingRowColors(true);
    view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    view_->setSelectionMode(QAbstractItemView::SingleSelection);
    view_->setSortingEnabled(true);
    view_->setModel(baseModel_);
  
    QHeaderView* horizontalHeader = view_->horizontalHeader();
    horizontalHeader->setDefaultAlignment(Qt::AlignHCenter);
    horizontalHeader->setMinimumSectionSize(300);
    horizontalHeader->setStretchLastSection(true);

    QHeaderView* verticalHeader = view_->verticalHeader();
    verticalHeader->setVisible(false);
  
    load_->setText("Load");
    connect(load_, SIGNAL(clicked()), this, SLOT(load()));
    connect(cellViewer_, SIGNAL(cellViewer_->cellLoaded()), SLOT(baseModel_->updateData()));
  }

  CellsLib::~CellsLib()
  {}

  void CellsLib::setCellViewer  (CellViewer* cellviewer)
  {
    cellViewer_ = cellviewer;
  }

  //comme pour elle
  /* QModelIndexList selecteds = view_->selectionModel()->selection().indexes();
  if (selected.empty()) return -1;
  return selecteds.first().row();
  */
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