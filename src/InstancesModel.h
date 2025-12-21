#include <QAbstractTableModel>

namespace Netlist{
    class InstancesWidget;
    class Cell;
    class Instance;

    class InstancesModel : public QAbstractTableModel {
        Q_OBJECT;
        public:
                        InstancesModel  (QObject* parent = NULL);
                       ~InstancesModel  ();
            void        setCell         (Cell*);
            Cell*       getModel        (int row);
            int         rowCount        (const QModelIndex& parent=QModelIndex()) const;
            int         columnCount     (const QModelIndex& parent=QModelIndex()) const;
            QVariant    data            (const QModelIndex& index, int role = Qt::DisplayRole) const;
            QVariant    headerData      (int section
                                        , Qt::Orientation Orientation
                                        , int role=Qt::DisplayRole) const;
        private:
            Cell * cell_;
      };


}
