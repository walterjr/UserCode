#ifndef HcalQIEType_h
#define HcalQIEType_h

/*
\class HcalQIEType
\author Walter Alda
POOL object to store Hcal QIEType
*/

#include "CondFormats/Serialization/interface/Serializable.h"

#include "CondFormats/HcalObjects/interface/HcalCondObjectContainer.h"
#include "CondFormats/HcalObjects/interface/HcalQIETyp.h"

//typedef HcalCondObjectContainer<HcalQIETyp> HcalQIEType;

class HcalQIEType: public HcalCondObjectContainer<HcalQIETyp>
{
 public:
#ifndef HCAL_COND_SUPPRESS_DEFAULT
  HcalQIEType():HcalCondObjectContainer<HcalQIETyp>(0) {}
#endif
  HcalQIEType(const HcalTopology* topo):HcalCondObjectContainer<HcalQIETyp>(topo) {}

  std::string myname() const {return (std::string)"HcalRespCorrs";}

 private:

 COND_SERIALIZABLE;
};

#endif
