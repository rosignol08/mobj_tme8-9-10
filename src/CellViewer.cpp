#include "CellViewer.h"
#include "CellsLib.h"
//#include "CellWidget.h"
//#include "SaveCellDialog.h"
#include "InstancesWidget.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QKeySequence>
#include <QString>

namespace Netlist{
    CellViewer::CellViewer (QWidget* parent)
  :QMainWindow(parent)
  ,cellWidget_(NULL)
  ,cellsLib_(NULL)
  ,instancesWidget_(NULL)
  ,saveCellDialog_(NULL)
  {
    cellWidget_ = new CellWidget();
    saveCellDialog_ = new SaveCellDialog(this);
    instancesWidget_ = new InstancesWidget(cellWidget_);
    cellsLib_ = new CellsLib(this); //faut utiliser this enfaite 

    instancesWidget_->setCellViewer(this);
    cellsLib_->setCellViewer(this);

    setCentralWidget(cellWidget_);
    QMenu* fileMenu = menuBar()->addMenu("&File");
  
    QAction * action = new QAction ("&Save As", this);
    action->setStatusTip("Save to disk (rename) the Cell");
    action->setShortcut(QKeySequence("CTRL+S"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(saveCell()));

    action = new QAction ("&Open", this);
    action->setStatusTip("Load to disk (rename) the Cell");
    action->setShortcut(QKeySequence("CTRL+O"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(openCell()));

    action = new QAction("&Quit", this);
    action->setStatusTip("Exit the Netlist Viewer");
    action->setShortcut(QKeySequence("CTRL+Q"));
    action->setVisible(true);
    fileMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(close()));
  
    showInstancesWidget();
    showCellsLib();
  }

  CellViewer::~CellViewer () {}

  Cell* CellViewer::getCell () const {
    return cellWidget_->getCell();
  }

  void CellViewer::setCell (Cell* cell) {
    cellWidget_->setCell(cell);
  }

  void CellViewer::saveCell (){
    Cell* cell = getCell();
    if (cell==NULL) return;

    QString cellName = cell->getName().c_str();
  
    if (saveCellDialog_->run(cellName)){
      cell->setName(cellName.toStdString());
      cell->save(cellName.toStdString());
    } 
  }

  void CellViewer::openCell (){
    QString cellName;
    if(OpenCellDialog::run(cellName)){
      Cell* cell = Cell::load(cellName.toStdString());
      if(cell != NULL){
        setCell(cell);
        emit cellLoaded(); //le signal 
      }
    }
  }

  void CellViewer::showCellsLib ()
  {
    cellsLib_->show(); //TODO voir si ça a vraiment un effet
    return;
  }

  void CellViewer::showInstancesWidget ()
  {
    instancesWidget_->show(); //TODO pareil voir si ça a vraiment un effet
    return;
  }

}