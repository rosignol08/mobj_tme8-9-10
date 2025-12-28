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

/*
Widget affichant le dessin complet d'une Cell. On implantera progessivement l'affichage des différents éléments:

Dessin du symbole des instances.
Dessin des connecteurs des instances.
Dessin des connecteurs du modèle.
Dessin des fils.
Fonctions de déplacement (haut, bas, gauche droite).

*/

namespace Netlist
{
    class Cell;
    class NodeTerm;
    class BoxShape;
    class LineShape;
    class TermShape;
    class ArcShape;
    class EllipseShape;
    class Term;
    class Instance; //test: si on veut ajouter des instances

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
                    void    query(unsigned int flag, QPainter & painter); //TODO check apres cette fonction mec
                            CellWidget(QWidget *parent = NULL);
        virtual            ~CellWidget();
                    void    setCell(Cell *);
        inline      Cell*   getCell() const;

        inline      int     xToScreenX(int x) const;
        inline      int     yToScreenY(int y) const;
        inline      QRect   boxToScreenRect(const Box &box) const; //converti de box en QRect
        inline      QPoint  pointToScreenPoint(const Point &point) const; //converti de point en Qpoint
        inline      int     screenXToX(int x) const; //converti de x widget vers x shema
        inline      int     screenYToY(int y) const; //converti de y widget vers y shema
        inline      Box     screenRectToBox(const QRect &rect) const; //converti de QRect vers Box 
        inline      Point   screenPointToPoint(const QPoint &point) const;

        virtual     QSize   minimumSizeHint() const;
        virtual     void    resizeEvent(QResizeEvent *);

    protected:
        virtual     void    paintEvent(QPaintEvent *);
        virtual     void    keyPressEvent(QKeyEvent *);
        //test: si on veut ajouter des instances
        virtual     void    mouseMoveEvent(QMouseEvent*); //fonction pour les ajout d'instances
        virtual     void    mousePressEvent(QMouseEvent*); //fonction pour les ajout d'instances
        virtual     void    mouseReleaseEvent(QMouseEvent*); //fonction pour les ajout d'instances

    public slots:
                    void    goLeft();
                    void    goRight();
                    void    goUp();
                    void    goDown();

    private:
        Cell *cell_;
        Box viewport_;
        //test: si on veut ajouter des instances
        QPoint dragStartPoint_; //pareil c'est pour drag les instances
        bool dragging_; //pareil c'est pour drag les instances
        Instance* selectedInstance_; //pareil c'est pour drag les instances
        bool movingInstance_; //pareil c'est pour drag les instances

        Instance* findInstanceAt(const Point& position); //test: si on veut ajouter des instances
        void drawBoxShape     (BoxShape* ellipseShape, QPainter & painter, Point instPoint);
        void drawLineShape    (LineShape* ellipseShape, QPainter & painter, Point instPoint);
        void drawTermShape    (TermShape* ellipseShape, QPainter & painter, Point instPoint);
        void drawArcShape     (ArcShape* ellipseShape, QPainter & painter, Point instPoint);
        void drawEllipseShape (EllipseShape* ellipseShape, QPainter & painter, Point instPoint);
        void drawTerm         (Term* term, QPainter& painter);
    };

  inline Cell* CellWidget::getCell() const { return this->cell_; }

  inline int CellWidget::xToScreenX(int x) const { return x - viewport_.getX1(); }

  inline int CellWidget::yToScreenY(int y) const { return viewport_.getY2() - y; } //TODO voir si faut remplacer par un -

  //converti de box en QRect
  inline QRect CellWidget::boxToScreenRect(const Box &box) const
  {
      /* //correction qu'on m'as donné mais jsp si c'est bon donc je met de cote
      int x = xToScreenX(box.getX1());
      // IMPORTANT : On prend le Y2 (le HAUT du schéma) pour le point de départ écran
      int y = yToScreenY(box.getY2()); 
      int w = box.getX2() - box.getX1();
      int h = box.getY2() - box.getY1();
      return QRect(x, y, w, h);
      */
      int x = xToScreenX(box.getX1());
      int y = yToScreenY(box.getY2());
      int w = box.getX2() - box.getX1();
      int h = box.getY2() - box.getY1();
      return QRect(x, y, w, h);
  }

  //converti de point en Qpoint
  inline QPoint CellWidget::pointToScreenPoint(const Point &point) const
  {
      int x = xToScreenX(point.getX());
      int y = yToScreenY(point.getY());
      return QPoint(x,y);//en vrai on peut appeler les fonction ici pas besoin d'allouer x et y mais c'est plus propre
  }

  //converti de x widget vers x shema
  inline int CellWidget::screenXToX(int x) const { return x + viewport_.getX1(); }

  //converti de y widget vers y shema
  inline int CellWidget::screenYToY(int y) const { return viewport_.getY2() - y; } //TODO voir si faut remplacer par un -

  //converti de QRect vers Box 
  inline Box CellWidget::screenRectToBox(const QRect &rect) const
  {
      int x1 = screenXToX(rect.x());                    //coin gauche
      int x2 = screenXToX(rect.x() + rect.width());     //coin droit (x + largeur)
      int y2 = screenYToY(rect.y());                    //haut (petit y écran)
      int y1 = screenYToY(rect.y() + rect.height());    //bas (grand y écran)
      return Box(x1, y1, x2, y2);
  }

  inline Point CellWidget::screenPointToPoint(const QPoint &point) const
  {
      int x = screenXToX(point.x());
      int y = screenYToY(point.y());
      return Point(x, y); //pareil on est pas obligé de faire x et y mais c'est joli :)
  }

}

#endif // NETLIST_CELL_WIDGET_H
