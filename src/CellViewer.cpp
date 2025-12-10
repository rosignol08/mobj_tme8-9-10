#include "CellViewer.h"
#include "OpenCellDialog.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QKeySequence>
#include <QString>

namespace Netlist{
    CellViewer::CellViewer (QWidget* parent)
  :QMainWindow(parent)
  //, cellWidget(parent)
  ,cellWidget_(NULL)
  //  cellsLib_(),
  //  instancesWidget_(),
  ,saveCellDialog_()
  {
    cellWidget_ = new CellWidget();
    saveCellDialog_ = new SaveCellDialog(this);

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
  }

  CellViewer::~CellViewer () {}

  Cell* CellViewer::getCell () const {
    return cellWidget_->getCell();
  }

  void CellViewer::setCell (Cell* cell) {
    //std::cout << "set cell" << std::endl;
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
      Cell* cell = cell->load(cellName.toStdString());
      //std::cout << cell->getName() << std::endl;
      if(cell != NULL){
        //std::cout << "pas null" << std::endl;
        setCell(cell);
      }
    }
  }

}
