// #include "Draw.h"
// // #include "Point.h"
// // // #include "CellWidget.h"
// // #include "Term.h"
// #include "BoxShape.h"
// #include "Box.h"
// // #include <QPainter>
// #include <QRect>
// // #include <QPoint>

// using namespace std;

// namespace Netlist{
//   void Draw::drawBoxShape(BoxShape* boxShape, QPainter & painter, Point position){
//         Box box = boxShape->getBoundingBox();
//         box.translate(position);
//         QRect rect = CellWidget::boxToScreenRect(box); //TODO verifie la fonction
//         painter.drawRect(rect);
//         std::cout << " -> Dessin BoxShape: " << rect << std::endl;
//     }

//     void Draw::drawLineShape(LineShape* lineShape, QPainter & painter, Point position){
//         int x1 = lineShape->getX1();
//         int y1 = lineShape->getY1();
//         int x2 = lineShape->getX2();
//         int y2 = lineShape->getY2();

//         x1 += position.getX();
//         y1 += position.getY();
//         x2 += position.getX();
//         y2 += position.getY();

//         //conversion en coordonnées écran (faut checker les fonction yToScreenY, boxToScreenRect et screenYToY)
//         QPoint p1(xToScreenX(x1), yToScreenY(y1));
//         QPoint p2(xToScreenX(x2), yToScreenY(y2));

//         //draw la ligne
//         painter.drawLine(p1, p2);
//         std::cout << " -> Dessin LineShape: " << p1 << " to " << p2 << std::endl;
//     }

//     void Draw::drawTermShape(TermShape* termShape, QPainter& painter, Point position){
//         int x = position.getX();
//         int y = position.getY();
//         Term* term = termShape->getTerm();
//         QString text = QString::fromStdString(term->getName());

//         painter.setPen(QPen(Qt::red));

//         // NameAlign nameAlign = termShape->getAlign();
//         // Qt::Alignment align = NULL;
//         // if      (nameAlign == TopLeft)     { align=Qt::AlignTop; }
//         // else if (nameAlign == TopRight)    { align=Qt::AlignTop; }
//         // else if (nameAlign == BottomLeft)  { align=Qt::AlignBottom; }
//         // else if (nameAlign == BottomRight) { align=Qt::AlignBottom; }
//         // QTextOption textOption = QTextOption(align);

//         int width = 10;

//         QPoint p(xToScreenX(x) + termShape->getX(), yToScreenY(y) + termShape->getY());
//         QRect rect = QRect(p.x() - (width/2), p.y() - (width/2), width, width);
//         // QPainter::fillRect(rect, Qt::red);
//         // fillRect(rect, Qt::red);
//         painter.drawRect(rect);

//         // const QRect rectangle = QRect(p.x(), p.y(), 100, 50);
//         // QRect boundingRect;
//         // painter.drawText(rectangle, 0, text, &boundingRect);

//         QFont font = QFont();
//         font.setPixelSize(10);
//         painter.setFont(font);
//         painter.drawText(p.x(), p.y(), text);

//         std::cout << " -> Dessin TermShape: " << p << std::endl;
//     }

//     void Draw::drawArcShape(ArcShape* arcShape, QPainter& painter, Point position){
//         Box box = arcShape->getBoundingBox();
//         box.translate(position);
//         int start = arcShape->getStart();
//         int span = arcShape->getSpan();
//         cout << "start = " << start << ", span = " << span << endl;
//         QRect rect = boxToScreenRect(box); //TODO verifie la fonction
//         painter.drawArc(rect, start*16, span*16);
//         // painter.drawRect(rect);
//         std::cout << " -> Dessin ArcShape: " << rect << "with start " << start << " with span "<< span << std::endl;
//     }

//     void Draw::drawEllipseShape(EllipseShape* ellipseShape, QPainter & painter, Point position){
//         Box box = ellipseShape->getBoundingBox();
//         box.translate(position);
//         QRect rect = boxToScreenRect(box); //TODO verifie la fonction
//         painter.drawEllipse(rect);
//         std::cout << " -> Dessin EllipseShape: " << rect << std::endl;
//     }

//     void Draw::drawTerm(Term* term, QPainter& painter){
//         QString text = QString::fromStdString(term->getName());
//         Point position = term->getPosition();
//         int x = xToScreenX(position.getX());
//         int y = yToScreenY(position.getY());
//         int width = 20;
//         Term::Direction dir = term->getDirection();
//         QFont font = QFont();
//         font.setPixelSize(10);

//         painter.setFont(font);
//         painter.setPen(QPen(Qt::red));

//         switch(dir){
//         case Term::Direction::In:
//             // QPoint points1[5] = {
//             //         QPoint(x-(width/2), y-(width/2)),
//             //         QPoint(x, y-(width/2)),
//             //         QPoint(x+(width/2), y),
//             //         QPoint(x, y+(width/2)),
//             //         QPoint(x-width/2, y+(width/2))
//             // };
//             // painter.drawPolygon(points1, 5);
//             // painter.drawPolygon(
//             //     (QPoint[2]) {
//             //         QPoint(1,2),
//             //         QPoint(3,4)
//             //     }, 2
//             // );
//             // painter.drawLine(QPoint(x, y+(width/2)), QPoint(x+(width/2), y));
//             // painter.drawLine(QPoint(x, y-(width/2)), QPoint(x+(width/2), y));
//             // painter.drawRect(x-(width/2), y-(width/2), width/2, width);
//             break;
//         // case Term::Direction::Out:
//         //     QPoint points2[5] = {
//         //             QPoint(x-(width/2), y),
//         //             QPoint(x, y-(width/2)),
//         //             QPoint(x+(width/2), y-(width/2)),
//         //             QPoint(x+(width/2), y-(width/2)),
//         //             QPoint(x, y+(width/2))
//         //         };
//         //     painter.drawPolygon(points2, 5);
//         //     // painter.drawLine(QPoint(x, y+(width/2)), QPoint(x-(width/2), y));
//         //     // painter.drawLine(QPoint(x, y-(width/2)), QPoint(x-(width/2), y));
//         //     // painter.drawRect(x, y-(width/2), width/2, width);
//         //     break;
//         default:;
//         };
//         if (dir == Term::Direction::In){
//             QPoint points1[5] = {
//                     QPoint(x-(width/2), y-(width/2)),
//                     QPoint(x, y-(width/2)),
//                     QPoint(x+(width/2), y),
//                     QPoint(x, y+(width/2)),
//                     QPoint(x-width/2, y+(width/2))
//             };
//             painter.drawPolygon(points1, 5, Qt::OddEvenFill);
//         }
//         else if (dir == Term::Direction::Out){
//             QPoint points2[5] = {
//                 QPoint(x-(width/2), y),
//                 QPoint(x, y-(width/2)),
//                 QPoint(x+(width/2), y-(width/2)),
//                 QPoint(x+(width/2), y+(width/2)),
//                 QPoint(x, y+(width/2))
//             };
//             painter.drawPolygon(points2, 5, Qt::WindingFill);
//         }
//         painter.drawText(x, y, text);
//     }
// }