#include "TFile.h"
#include "TProfile.h"
#include "TGraphErrors.h"
#include "TF1.h"

#include <string>
#include <map>

using namespace std;

int main(int argc, char **argv)
{
  // parse command line
  string dir = argv[1];

  // config
  struct Record
  {
    string dir;
    double min;
    double max;
  };

  vector<Record> mu_records = {
    { "arm0", 0.06, 0.08 },
    { "arm1", 0.06, 0.10 },
  };

  vector<Record> si_mu_records = {
    { "si_rp2_mu_arm0", 0.085, 0.095 },
    { "si_rp3_mu_arm0", 0.085, 0.095 },
    { "si_rp102_mu_arm1", 0.08, 0.10 },
    { "si_rp103_mu_arm1", 0.08, 0.10 },
  };

  vector<Record> arm_records = {
    { "arm0", 0.06, 0.08 },
    { "arm1", 0.06, 0.10 },
  };

  TF1 *ff = new TF1("ff", "[0]");
  TF1 *ff_pol1 = new TF1("ff_pol1", "[0] + [1]*x");

  // prepare input
  TFile *f_in = TFile::Open((dir + "/output.root").c_str());
  if (!f_in)
  {
    printf("ERROR: can't open input.\n");
    return 1;
  }

  // prepare output
  TFile *f_out = TFile::Open((dir + "/do_fits.root").c_str(), "recreate");

  // process mu
  TDirectory *mu_dir = f_out->mkdir("multiRPPlots");
  for (const auto &rec : mu_records)
  {
    TProfile *p_x = (TProfile *) f_in->Get((string("multiRPPlots/") + rec.dir + "/p_th_x_vs_xi").c_str());
    TProfile *p_y = (TProfile *) f_in->Get((string("multiRPPlots/") + rec.dir + "/p_th_y_vs_xi").c_str());

    if (p_x == NULL || p_y == NULL)
    {
      printf("ERROR: cannot load from directory '%s'.\n", rec.dir.c_str());
      return 2;
    }

    ff->SetParameter(0, 0.);
    p_x->Fit(ff, "Q", "", rec.min, rec.max);

    ff_pol1->SetParameters(0., 0.);
    p_x->Fit(ff_pol1, "Q+", "", rec.min, rec.max);

    double x_min = 0., x_max = 0.;
    if (rec.dir == "arm0") { x_min = 0.08; x_max = 0.11; }
    if (rec.dir == "arm1") { x_min = 0.07; x_max = 0.12; }

    ff->SetParameter(0, 0.);
    p_y->Fit(ff, "Q", "", x_min, x_max);

    ff_pol1->SetParameters(0., 0.);
    p_y->Fit(ff_pol1, "Q+", "", x_min, x_max);

    gDirectory = mu_dir->mkdir(rec.dir.c_str());
    p_x->Write("p_th_x_vs_xi");
    p_y->Write("p_th_y_vs_xi");

    //----------

    TGraphErrors *g_th_x_RMS_vs_xi = (TGraphErrors *) f_in->Get((string("multiRPPlots/") + rec.dir + "/g_th_x_RMS_vs_xi").c_str());
    TGraphErrors *g_th_y_RMS_vs_xi = (TGraphErrors *) f_in->Get((string("multiRPPlots/") + rec.dir + "/g_th_y_RMS_vs_xi").c_str());

    ff->SetParameter(0, 0.);
    g_th_x_RMS_vs_xi->Fit(ff, "Q", "", rec.min, rec.max);

    ff->SetParameter(0, 0.);
    g_th_y_RMS_vs_xi->Fit(ff, "Q", "", rec.min, rec.max);

    g_th_x_RMS_vs_xi->Write("g_th_x_RMS_vs_xi");
    g_th_y_RMS_vs_xi->Write("g_th_y_RMS_vs_xi");

    //----------

    TGraphErrors *g_vtx_y_RMS_vs_xi = (TGraphErrors *) f_in->Get((string("multiRPPlots/") + rec.dir + "/g_vtx_y_RMS_vs_xi").c_str());

    ff->SetParameter(0, 0.);
    g_vtx_y_RMS_vs_xi->Fit(ff, "Q", "", rec.min, rec.max);

    g_vtx_y_RMS_vs_xi->Write("g_vtx_y_RMS_vs_xi");
  }

  // process si_mu
  TDirectory *si_mu_dir = f_out->mkdir("singleMultiCorrelationPlots");
  for (const auto &rec : si_mu_records)
  {
    TProfile *p = (TProfile *) f_in->Get((string("singleMultiCorrelationPlots/") + rec.dir + "/p_xi_diff_si_mu_vs_xi_mu").c_str());

    if (p == NULL)
    {
      printf("ERROR: cannot load from directory '%s'.\n", rec.dir.c_str());
      return 2;
    }

    ff->SetParameter(0, 0.);

    p->Fit(ff, "Q", "", rec.min, rec.max);

    gDirectory = si_mu_dir->mkdir(rec.dir.c_str());
    p->Write("p_xi_diff_si_mu_vs_xi_mu");
  }

  // process arm
  TDirectory *arm_dir = f_out->mkdir("armCorrelationPlots");
  for (const auto &rec : arm_records)
  {
    TProfile *p = (TProfile *) f_in->Get((string("armCorrelationPlots/") + rec.dir + "/p_xi_si_diffNF_vs_xi_mu").c_str());

    if (p == NULL)
    {
      printf("ERROR: cannot load from directory '%s'.\n", rec.dir.c_str());
      return 2;
    }

    ff->SetParameter(0, 0.);
    p->Fit(ff, "Q", "", rec.min, rec.max);

    gDirectory = arm_dir->mkdir(rec.dir.c_str());
    p->Write("p_xi_si_diffNF_vs_xi_mu");
  }

  // clean up
  delete f_in;
  delete f_out;

  return 0;
}
