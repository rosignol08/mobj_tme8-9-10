//table contenant la liste des Cell actuellement présentes en mémoire
#include <QAbstractTableModel>
#include "Cell.h"

namespace Netlist{
    class CellsModel : public QAbstractTableModel{
        Q_OBJECT;
        public:
                      CellsModel    (QObject* parent = NULL);
                     ~CellsModel    ();
            Cell*     getModel      (int row);
            int       rowCount      (const QModelIndex& parent=QModelIndex()) const;
            int       columnCount   (const QModelIndex& parent=QModelIndex()) const;
            QVariant  data          (const QModelIndex& index, int role = Qt::DisplayRole) const;
            QVariant  headerData    (int section
                                    , Qt::Orientation Orientation
                                    , int role=Qt::DisplayRole) const;
        public slots:                    
            void updateData();
    };
}