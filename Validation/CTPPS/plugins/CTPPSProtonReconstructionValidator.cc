/****************************************************************************
 *
 * This is a part of CTPPS validation software
 * Authors:
 *   Jan Kašpar
 *
 ****************************************************************************/


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "DataFormats/ProtonReco/interface/ProtonTrack.h"

#include "TFile.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TGraphErrors.h"

#include <map>
#include <string>

//----------------------------------------------------------------------------------------------------

class CTPPSProtonReconstructionValidator : public edm::one::EDAnalyzer<>
{
  public:
    explicit CTPPSProtonReconstructionValidator( const edm::ParameterSet& );
    ~CTPPSProtonReconstructionValidator();

  private:
    virtual void beginJob() override;

    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    virtual void endJob() override;

    void FillPlots(unsigned int meth_idx, unsigned int idx, const reco::ProtonTrack &rec_pr,
      const HepMC::FourVector &vtx, const HepMC::FourVector &mom);

    edm::EDGetTokenT<edm::HepMCProduct> tokenHepMCBeforeSmearing;
    edm::EDGetTokenT<edm::HepMCProduct> tokenHepMCAfterSmearing;
    edm::EDGetTokenT<std::vector<reco::ProtonTrack>> tokenRecoProtons;

    std::string outputFile;

    struct PlotGroup
    {
      TH1D *h_de_xi = NULL;
      TProfile *p_de_xi_vs_xi_simu;

      void Init()
      {
        h_de_xi = new TH1D("", ";#xi_{reco} - #xi_{simu}", 100, 0., 0.);
        p_de_xi_vs_xi_simu = new TProfile("", ";#xi_{simu};#xi_{reco} - #xi_{simu}", 19, 0.015, 0.205);
      }

      static TGraphErrors* ProfileToRMSGraph(TProfile *p, const std::string &name = "")
      {
          TGraphErrors *g = new TGraphErrors();
          g->SetName(name.c_str());

          for (int bi = 1; bi <= p->GetNbinsX(); ++bi)
          {
              double c = p->GetBinCenter(bi);
              double w = p->GetBinWidth(bi);

              double N = p->GetBinEntries(bi);
              double Sy = p->GetBinContent(bi) * N;
              double Syy = p->GetSumw2()->At(bi);

              double si_sq = Syy/N - Sy*Sy/N/N;
              double si = (si_sq >= 0.) ? sqrt(si_sq) : 0.;
              double si_unc_sq = si_sq / 2. / N;	// Gaussian approximation
              double si_unc = (si_unc_sq >= 0.) ? sqrt(si_unc_sq) : 0.;

              int idx = g->GetN();
              g->SetPoint(idx, c, si);
              g->SetPointError(idx, w/2., si_unc);
          }

          return g;
      }

      void Write() const
      {
        h_de_xi->Write("h_de_xi");
        p_de_xi_vs_xi_simu->Write("p_de_xi_vs_xi_simu");
        ProfileToRMSGraph(p_de_xi_vs_xi_simu, "g_rms_de_xi_vs_xi_simu")->Write();
      }
    };

    std::map<unsigned int, std::map<unsigned int, PlotGroup>> plots;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;
using namespace HepMC;

//----------------------------------------------------------------------------------------------------

CTPPSProtonReconstructionValidator::CTPPSProtonReconstructionValidator(const edm::ParameterSet& iConfig) :
  tokenHepMCBeforeSmearing( consumes<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("tagHepMCBeforeSmearing")) ),
  tokenHepMCAfterSmearing( consumes<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("tagHepMCAfterSmearing")) ),
  tokenRecoProtons( consumes<std::vector<reco::ProtonTrack>>(iConfig.getParameter<edm::InputTag>("tagRecoProtons")) ),
  outputFile(iConfig.getParameter<string>("outputFile"))
{
}

//----------------------------------------------------------------------------------------------------

