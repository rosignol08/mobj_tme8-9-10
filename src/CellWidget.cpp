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
#include "TermShape.h"
#include "EllipseShape.h"
#include "ArcShape.h"

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
        : QWidget(parent), cell_(nullptr),viewport_(0,-500,500,0){
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

    void CellWidget::drawBoxShape(BoxShape* boxShape, QPainter & painter, Point position){
        Box box = boxShape->getBoundingBox();
        box.translate(position);
        QRect rect = boxToScreenRect(box); //TODO verifie la fonction
        painter.drawRect(rect);
        std::cout << " -> Dessin BoxShape: " << rect << std::endl;
    }

    void CellWidget::drawLineShape(LineShape* lineShape, QPainter & painter, Point position){
        int x1 = lineShape->getX1();
        int y1 = lineShape->getY1();
        int x2 = lineShape->getX2();
        int y2 = lineShape->getY2();

        x1 += position.getX();
        y1 += position.getY();
        x2 += position.getX();
        y2 += position.getY();

        //conversion en coordonnées écran (faut checker les fonction yToScreenY, boxToScreenRect et screenYToY)
        QPoint p1(xToScreenX(x1), yToScreenY(y1));
        QPoint p2(xToScreenX(x2), yToScreenY(y2));

        //draw la ligne
        painter.drawLine(p1, p2);
        std::cout << " -> Dessin LineShape: " << p1 << " to " << p2 << std::endl;
    }

    void CellWidget::drawArcShape(ArcShape* arcShape, QPainter& painter, Point position){
        Box box = arcShape->getBoundingBox();
        box.translate(position);
        int start = arcShape->getStart();
        int span = arcShape->getSpan();
        cout << "start = " << start << ", span = " << span << endl;
        QRect rect = boxToScreenRect(box); 
        painter.drawArc(rect, start*16, span*16); //TODO comprendre pourquoi le symbole de xor2 ne s'affiche pas correctement
                                                  // (c'est peut-être à cause du XML et pas de la fonction)
        // painter.drawRect(rect);
        std::cout << " -> Dessin ArcShape: " << rect << "with start " << start << " with span "<< span << std::endl;
    }

    void CellWidget::drawEllipseShape(EllipseShape* ellipseShape, QPainter & painter, Point position){
        Box box = ellipseShape->getBoundingBox();
        box.translate(position);
        QRect rect = boxToScreenRect(box); //TODO verifie la fonction
        painter.drawEllipse(rect);
        std::cout << " -> Dessin EllipseShape: " << rect << std::endl;
    }

    // Dessine un terminal d'une instance (internal)
    void CellWidget::drawTermShape(TermShape* termShape, QPainter& painter, Point position){
        int x = position.getX();
        int y = position.getY();
        Term* term = termShape->getTerm();
        QString text = QString::fromStdString(term->getName());

        painter.setPen(QPen(Qt::red));

        // TODO essayer d'aligner le texte par rapport au symbole en respectant l'attribut Term.align_
        // NameAlign nameAlign = termShape->getAlign();
        // Qt::Alignment align = NULL;
        // if      (nameAlign == TopLeft)     { align=Qt::AlignTop; }
        // else if (nameAlign == TopRight)    { align=Qt::AlignTop; }
        // else if (nameAlign == BottomLeft)  { align=Qt::AlignBottom; }
        // else if (nameAlign == BottomRight) { align=Qt::AlignBottom; }
        // QTextOption textOption = QTextOption(align);

        int width = 10;

        QPoint p(xToScreenX(x + termShape->getX()), yToScreenY(y + termShape->getY())); //TODO ça marche c'etait ça avant
        //QPoint p(xToScreenX(x) + termShape->getX(), yToScreenY(y) + termShape->getY()); //céest cassé ça 
        QRect rect = QRect(p.x() - (width/2), p.y() - (width/2), width, width);
        // QPainter::fillRect(rect, Qt::red);
        // fillRect(rect, Qt::red);

        // TODO trouver comment dessiner une rectangle plein
        painter.drawRect(rect);

        // const QRect rectangle = QRect(p.x(), p.y(), 100, 50);
        // QRect boundingRect;
        // painter.drawText(rectangle, 0, text, &boundingRect);

        QFont font = QFont();
        font.setPixelSize(10);
        painter.setFont(font);
        painter.drawText(p.x() - 10, p.y() - 10, text);

        std::cout << " -> Dessin TermShape: " << p << std::endl;
    }

    // Dessine un terminal d'une cellule (external)
    void CellWidget::drawTerm(Term* term, QPainter& painter){
        QString text = QString::fromStdString(term->getName());
        Point position = term->getPosition();
        int x = xToScreenX(position.getX());
        int y = yToScreenY(position.getY());
        int width = 20;
        Term::Direction dir = term->getDirection();
        QFont font = QFont();
        font.setPixelSize(10);

        painter.setFont(font);
        painter.setPen(QPen(Qt::red));

        if (dir == Term::Direction::In){
            QPoint points1[5] = {
                    QPoint(x-(width/2), y-(width/2)),
                    QPoint(x, y-(width/2)),
                    QPoint(x+(width/2), y),
                    QPoint(x, y+(width/2)),
                    QPoint(x-width/2, y+(width/2))
            };
            painter.drawPolygon(points1, 5, Qt::OddEvenFill);
        }
        else if (dir == Term::Direction::Out){
            QPoint points2[5] = {
                QPoint(x-(width/2), y),
                QPoint(x, y-(width/2)),
                QPoint(x+(width/2), y-(width/2)),
                QPoint(x+(width/2), y+(width/2)),
                QPoint(x, y+(width/2))
            };
            painter.drawPolygon(points2, 5, Qt::WindingFill);
        }
        painter.drawText(x - 10, y - 10, text);
    }

    //pour dessiner les formes des instances
    void CellWidget::query(unsigned int flag, QPainter & painter){
        if( !cell_ || !flag) return;
        const std::vector<Instance*>& instances = cell_->getInstances();
        const std::vector<Term*>& terms = cell_->getTerms();
        const std::vector<Net*>& nets = cell_->getNets();
        for(size_t i = 0; i < instances.size(); ++i){
            Point position = instances[i]->getPosition(); // voir où on l'utilise
            const Symbol* symboles = instances[i]->getMasterCell()->getSymbol();
            if(!symboles){
                continue;
            }
            if(flag & InstanceShape){
                const std::vector<Shape*> & shapes = symboles->getShapes();
                std::cout << "Instance: " << instances[i]->getName() << " - Nb formes: " << shapes.size() << std::endl;
                for(size_t j = 0; j < shapes.size(); ++j){
                    BoxShape* boxShape = dynamic_cast<BoxShape*>(shapes[j]);
                    LineShape* lineShape = dynamic_cast<LineShape*>(shapes[j]);
                    TermShape* termShape = dynamic_cast<TermShape*>(shapes[j]);
                    ArcShape* arcShape = dynamic_cast<ArcShape*>(shapes[j]);
                    EllipseShape* ellipseShape = dynamic_cast<EllipseShape*>(shapes[j]);

                    painter.setPen(QPen(Qt::darkGreen));

                    if(boxShape) drawBoxShape(boxShape, painter, position);
                    else if(lineShape) drawLineShape(lineShape, painter, position);
                    else if(termShape) drawTermShape(termShape, painter, position);
                    else if(arcShape) drawArcShape(arcShape, painter, position);
                    else if(ellipseShape) drawEllipseShape(ellipseShape, painter, position);
                    else std::cout << " -> Forme ignoree (pas une shape définie)" << std::endl;
                }
                std::cout << "Instance " << instances[i]->getName() << " at " << position << std::endl;
            }
        }
        for (Term* term: terms){
            if (term){
                drawTerm(term, painter);
            }
        }
        for(Net* net: nets){
            for (Line* line: net->getLines()){
                if (line){
                    painter.setPen(QPen(Qt::cyan));
                    Point pSource = line->getSourcePosition(); 
                    Point pTarget = line->getTargetPosition();
                    int xSrc = xToScreenX(pSource.getX());
                    int ySrc = yToScreenY(pSource.getY());
                    int xTrg = xToScreenX(pTarget.getX());
                    int yTrg = yToScreenY(pTarget.getY());
                    painter.drawLine(QPoint(xSrc, ySrc), QPoint(xTrg, yTrg));
                }
            }

            for (Node* node: net->getNodes()){
                if (node){
                    if (node->getLines().size() >= 3) {
                        int width = 10;
                        painter.setPen(QPen(Qt::cyan));
                        int x = xToScreenX(node->getPosition().getX());
                        int y = yToScreenY(node->getPosition().getY());
                        painter.drawRect(x - width/2, y - width/2, width, width);
                    }
                    
                }
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
                goUp();
                break;
            case Qt::Key_Down:
                goDown();
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
