// -*- explicit-buffer-name: "Main.cpp<M1-MOBJ/7>" -*-

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
using namespace std;

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
using namespace Netlist;

#include <QApplication>
#include <QtGui>
#include "CellViewer.h"

int main ( int argc, char* argv[] ){
  // CHARGEMENT DES MODELES
  std::cout << "Chargement des modeles..." << std::endl;
  Cell* vdd = Cell::load( "vdd" );
  Cell* gnd = Cell::load( "gnd" );
  Cell* transistorN = Cell::load( "TransistorN" );
  Cell* transistorP = Cell::load( "TransistorP" );
  Cell* and2 = Cell::load( "and2" );
  Cell* or2 = Cell::load( "or2" );
  Cell* xor2      = Cell::load( "xor2" );
  Cell* halfadder = Cell::load( "halfadder" );
  std::cout << "half chargé" << std::endl;

  // cout << "\nContenu du <xor2>:" << endl;
  // xor2->toXml( cout );
  
  cout << "\nContenu du <halfadder>:" << endl;
  halfadder->toXml( cout );

  // TESTS APPLICATION
  QApplication* qa = new QApplication(argc, argv);

  CellViewer* viewer = new CellViewer();
  viewer->setCell(halfadder);
  std::cout << "half set\n" << std::endl;
  viewer->show();
  std::cout << "half show\n" << std::endl;

  int rvalue = qa->exec();
  delete qa;
  return rvalue;
}