CTPPSProtonReconstructionValidator::~CTPPSProtonReconstructionValidator()
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionValidator::analyze(const edm::Event& iEvent, const edm::EventSetup&)
{
  // get input
  edm::Handle<edm::HepMCProduct> hHepMCBeforeSmearing;
  iEvent.getByToken(tokenHepMCBeforeSmearing, hHepMCBeforeSmearing);
  HepMC::GenEvent *hepMCEventBeforeSmearing = (HepMC::GenEvent *) hHepMCBeforeSmearing->GetEvent();

  edm::Handle<edm::HepMCProduct> hHepMCAfterSmearing;
  iEvent.getByToken(tokenHepMCAfterSmearing, hHepMCAfterSmearing);
  HepMC::GenEvent *hepMCEventAfterSmearing = (HepMC::GenEvent *) hHepMCAfterSmearing->GetEvent();

  edm::Handle<vector<reco::ProtonTrack>> hRecoProtons;
  iEvent.getByToken(tokenRecoProtons, hRecoProtons);

  // extract vertex position
  bool vertex_set = false;
  FourVector vtx;
  for (auto it = hepMCEventAfterSmearing->vertices_begin(); it != hepMCEventAfterSmearing->vertices_end(); ++it)
  {
    if (vertex_set)
    {
      printf("ERROR: multiple vertices found.\n");
      return;
    }

    vertex_set = true;
    vtx = (*it)->position();
  }

  // extract protons
  bool proton_45_set = false;
  bool proton_56_set = false;
  FourVector mom_45, mom_56;

  for (auto it = hepMCEventBeforeSmearing->particles_begin(); it != hepMCEventBeforeSmearing->particles_end(); ++it)
  {
    const auto &mom = (*it)->momentum();

    if (mom.z() > 0)
    {
      // 45
      if (proton_45_set)
      {
        printf("ERROR: multiple protons in sector 45 found.\n");
        return;
      }

      proton_45_set = true;
      mom_45 = mom;
    } else {
      // 56
      if (proton_56_set)
      {
        printf("ERROR: multiple protons in sector 56 found.\n");
        return;
      }

      proton_56_set = true;
      mom_56 = mom;
    }
  }

  // do comparison
  for (const auto &rec_pr : *hRecoProtons)
  {
    if (! rec_pr.valid())
      continue;

    unsigned int idx;

    bool mom_set = false;
    FourVector mom;

    if (rec_pr.lhcSector == reco::ProtonTrack::sector45)
    {
      idx = 0;
      mom_set = proton_45_set;
      mom = mom_45;
    } else {
      idx = 1;
      mom_set = proton_56_set;
      mom = mom_56;
    }

    if (! mom_set)
      continue;

    unsigned int meth_idx;

    if (rec_pr.method == reco::ProtonTrack::rmSingleRP)
    {
      meth_idx = 0;

      CTPPSDetId rpId(* rec_pr.contributingRPIds.begin());
      idx = 100*rpId.arm() + 10*rpId.station() + rpId.rp();
    } else {
      meth_idx = 1;
    }

    FillPlots(meth_idx, idx, rec_pr, vtx, mom);
  }
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionValidator::FillPlots(unsigned int meth_idx, unsigned int idx, const reco::ProtonTrack &rec_pr,
      const HepMC::FourVector &vtx, const HepMC::FourVector &mom)
{
  const double p_nom = 6500.;
  const double xi_simu = (p_nom - mom.e()) / p_nom;

  const double xi_reco = rec_pr.xi();

  if (meth_idx == 1 && fabs(xi_reco - xi_simu) > 0.01)
    printf("   PROBLEM: xi diff too large\n");

  auto &p = plots[meth_idx][idx];
  if (p.h_de_xi == NULL)
    p.Init();

  p.h_de_xi->Fill(xi_reco - xi_simu);
  p.p_de_xi_vs_xi_simu->Fill(xi_simu, xi_reco - xi_simu);
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionValidator::beginJob()
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionValidator::endJob()
{
  TFile *f_out = TFile::Open(outputFile.c_str(), "recreate");

  for (const auto &mit : plots)
  {
    string method = (mit.first == 0) ? "single rp" : "multi rp";
    TDirectory *d_method = f_out->mkdir(method.c_str());

    for (const auto &eit : mit.second)
    {
      char buf[20];
      sprintf(buf, "%i", eit.first);
    
      TDirectory *d_element = d_method->mkdir(buf);
      
      gDirectory = d_element;
      eit.second.Write();
    }
  }

  delete f_out;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(CTPPSProtonReconstructionValidator);
