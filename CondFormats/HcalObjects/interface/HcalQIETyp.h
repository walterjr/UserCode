#ifndef HcalQIETyp_h
#define HcalQIETyp_h

#include "CondFormats/Serialization/interface/Serializable.h"

/*
\class HcalRespCorr
\author Walter Alda 
contains the QIE Type + corresponding DetId
*/

class HcalQIETyp
{
 public:
  HcalQIETyp(): mId(0), mValue(0) {}
  HcalQIETyp(unsigned long fid, float value): mId(fid), mValue(value) {}

  uint32_t rawId() const {return mId;}

  int getValue() const {return mValue;}

 private:
  uint32_t mId;
  int mValue;

 COND_SERIALIZABLE;
};

#endif
