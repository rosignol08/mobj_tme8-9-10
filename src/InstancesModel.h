
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
        private:
            Cell * cell_; //TODO mettre une etoile ou pas
            int row = 0;
        public:
            //string
            InstancesModel(InstancesWidget * parent); //string nom_instance, string nom_model
            Instance getModel(); //TODO à voir
            void setCell(Cell * cell);
            int rowCount(const QModelIndex& parent = QModelIndex()) const;
            int columnCount(const QModelIndex& parent = QModelIndex()) const;
            QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    };


}