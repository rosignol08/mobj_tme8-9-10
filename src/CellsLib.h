#include <QWidget>
#include <QTableView>
#include <QPushButton>

namespace Netlist
{
    class CellViewer;
    class CellsModel;

    class CellsLib : public QWidget
        {
            Q_OBJECT;

        public:
            CellsLib(QWidget *parent = NULL);
           ~CellsLib();
            void setCellViewer(CellViewer *);
            int getSelectedRow() const;
            inline CellsModel *getBaseModel();
        public slots:
            void load();
            void addAsInstance(); //ajout comme si c une instance a cellule actuelle test: si on veut ajouter des instances

        private:
            CellViewer *cellViewer_;
            CellsModel *baseModel_;
            QTableView *view_;
            QPushButton *load_;
        };

        CellsModel* CellsLib::getBaseModel () { return baseModel_;};
    }
