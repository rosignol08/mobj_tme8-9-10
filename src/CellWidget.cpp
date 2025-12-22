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
        //ou QRect nameRect = boxtoscreenrect(box);

        painter.drawRect(nameRect);
        painter.drawText(nameRect, Qt::AlignCenter, cellName);
        // ... query(Instances Shapes, painter);
    }
    //pour "rafiner les traits"
    void CellWidget::query(unsigned int flag, QPainter & peintre){
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
                for(size_t j = 0; j < shapes.size(); ++j){
                    BoxShape* boxShape = dynamic_cast<BoxShape*>(shapes[j]);
                    if(boxShape){
                        Box box = boxShape->getBoundingBox();
                        QRect rect = boxToScreenRect(box); //TODO verifie la fonction
                        peintre.drawRect(rect);
                    }
                }
            }
        }
    }

    void CellWidget::goUp(){
        viewport_.translate(Point(0,20)); //monte de 20 px
        repaint();
    }

    void CellWidget::goDown(){
        viewport_.translate(Point(0,-20));
        repaint();
    }
    
    void CellWidget::goLeft(){
        viewport_.translate(Point(-20,0));
        repaint();
    }
    
    void CellWidget::goRight(){
        viewport_.translate(Point(20,0));//va a droite de 20 px normalement
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
