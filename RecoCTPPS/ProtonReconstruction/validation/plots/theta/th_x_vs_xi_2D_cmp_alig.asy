import root;
import pad_layout;

string topDir = "../../data/";

string alignments[], a_labels[];
alignments.push("2017_01_17"); a_labels.push("2017-01-17");
alignments.push("2018_07_17"); a_labels.push("2018-07-17");

string cols[], c_labels[];
cols.push("arm0"); c_labels.push("sector 45 (L)");
cols.push("arm1"); c_labels.push("sector 56 (R)");

//string dataset = "fill_5029/run_275371";
string dataset = "fill_5261/run_279766";

/*
	// B
	"fill_4947/run_273730",
	//"fill_4953/run_274094",
	//"fill_4961/run_274199",
	//"fill_4964/run_274241",
	//"fill_4976/run_274284",
	//"fill_4964/run_274244",
	//"fill_4985/run_274388",
	"fill_4988/run_274422",
	//"fill_4990/run_274442",
	//"fill_5005/run_274958",
	//"fill_5013/run_274969",
	//"fill_5017/run_274999",
	//"fill_5020/run_275068",
	//"fill_5021/run_275125",
	//"fill_5024/run_275291",
	//"fill_5026/run_275310",
	"fill_5027/run_275337",
	//"fill_5028/run_275345",
	//"fill_5029/run_275371",
	//"fill_5030/run_275376",

	// C
	//"fill_5038/run_275658",
	//"fill_5043/run_275783",
	//"fill_5045/run_275836",
	//"fill_5048/run_275890",
	"fill_5052/run_275913",

	// G
	"fill_5261/run_279766",
	//"fill_5264/run_279794",
	//"fill_5265/run_279823",
	//"fill_5266/run_279841",
	//"fill_5267/run_279849",
	//"fill_5274/run_279931",
	"fill_5275/run_279966",
	//"fill_5276/run_279975",
	//"fill_5277/run_280018",
	//"fill_5279/run_280191",
	//"fill_5287/run_280330",
	"fill_5288/run_280385",
};
*/

TH2_palette = Gradient(blue, heavygreen, yellow, red);

//----------------------------------------------------------------------------------------------------

NewPad(false);
for (int ci : cols.keys)
	NewPadLabel(c_labels[ci]);


for (int ai : alignments.keys)
{
	NewRow();

	NewPadLabel(a_labels[ai]);

	for (int ci : cols.keys)
	{
		NewPad("$\xi_{\rm multi}$", "$\th^*_x\ung{\mu rad}$");

		string f = topDir + dataset + "/alignment_" + alignments[ai] + "/output.root";
		string on = "multiRPPlots/" + cols[ci] + "/h2_th_x_vs_xi";

		RootObject obj = RootGetObject(f, on);
		obj.vExec("Rebin2D", 2, 2);

		draw(scale(1., 1e6), obj);

		limits((0.00, -300), (0.16, +300), Crop);
	}
}

GShipout(hSkip=0mm, vSkip=0mm);
