#include "Cell.h"

class CellViewer : public QMainWindow {
    Q_OBJECT;
  public:
                      CellViewer          ( QWidget* parent=NULL );
    virtual          ~CellViewer          ();
            Cell*     getCell             () const;
    inline  CellsLib* getCellsLib         ();  // TME9+.
  public slots:
            void      setCell             ( Cell* );
            void      saveCell            ();
            void      openCell            ();
            void      showCellsLib        ();  // TME9+.
            void      showInstancesWidget ();  // TME9+.
  private:
    CellWidget*      cellWidget_;
    CellsLib*        cellsLib_;         // TME9+.
    InstancesWidget* instancesWidget_;  // TME9+.
    SaveCellDialog*  saveCellDialog_;
};
