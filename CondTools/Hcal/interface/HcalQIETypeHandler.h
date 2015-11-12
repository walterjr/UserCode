#ifndef HcalQIETypeHandler_h
#define HcalQIETypeHandler_h

// Walter Alda, 11.10.2015


#include <string>
#include <iostream>
#include <typeinfo>
#include <fstream>

#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondCore/PopCon/interface/PopConSourceHandler.h"
 
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
// user include files
#include "CondFormats/HcalObjects/interface/HcalQIEType.h"
#include "CondFormats/DataRecord/interface/HcalQIETypeRcd.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalDbASCIIIO.h"


class HcalQIETypeHandler : public popcon::PopConSourceHandler<HcalQIEType>
{
 public:
  void getNewObjects();
  std::string id() const { return m_name;}
  ~HcalQIETypeHandler();
  HcalQIETypeHandler(edm::ParameterSet const &);

  void initObject(HcalQIEType*);

 private:
  unsigned int sinceTime;
  edm::FileInPath fFile;
  HcalQIEType* myDBObject;
  std::string m_name;

};
#endif
