// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
class QPainter;
#include "Box.h"

namespace Netlist
{

    class Cell;
    class NodeTerm;

    /*
    Point	  x, y
    QPoint	  x, y
    Box	      x1, y1, x2, y2  (les coins inférieur droit et supérieur gauche)
    QRect	  x, y, w, h      (Le coin supérieur gauche, la largeur (w) et hauteur (h))
    */

    class CellWidget : public QWidget
    {
        Q_OBJECT;

    public:
                    void    query(unsigned int flag, QPainter & peintre); //TODO check apres cette fonction mec
                            CellWidget(QWidget *parent = NULL);
        virtual             ~CellWidget();
                    void    setCell(Cell *);
        inline      Cell    *getCell() const { return this->cell_; }
        inline      int     xToScreenX(int x) const { return x - viewport_.getX1(); }
        inline      int     yToScreenY(int y) const { return viewport_.getY2() + y; }
        //converti de box en QRect
        inline      QRect boxToScreenRect(const Box &box) const
        {
            int x = xToScreenX(box.getX1());
            int y = yToScreenY(box.getY1());
            int w = box.getX2() - box.getX1();
            int h = box.getY2() - box.getY1();
            return QRect(x, y, w, h);
        }
        //converti de point en Qpoint
        inline      QPoint  pointToScreenPoint(const Point &point) const {
            int x = xToScreenX(point.getX());
            int y = yToScreenY(point.getY());
            return QPoint(x,y);//en vrai on peut appeler les fonction ici pas besoin d'allouer x et y mais c'est plus propre
        }
        //converti de x widget vers x shema
        inline      int     screenXToX(int x) const {
            return x + viewport_.getX1();
        }
        //converti de y widget vers y shema
        inline      int     screenYToY(int y) const {
            return viewport_.getY2() + y;
        }
        //converti de QRect vers Box 
        inline      Box     screenRectToBox(const QRect &rect) const{
            int x1 = screenXToX(rect.x());                    //coin gauche
            int x2 = screenXToX(rect.x() + rect.width());     //coin droit (x + largeur)
            int y2 = screenYToY(rect.y());                    //haut (petit y écran)
            int y1 = screenYToY(rect.y() + rect.height());    //bas (grand y écran)
            return Box(x1, y1, x2, y2);
        }
        inline      Point   screenPointToPoint(const QPoint &point) const {
            int x = screenXToX(point.x());
            int y = screenYToY(point.y());
            return Point(x, y); //pareil on est pas obligé de faire x et y mais c'est joli :)
        }
        virtual     QSize   minimumSizeHint() const;
        virtual     void    resizeEvent(QResizeEvent *);

    protected:
        virtual     void    paintEvent(QPaintEvent *);
        virtual     void    keyPressEvent(QKeyEvent *);
    public slots:
                    void    goLeft();
                    void    goRight();
                    void    goUp();
                    void    goDown();

    private:
        Cell *cell_;
        Box viewport_;
    };

} // Netlist namespace.

#endif // NETLIST_CELL_WIDGET_H
