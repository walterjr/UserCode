#include "CondCore/PopCon/interface/PopConAnalyzer.h"
#include "CondTools/Hcal/interface/HcalQIETypeHandler.h"
#include "FWCore/Framework/interface/MakerMacros.h"

//typedef popcon::PopConAnalyzer<HcalQIETypeHandler> HcalQIETypePopConAnalyzer;

class HcalQIETypePopConAnalyzer: public popcon::PopConAnalyzer<HcalQIETypeHandler>
{
public:
  typedef HcalQIETypeHandler SourceHandler;

  HcalQIETypePopConAnalyzer(const edm::ParameterSet& pset): 
    popcon::PopConAnalyzer<HcalQIETypeHandler>(pset),
    m_populator(pset),
    m_source(pset.getParameter<edm::ParameterSet>("Source")) {}

private:
  virtual void endJob() override 
  {
    m_source.initObject(myDBObject);
    write();
  }

  virtual void analyze(const edm::Event& ev, const edm::EventSetup& esetup) override
  {
    //Using ES to get the data:

    edm::ESHandle<HcalQIEType> objecthandle;
    esetup.get<HcalQIETypeRcd>().get(objecthandle);
    myDBObject = new HcalQIEType(*objecthandle.product() );
  }

  void write() { m_populator.write(m_source); }

private:
  popcon::PopCon m_populator;
  SourceHandler m_source;

  HcalQIEType* myDBObject;
};

DEFINE_FWK_MODULE(HcalQIETypePopConAnalyzer);
