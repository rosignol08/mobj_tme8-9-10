#include <QWidget>
#include <QTableView>
#include <QAbstractTableModel>
#include <QPushButton>
#include <QAbstractItemView>
#include "Cell.h"

namespace Netlist{
  class CellViewer;
  class InstancesModel;

  class InstancesWidget : public QWidget {
    Q_OBJECT;

    public:
                  InstancesWidget (QWidget* parent=NULL);
virtual          ~InstancesWidget ();
          void    setCellViewer   (CellViewer* );
          int     getSelectedRow  () const;
          void    setCell         (Cell*);

    public slots:
      void  load            ();

    private:
      CellViewer*     cellViewer_;
      InstancesModel* baseModel_;
      QTableView*     view_;
      QPushButton*    load_;
  };
}
