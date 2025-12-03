// -*- explicit-buffer-name: "Line.cpp<M1-MOBJ/7>" -*-

#include "XmlUtil.h"
#include "Line.h"
#include "Cell.h"
#include "Net.h"


namespace Netlist {

  using namespace std;


  Line::Line ( Node* source, Node* target )
    : source_(source)
    , target_(target)
  {
    source_->attach( this );
    target_->attach( this );
  }


  Line::~Line ()
  {
    source_->detach( this );
    target_->detach( this );
    target_->getNet()->remove( this );
  }


  void  Line::toXml ( ostream& stream ) const
  {
    stream << indent << "<line source=\"" << source_->getId()
                     <<    "\" target=\"" << target_->getId() << "\"/>\n";
  }

/*
  bool   Line::fromXml ( Net* net, xmlTextReaderPtr reader )
  {
    const xmlChar* lineTag  = xmlTextReaderConstString        ( reader, (const xmlChar*)"line" );
    const xmlChar* nodeName = xmlTextReaderConstLocalName     ( reader );

    if (lineTag == nodeName) {
      int idSource = 0;
      int idTarget = 0;
      xmlGetIntAttribute( reader, "source", idSource );
      xmlGetIntAttribute( reader, "target", idTarget );

      Node* source = nullptr;
      Node* target = nullptr;
      
      const std::vector<Node*>& nodes = net->getNodes();
      int id_source = -1;
      int id_target = -1;
      for (Node* node : nodes) {
        id_source = node->getId();
        if (node->getId() == static_cast<size_t>(idSource)) {
          source = node;
          std::cout << " pas erreur au node :" << node->getId() << endl;
        }
        else {
          std::cout << " erreur au node :" << node->getId() << endl;
        }
        id_target = node->getId();
        if (node->getId() == static_cast<size_t>(idTarget)) {
          target = node;
        }
                else {
          std::cout << " erreur à la target :" << endl;
        }

      }

      if (not source) {
        std::cout << "[DEBUG] Line::fromXml(): Checking Node id " << id_source << " against source id "
                  << idSource << "." << std::endl;
        cerr << "[ERROR] Line::fromXml(): Unknown source node id:" << idSource << " (line:"
             << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
        return false;
      }

      
      if (not target) {
        std::cout << "[DEBUG] Line::fromXml(): Checking Node id " << id_target << " aulieu de "
                  << idTarget << "." << std::endl;
        cerr << "[ERROR] Line::fromXml(): Unknown target node id:" << idTarget << " (line:"
             << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
        return false;
      }

      net->add( new Line(source,target) );
      return true;
    }

    return false;
  }

*/
  bool   Line::fromXml ( Net* net, xmlTextReaderPtr reader )
  {
    const xmlChar* lineTag  = xmlTextReaderConstString        ( reader, (const xmlChar*)"line" );
    const xmlChar* nodeName = xmlTextReaderConstLocalName     ( reader );

    if (lineTag == nodeName) {
      int idSource = 0;
      int idTarget = 0;
      xmlGetIntAttribute( reader, "source", idSource );
      xmlGetIntAttribute( reader, "target", idTarget );

      Node*      source = net->getNode( idSource );
      Node*      target = net->getNode( idTarget );

      if (not source) {
        cerr << "[ERROR] Line::fromXml(): Unknown source node id:" << idSource << " (line:"
             << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
        return false;
      }
      if (not target) {
        cerr << "[ERROR] Line::fromXml(): Unknown target node id:" << idTarget << " (line:"
             << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
        return false;
      }

      net->add( new Line(source,target) );
      return true;
    }

    return false;
  }
}  // Netlist namespace.
