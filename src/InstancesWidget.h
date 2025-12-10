#pragma once
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QAbstractItemView>
#include "InstancesModel.h"
#include "Cell.h"

namespace Netlist{
    class CellViewer;
    class InstancesWidget : public QWidget {
    Q_OBJECT;

    public:
                    InstancesWidget ( QWidget* parent=NULL);
              void  setCellViewer   ( CellViewer* );
              int   getSelectedRow  () const;
              void  setCell         ( Cell* );

    public slots:
              void  load            ();

    private:
              CellViewer*     cellViewer_;
              InstancesModel* baseModel_;
              QTableView*     view_;
              QPushButton*    load_;
  };
}
