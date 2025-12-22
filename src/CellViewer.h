#include <QDialog>
#include <QMainWindow>
#include "Cell.h"
#include "CellWidget.h"
#include "SaveCellDialog.h"
#include "OpenCellDialog.h"
#include "CellWidget.h"

namespace Netlist
{
    class CellsLib;
    class InstancesWidget;

    class CellViewer : public QMainWindow
    {
        Q_OBJECT;

    public:
                        CellViewer            (QWidget* parent=NULL);
      virtual          ~CellViewer            ();
              Cell*     getCell               () const;
              inline    CellsLib* getCellsLib ();  // TME9+.
    public slots:
              void      setCell               (Cell*);
              void      saveCell              ();
              void      openCell              ();
              void      showCellsLib          ();  // TME9+.
              void      showInstancesWidget   ();  // TME9+.
    // signal //c'est un signal
              void      cellLoaded            () {};

    private:
        CellWidget *cellWidget_;
        CellsLib *cellsLib_;               // TME9+.
        InstancesWidget *instancesWidget_; // TME9+.
        SaveCellDialog *saveCellDialog_;
    };

  inline CellsLib* CellViewer::getCellsLib() { return cellsLib_; };
}
