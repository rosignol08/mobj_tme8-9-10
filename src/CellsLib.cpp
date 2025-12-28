#include <QHeaderView>
#include <QTableView>
#include <QWidget>
#include <QVBoxLayout>
#include "CellsLib.h"
#include "CellViewer.h"
#include "CellsModel.h"
//test: si on veut ajouter des instances
#include "Instance.h"
#include "Point.h"


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
    //test: si on veut ajouter des instances double click TODO marche pas trop
    
    // Double-clic pour ajouter comme instance
    connect(view_, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(addAsInstance()));
    
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
  //test: si on veut ajouter des instances
  void CellsLib::addAsInstance()//TODO regarde si ça te plait
  {
    int selectedRow = getSelectedRow();
    if(selectedRow < 0) return;
    
    Cell* currentCell = cellViewer_->getCell();
    if(!currentCell) {
      std::cerr << "[ERROR] No cell loaded to add instance to" << std::endl;
      return;
    }
    
    Cell* masterCell = baseModel_->getModel(selectedRow);
    if(!masterCell) return;
    
    //gen un nom pour l'instance
    static int instanceCounter = 0;
    std::string instanceName = masterCell->getName() + "_" + std::to_string(instanceCounter++);
    
    //calcul de position avec offset pour pas de chevauchement
    int offsetX = (instanceCounter % 5) * 100;  //5 colonnes
    int offsetY = (instanceCounter / 5) * 100;  //new ligne tout les 5
    
    //creat de l'instance
    Instance* newInstance = new Instance(currentCell, masterCell, instanceName);
    newInstance->setPosition(Point(offsetX, offsetY));
    
    std::cout << "Instance <" << instanceName << "> of <" << masterCell->getName() 
              << "> added to <" << currentCell->getName() << "> at position (" 
              << offsetX << ", " << offsetY << ")" << std::endl;
    
    //updata l'affichage
    cellViewer_->setCell(currentCell);
  }
}