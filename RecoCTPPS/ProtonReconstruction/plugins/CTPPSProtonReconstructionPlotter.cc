/****************************************************************************
*
* Authors:
*  Jan Kašpar (jan.kaspar@gmail.com)
*
****************************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"

#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/ProtonReco/interface/ProtonTrack.h"

#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

//----------------------------------------------------------------------------------------------------

class CTPPSProtonReconstructionPlotter : public edm::one::EDAnalyzer<>
{
  public:
    explicit CTPPSProtonReconstructionPlotter(const edm::ParameterSet&);
    ~CTPPSProtonReconstructionPlotter() {}

  private:
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    virtual void endJob() override;

    edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite>> tokenTracks;
    edm::EDGetTokenT<std::vector<reco::ProtonTrack>> tokenRecoProtons;

    std::string outputFile;

    struct SingleRPPlots
    {
      TH1D *h_xi = NULL;

      void Init()
      {
        h_xi = new TH1D("", ";#xi", 100, 0., 0.2);
      }

      void Fill(const reco::ProtonTrack &p)
      {
        if (!h_xi)
          Init();

        if (p.valid())
        {
          h_xi->Fill(p.xi());
        }
      }

      void Write() const
      {
        h_xi->Write("h_xi");
      }
    };

    std::map<unsigned int, SingleRPPlots> singleRPPlots;

    struct MultiRPPlots
    {
      TH1D *h_xi=NULL, *h_th_x=NULL, *h_th_y=NULL, *h_vtx_y=NULL, *h_chi_sq=NULL, *h_chi_sq_norm=NULL;
      TH2D *h2_th_x_vs_xi = NULL, *h2_th_y_vs_xi = NULL;
      TProfile *p_th_x_vs_xi = NULL, *p_th_y_vs_xi = NULL;

      void Init()
      {
        h_xi = new TH1D("", ";#xi", 100, 0., 0.2);

        h_th_x = new TH1D("", ";#theta_{x}   (rad)", 100, -500E-6, +500E-6);
        h_th_y = new TH1D("", ";#theta_{y}   (rad)", 100, -500E-6, +500E-6);

        h_vtx_y = new TH1D("", ";vtx_{y}   (mm)", 100, -2., +2.);

        h_chi_sq = new TH1D("", ";#chi^{2}", 100, 0., 0.);
        h_chi_sq_norm = new TH1D("", ";#chi^{2}/ndf", 100, 0., 5.);

        h2_th_x_vs_xi = new TH2D("", ";#xi;#theta_{x}   (rad)", 100, 0., 0.2, 100, -500E-6, +500E-6);
        h2_th_y_vs_xi = new TH2D("", ";#xi;#theta_{y}   (rad)", 100, 0., 0.2, 100, -500E-6, +500E-6);

        p_th_x_vs_xi = new TProfile("", ";#xi;#theta_{x}   (rad)", 100, 0., 0.2);
        p_th_y_vs_xi = new TProfile("", ";#xi;#theta_{y}   (rad)", 100, 0., 0.2);
      }

      void Fill(const reco::ProtonTrack &p)
      {
        if (!h_xi)
          Init();

        if (p.valid())
        {
          const double th_x = p.direction().x() / p.direction().mag();
          const double th_y = p.direction().y() / p.direction().mag();

          h_xi->Fill(p.xi());

          h_th_x->Fill(th_x);
          h_th_y->Fill(th_y);

          h_vtx_y->Fill(p.vertex().y());

          h_chi_sq->Fill(p.fitChiSq);

          if (p.fitNDF > 0)
            h_chi_sq_norm->Fill(p.fitChiSq / p.fitNDF);

          h2_th_x_vs_xi->Fill(p.xi(), th_x);
          h2_th_y_vs_xi->Fill(p.xi(), th_y);

          p_th_x_vs_xi->Fill(p.xi(), th_x);
          p_th_y_vs_xi->Fill(p.xi(), th_y);
        }
      }

      void Write() const
      {
        h_xi->Write("h_xi");
        h_th_x->Write("h_th_x");
        h_th_y->Write("h_th_y");
        h_vtx_y->Write("h_vtx_y");
        h_chi_sq->Write("h_chi_sq");
        h_chi_sq_norm->Write("h_chi_sq_norm");

        h2_th_x_vs_xi->Write("h2_th_x_vs_xi");
        h2_th_y_vs_xi->Write("h2_th_y_vs_xi");

        p_th_x_vs_xi->Write("p_th_x_vs_xi");
        p_th_y_vs_xi->Write("p_th_y_vs_xi");
      }
    };

    std::map<unsigned int, MultiRPPlots> multiRPPlots;

    struct SingleMultiCorrelationPlots
    {
      TH2D *h2_xi_mu_vs_xi_si = NULL;
      TH1D *h_xi_diff_mu_si = NULL;
      TH1D *h_xi_diff_si_mu = NULL;
      TProfile *p_xi_diff_si_mu_vs_xi_mu = NULL;

      void Init()
      {
        h2_xi_mu_vs_xi_si = new TH2D("", ";#xi_{single};#xi_{multi}", 100, 0., 0.2, 100, 0., 0.2);
        h_xi_diff_mu_si = new TH1D("", ";#xi_{multi} - #xi_{single}", 100, -0.1, +0.1);
        h_xi_diff_si_mu = new TH1D("", ";#xi_{single} - #xi_{multi}", 100, -0.1, +0.1);
        p_xi_diff_si_mu_vs_xi_mu = new TProfile("", ";#xi_{multi};#xi_{single} - #xi_{multi}", 100, 0., 0.2);
      }

      void Fill(const reco::ProtonTrack &p_single, const reco::ProtonTrack &p_multi)
      {
        if (!h2_xi_mu_vs_xi_si)
          Init();

        if (p_single.valid() && p_multi.valid())
        {
          h2_xi_mu_vs_xi_si->Fill(p_single.xi(), p_multi.xi());
          h_xi_diff_mu_si->Fill(p_multi.xi() - p_single.xi());
          h_xi_diff_si_mu->Fill(p_single.xi() - p_multi.xi());
          p_xi_diff_si_mu_vs_xi_mu->Fill(p_multi.xi(), p_single.xi() - p_multi.xi());
        }
      }

      void Write() const
      {
        h2_xi_mu_vs_xi_si->Write("h2_xi_mu_vs_xi_si");
        h_xi_diff_mu_si->Write("h_xi_diff_mu_si");
        h_xi_diff_si_mu->Write("h_xi_diff_si_mu");
        p_xi_diff_si_mu_vs_xi_mu->Write("p_xi_diff_si_mu_vs_xi_mu");
      }
    };

    std::map<unsigned int, SingleMultiCorrelationPlots> singleMultiCorrelationPlots;

    struct ArmCorrelationPlots
    {
      TH1D *h_xi_si_diffNF = NULL;
      TProfile *p_xi_si_diffNF_vs_xi_mu = NULL;

      void Init()
      {
        h_xi_si_diffNF = new TH1D("", ";#xi_{sF} - #xi_{sN}", 100, -0.02, +0.02);
        p_xi_si_diffNF_vs_xi_mu = new TProfile("", ";#xi_{m};#xi_{sF} - #xi_{sN}", 100, 0., 0.2);
      }

      void Fill(const reco::ProtonTrack &p_s_N, const reco::ProtonTrack &p_s_F, const reco::ProtonTrack &p_m)
      {
        if (!h_xi_si_diffNF)
          Init();

        if (p_s_N.valid() && p_s_F.valid() && p_m.valid())
        {
          h_xi_si_diffNF->Fill(p_s_F.xi() - p_s_N.xi());
          p_xi_si_diffNF_vs_xi_mu->Fill(p_m.xi(), p_s_F.xi() - p_s_N.xi());
        }
      }

      void Write() const
      {
        h_xi_si_diffNF->Write("h_xi_si_diffNF");
        p_xi_si_diffNF_vs_xi_mu->Write("p_xi_si_diffNF_vs_xi_mu");
      }
    };

    std::map<unsigned int, ArmCorrelationPlots> armCorrelationPlots;

    TH1D *h_de_x_f_n_L, *h_de_x_f_n_R;

    unsigned int n_non_empty_events;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

CTPPSProtonReconstructionPlotter::CTPPSProtonReconstructionPlotter(const edm::ParameterSet &ps) :
  tokenTracks(consumes< std::vector<CTPPSLocalTrackLite>>(ps.getParameter<edm::InputTag>("tagTracks"))),
  tokenRecoProtons(consumes<std::vector<reco::ProtonTrack>>(ps.getParameter<InputTag>("tagRecoProtons"))),
  outputFile(ps.getParameter<string>("outputFile"))
{
  h_de_x_f_n_L = new TH1D("h_de_x_f_n_L", ";x_{LF} - x_{LN}   (mm)", 100, -3., +3.);
  h_de_x_f_n_R = new TH1D("h_de_x_f_n_R", ";x_{RF} - x_{RN}   (mm)", 100, -3., +3.);

  n_non_empty_events = 0;
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionPlotter::analyze(const edm::Event &event, const edm::EventSetup&)
{
  // get input
  edm::Handle< std::vector<CTPPSLocalTrackLite> > tracks;
  event.getByToken(tokenTracks, tracks);

  Handle<vector<reco::ProtonTrack>> recoProtons;
  event.getByToken(tokenRecoProtons, recoProtons);

  if (recoProtons->size() > 0)
    n_non_empty_events++;

  // track plots
  const CTPPSLocalTrackLite *tr_L_N = NULL;
  const CTPPSLocalTrackLite *tr_L_F = NULL;
  const CTPPSLocalTrackLite *tr_R_N = NULL;
  const CTPPSLocalTrackLite *tr_R_F = NULL;

  for (const auto &tr : *tracks)
  {
    CTPPSDetId rpId(tr.getRPId());
    unsigned int decRPId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();

    if (decRPId == 2) tr_L_N = &tr;
    if (decRPId == 3) tr_L_F = &tr;
    if (decRPId == 102) tr_R_N = &tr;
    if (decRPId == 103) tr_R_F = &tr;
  }

  bool x_correlation_L = false;
  bool x_correlation_R = false;

  if (tr_L_N && tr_L_F)
  {
    const double de = tr_L_F->getX() - tr_L_N->getX();
    h_de_x_f_n_L->Fill(de);

    if (fabs(de - 1.18) < 2. * 0.22)
      x_correlation_L = true;
  }

  if (tr_R_N && tr_R_F)
  {
    const double de = tr_R_F->getX() - tr_R_N->getX();
    h_de_x_f_n_R->Fill(de);

    if (fabs(de + 0.81) < 2. * 0.24)
      x_correlation_R = true;
  }

  // make single-RP-reco plots
  for (const auto & proton : *recoProtons)
  {
    if (proton.method == reco::ProtonTrack::rmSingleRP)
    {
      CTPPSDetId rpId(* proton.contributingRPIds.begin());
      unsigned int decRPId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();
      singleRPPlots[decRPId].Fill(proton);
    }
  }

  // make multi-RP-reco plots
  for (const auto & proton : *recoProtons)
  {
    bool x_correlation = (proton.lhcSector == reco::ProtonTrack::sector45) ? x_correlation_L : x_correlation_R;

    if (!x_correlation)
      continue;

    if (proton.method == reco::ProtonTrack::rmMultiRP)
    {
      CTPPSDetId rpId(* proton.contributingRPIds.begin());
      unsigned int armId = rpId.arm();
      multiRPPlots[armId].Fill(proton);
    }
  }

  // make correlation plots
  for (unsigned int i = 0; i < recoProtons->size(); ++i)
  {
    for (unsigned int j = 0; j < recoProtons->size(); ++j)
    {
      const reco::ProtonTrack &pi = (*recoProtons)[i];
      const reco::ProtonTrack &pj = (*recoProtons)[j];

      if (pi.method != reco::ProtonTrack::rmSingleRP || pj.method != reco::ProtonTrack::rmMultiRP)
        continue;

      // only compare object from the same arm
      CTPPSDetId i_rpId(* pi.contributingRPIds.begin());
      CTPPSDetId j_rpId(* pj.contributingRPIds.begin());

      if (i_rpId.arm() != j_rpId.arm())
        continue;

      // build index
      const unsigned int idx = i_rpId.arm()*1000 + i_rpId.station()*100 + i_rpId.rp()*10 + j_rpId.arm();

      // fill plots
      singleMultiCorrelationPlots[idx].Fill(pi, pj);
    }
  }

  // arm correlation plots
  const reco::ProtonTrack *p_arm0_s_N = NULL, *p_arm0_s_F = NULL, *p_arm0_m = NULL;
  const reco::ProtonTrack *p_arm1_s_N = NULL, *p_arm1_s_F = NULL, *p_arm1_m = NULL;

  for (const auto & proton : *recoProtons)
  {
    CTPPSDetId rpId(* proton.contributingRPIds.begin());
    unsigned int rpDecId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();

    if (proton.method == reco::ProtonTrack::rmMultiRP && proton.lhcSector == reco::ProtonTrack::sector45) p_arm0_m = &proton;
    if (proton.method == reco::ProtonTrack::rmMultiRP && proton.lhcSector == reco::ProtonTrack::sector56) p_arm1_m = &proton;

    if (proton.method == reco::ProtonTrack::rmSingleRP && rpDecId == 2) p_arm0_s_N = &proton;
    if (proton.method == reco::ProtonTrack::rmSingleRP && rpDecId == 3) p_arm0_s_F = &proton;

    if (proton.method == reco::ProtonTrack::rmSingleRP && rpDecId == 102) p_arm1_s_N = &proton;
    if (proton.method == reco::ProtonTrack::rmSingleRP && rpDecId == 103) p_arm1_s_F = &proton;
  }

  if (p_arm0_s_N && p_arm0_s_F && p_arm0_m)
    armCorrelationPlots[0].Fill(*p_arm0_s_N, *p_arm0_s_F, *p_arm0_m);

  if (p_arm1_s_N && p_arm1_s_F && p_arm1_m)
    armCorrelationPlots[1].Fill(*p_arm1_s_N, *p_arm1_s_F, *p_arm1_m);
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstructionPlotter::endJob()
{
  printf(">> CTPPSProtonReconstructionPlotter: n_non_empty_events = %u\n", n_non_empty_events);

  TFile *f_out = TFile::Open(outputFile.c_str(), "recreate");

  h_de_x_f_n_L->Write();
  h_de_x_f_n_R->Write();

  TDirectory *d_singleRPPlots = f_out->mkdir("singleRPPlots");
  for (const auto it : singleRPPlots)
  {
    char buf[100];
    sprintf(buf, "rp%u", it.first);
    gDirectory = d_singleRPPlots->mkdir(buf);
    it.second.Write();
  }

  TDirectory *d_multiRPPlots = f_out->mkdir("multiRPPlots");
  for (const auto it : multiRPPlots)
  {
    char buf[100];
    sprintf(buf, "arm%u", it.first);
    gDirectory = d_multiRPPlots->mkdir(buf);
    it.second.Write();
  }

  TDirectory *d_singleMultiCorrelationPlots = f_out->mkdir("singleMultiCorrelationPlots");
  for (const auto it : singleMultiCorrelationPlots)
  {
    unsigned int si_rp = it.first / 10;
    unsigned int mu_arm = it.first % 10;

    char buf[100];
    sprintf(buf, "si_rp%u_mu_arm%u", si_rp, mu_arm);
    gDirectory = d_singleMultiCorrelationPlots->mkdir(buf);
    it.second.Write();
  }

  TDirectory *d_armCorrelationPlots = f_out->mkdir("armCorrelationPlots");
  for (const auto it : armCorrelationPlots)
  {
    unsigned int arm = it.first;

    char buf[100];
    sprintf(buf, "arm%u", arm);
    gDirectory = d_armCorrelationPlots->mkdir(buf);
    it.second.Write();
  }

  delete f_out;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(CTPPSProtonReconstructionPlotter);
