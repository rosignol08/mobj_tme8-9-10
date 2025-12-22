#include <QHeaderView>
#include <QTableView>
#include <QWidget>
#include <QVBoxLayout>
#include "InstancesWidget.h"
#include "InstancesModel.h"
#include "CellViewer.h"

namespace Netlist
{

    InstancesWidget::InstancesWidget(QWidget *parent)
        : QWidget(parent), cellViewer_(NULL), baseModel_(new InstancesModel(this)), view_(new QTableView(this)), load_(new QPushButton(this))
    {
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_DeleteOnClose, false);
        setContextMenuPolicy(Qt::ActionsContextMenu);

        view_->setShowGrid(false);
        view_->setAlternatingRowColors(true);
        view_->setSelectionBehavior(QAbstractItemView::SelectRows);
        view_->setSelectionMode(QAbstractItemView::SingleSelection);
        view_->setSortingEnabled(true);
        view_->setModel(baseModel_); //associe le modèle TODO etre sur que c'est le model actuel
        std::cout << "modele associé :" << baseModel_ << std::endl;

        QHeaderView *horizontalHeader = view_->horizontalHeader();
        horizontalHeader->setDefaultAlignment(Qt::AlignHCenter);
        horizontalHeader->setMinimumSectionSize(300);
        horizontalHeader->setStretchLastSection(true);

        QHeaderView *verticalHeader = view_->verticalHeader();
        verticalHeader->setVisible(false);

        load_->setText("Load");
        connect(load_, SIGNAL(clicked()), this, SLOT(load()));

        //pour positionner les widgets
        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(view_);
        layout->addWidget(load_);
        setLayout(layout);

        //config de la fenêtre
        setWindowTitle("Instances");
        resize(400, 300);
    }

    InstancesWidget::~InstancesWidget() {}

    void InstancesWidget::setCellViewer(CellViewer *cellViewer)
    {
        cellViewer_ = cellViewer;
        if (cellViewer_) {
            connect(cellViewer_, SIGNAL(cellLoaded()), this, SLOT(updateInstances())); //pour mettre a jour la liste des instances quand on change de cell
        }
    }

    int InstancesWidget::getSelectedRow() const
    {
        QModelIndexList selecteds = view_->selectionModel()->selection().indexes();
        if (selecteds.empty())
            return -1;
        return selecteds.first().row();
    }

    void InstancesWidget::load()
    {
        int selectedRow = getSelectedRow();
        std::cout << "Selected row = " << selectedRow << std::endl;

        if (selectedRow < 0)
            return;
        cellViewer_->setCell(baseModel_->getModel(selectedRow));
    }

    void InstancesWidget::setCell(Cell *cell)
    {
        cellViewer_->setCell(cell);
        baseModel_->setCell(cell);
    }

    void InstancesWidget::updateInstances()
    {
        if (cellViewer_) {
            Cell* cell = cellViewer_->getCell();
            baseModel_->setCell(cell);
        }
    }
}
