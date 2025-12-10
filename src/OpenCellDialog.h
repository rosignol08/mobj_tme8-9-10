#include <QDialog>
#include <QLineEdit>

namespace Netlist{

  class OpenCellDialog : public QDialog{
    Q_OBJECT;

    public:
                      OpenCellDialog  (QWidget* parent = NULL);
                     ~OpenCellDialog  ();
      static  bool    run             (QString& name);
      const   QString getCellName     () const;
              void    setCellName     (const QString&);

    protected:
      QLineEdit* lineEdit_;
  };
}