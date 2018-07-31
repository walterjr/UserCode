import root;
import pad_layout;

string topDir = "../../data_eos/";

include "../fills_samples.asy";
InitDataSets("short");

//fill_data.delete();
//AddDataSet("phys_no_margin/fill_5005");

string stream = "DoubleEG";

//string alignment = "2018_07_17";
//string alignment = "2018_07_24.3";
string alignment = "2018_07_30.5";

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

TGraph_errorBar = None;

//----------------------------------------------------------------------------------------------------

NewPad(false);
label("\vbox{\hbox{stream: " + stream + "}\hbox{alignment: " + replace(alignment, "_", "\_") + "}}");

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
			NewPad("$\xi_{\rm multi}$", "mean of $y^*\ung{\mu m}$");

			string f = topDir + dataset + "/" + stream + "/alignment_" + alignment + "/output.root";
			string on = "multiRPPlots/" + cols[ci] + "/p_vtx_y_vs_xi";
			RootObject hist = RootGetObject(f, on, error = false);

			if (!hist.valid)
				continue;

			string f = topDir + dataset + "/" + stream + "/alignment_" + alignment + "/do_fits.root";
			string on = "multiRPPlots/" + cols[ci] + "/p_vtx_y_vs_xi|ff";
			RootObject fit = RootGetObject(f, on);

			draw(scale(1., 1e3), hist, "d0,p,eb", red);
			draw(scale(1., 1e3), fit, blue+2pt);

			limits((0.00, -100), (0.16, +100), Crop);
		}
	}
}

GShipout(hSkip=0mm, vSkip=0mm);
