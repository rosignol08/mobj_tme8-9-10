
//Fenêtre affichant la liste des instances et leurs modèles
/*
Le modèle associé à la QTableView (attribut view_) de type InstancesModel (attribut baseModel_) présentera deux colonnes:

Le nom de l'instance.   
Le nom du modèle de l'instance (le nom de la master cell).
Ne pas oublier de fournir le nom des colonnes (headers).
*/
//#include "CellViewer.h"
#include <QAbstractTableModel>

namespace Netlist{
    class InstancesWidget;
    class Cell;
    class Instance;

    class InstancesModel : public QAbstractTableModel {
        //Q_OBJECT;
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
