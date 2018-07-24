import root;
import pad_layout;

string topDir = "../../data_eos/";

include "../fills_samples.asy";
InitDataSets("short");

string stream = "DoubleEG";

//string alignment = "2018_07_17";
string alignment = "2018_07_24.3";

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

TGraph_errorBar = None;

//----------------------------------------------------------------------------------------------------

NewPad(false);
for (int ci : cols.keys)
	NewPadLabel(c_labels[ci]);

for (int fi : fill_data.keys)
{
	for (int dsi : fill_data[fi].datasets.keys)
	{
		NewRow();

		string dataset = fill_data[fi].datasets[dsi].tag;

		NewPadLabel(replace(dataset, "_", " "));

		for (int ci : cols.keys)
		{
			NewPad("$\xi_{\rm multi}$", "RMS of $y^*\ung{\mu m}$");

			string f = topDir + dataset + "/" + stream + "/alignment_" + alignment + "/do_fits.root";
			string on = "multiRPPlots/" + cols[ci] + "/g_vtx_y_RMS_vs_xi";
			RootObject hist = RootGetObject(f, on);

			string f = topDir + dataset + "/" + stream + "/alignment_" + alignment + "/do_fits.root";
			string on = "multiRPPlots/" + cols[ci] + "/g_vtx_y_RMS_vs_xi|ff";
			RootObject fit = RootGetObject(f, on);

			draw(scale(1., 1e3), hist, "d0,p,eb", red, mCi+1pt+red);
			draw(scale(1., 1e3), fit, blue+2pt);

			limits((0.00, 0), (0.16, +200), Crop);
		}
	}
}

GShipout(hSkip=1mm, vSkip=0mm);
