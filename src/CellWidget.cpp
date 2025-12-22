// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include <QResizeEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QApplication>
#include "CellWidget.h"
#include "Term.h"
#include "Instance.h"
#include "Symbol.h"
#include "Shape.h"
#include "Cell.h"
#include "Line.h"
#include "Node.h"
#include "Net.h"
#include "BoxShape.h"
#include "LineShape.h"

namespace Netlist
{

    using namespace std;

    //flags pour query()
    const unsigned int InstanceShape = 0x1;

    ostream &operator<<(ostream &o, const QRect &rect)
    {
        o << "<QRect x:" << rect.x()
          << " y:" << rect.y()
          << " w:" << rect.width()
          << " h:" << rect.height() << ">";
        return o;
    }

    ostream &operator<<(ostream &o, const QPoint &p)
    {
        o << "<QRect x:" << p.x() << " y:" << p.y() << ">";
        return o;
    }

    CellWidget::CellWidget(QWidget *parent)
        : QWidget(parent), cell_(nullptr),viewport_(0,0,500,500){
        setAttribute(Qt::WA_OpaquePaintEvent,true);//on retrace "tout" (avant et arrière plan)
        setAttribute(Qt::WA_NoSystemBackground,true);//important
        setAttribute(Qt::WA_StaticContents);//jsp
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setFocusPolicy(Qt::StrongFocus);
        setMouseTracking(true);
    }

    CellWidget::~CellWidget() {}

    void CellWidget::setCell(Cell *cell)
    {
        cell_ = cell;
        repaint();
    }

    QSize CellWidget::minimumSizeHint() const
    {
        return QSize(500, 500);
    }

    void CellWidget::resizeEvent(QResizeEvent *event){
        const QSize size = event->size();
        viewport_.setX2(viewport_.getX1() + size.width()); //TODO il faut des majuscules ou pas
        viewport_.setY1(viewport_.getY2() - size.height());
        repaint();
        return;
    }

    void CellWidget::paintEvent(QPaintEvent *event)
    {
        QFont bigFont = QFont("URW Bookman L", 36);

        QString cellName = "NULL";
        if (cell_)
            cellName = cell_->getName().c_str();

        QPainter painter(this);//sur le qwidget
        painter.setFont(bigFont);
        painter.setBackground(QBrush(Qt::white));//tu peux mette black si tu pref mec
        painter.eraseRect(QRect(QPoint(0, 0), size()));//fond arrier plan
        painter.setPen(QPen(Qt::darkGreen,2)); //avant plan
        int frameWidth = 460;
        int frameHeight = 100;
        QRect nameRect((size().width() - frameWidth) / 2, (size().height() - frameHeight) / 2, frameWidth, frameHeight);
        
        //dessine les formes des instances
        query(InstanceShape, painter);
        
        //dessine le nom au centre
        //painter.drawRect(nameRect);
        //painter.drawText(nameRect, Qt::AlignCenter, cellName);
    }
    //pour dessiner les formes des instances
    void CellWidget::query(unsigned int flag, QPainter & painter){
        if( !cell_ || !flag) return;
        const std::vector<Instance*> & instances = cell_->getInstances();
        for(size_t i = 0; i < instances.size(); ++i){
            Point instPoint = instances[i]->getPosition(); // voir ou on l'utilise
            const Symbol* symboles = instances[i]->getMasterCell()->getSymbol();
            if(!symboles){
                continue;
            }
            if(flag & InstanceShape){
                const std::vector<Shape*> & shapes = symboles->getShapes();
                std::cout << "Instance: " << instances[i]->getName() << " - Nb formes: " << shapes.size() << std::endl;
                for(size_t j = 0; j < shapes.size(); ++j){
                    BoxShape* boxShape = dynamic_cast<BoxShape*>(shapes[j]);
                    if(boxShape){
                        Box box = boxShape->getBoundingBox();
                        box.translate(instPoint);
                        QRect rect = boxToScreenRect(box); //TODO verifie la fonction
                        painter.drawRect(rect);
                        std::cout << " -> Dessin BoxShape: " << rect << std::endl;
                    }
                    //LineShape
                    LineShape* lineShape = dynamic_cast<LineShape*>(shapes[j]);
                    if(lineShape){
                        int x1 = lineShape->getX1();
                        int y1 = lineShape->getY1();
                        int x2 = lineShape->getX2();
                        int y2 = lineShape->getY2();

                        x1 += instPoint.getX();
                        y1 += instPoint.getY();
                        x2 += instPoint.getX();
                        y2 += instPoint.getY();

                        //conversion en coordonnées écran (faut checker les fonction yToScreenY, boxToScreenRect et screenYToY)
                        QPoint p1(xToScreenX(x1), yToScreenY(y1));
                        QPoint p2(xToScreenX(x2), yToScreenY(y2));

                        //draw la ligne
                        painter.drawLine(p1, p2);
                        std::cout << " -> Dessin LineShape: " << p1 << " to " << p2 << std::endl;
                    }
                    else {
                        std::cout << " -> Forme ignoree (pas une shape definie)" << std::endl;
                    }
                }
                std::cout << "Instance " << instances[i]->getName() << " at " << instPoint << std::endl;
            }
        }
    }

    void CellWidget::goUp(){
        viewport_.translate(Point(0,20)); //monte de 20 px
        std::cout << "nouvelles position du viewport : " << viewport_ << std::endl;
        repaint();
    }

    void CellWidget::goDown(){
        viewport_.translate(Point(0,-20));
        std::cout << "nouvelles position du viewport : " << viewport_ << std::endl;
        repaint();
    }
    
    void CellWidget::goLeft(){
        viewport_.translate(Point(-20,0));
        std::cout << "nouvelles position du viewport : " << viewport_ << std::endl;
        repaint();
    }
    
    void CellWidget::goRight(){
        viewport_.translate(Point(20,0));//va a droite de 20 px normalement
        std::cout << "nouvelles position du viewport : " << viewport_ << std::endl;
        repaint();
    }

    void CellWidget::keyPressEvent(QKeyEvent *event){
        if(event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier)){
            event->ignore();
            return;
        }
        switch(event->key()){
            case Qt::Key_Up:
                goDown(); //TODO voir si faut pas inverser
                break;
            case Qt::Key_Down:
                goUp();
                break;
            case Qt::Key_Left:
                goLeft();
                break;
            case Qt::Key_Right:
                goRight();
                break;
            default:
                event->ignore();
                return;
        }
        event->accept();
    }

} // Netlist namespace.
