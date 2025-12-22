
//Fenêtre affichant la liste des instances et leurs modèles
/*
Le modèle associé à la QTableView (attribut view_) de type InstancesModel (attribut baseModel_) présentera deux colonnes:

Le nom de l'instance.   
Le nom du modèle de l'instance (le nom de la master cell).
Ne pas oublier de fournir le nom des colonnes (headers).
*/

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
      void  updateInstances ();

    private:
      CellViewer*     cellViewer_;
      InstancesModel* baseModel_;
      QTableView*     view_;
      QPushButton*    load_;
  };
}
