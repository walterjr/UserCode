import root;
import pad_layout;

string topDir = "../../data_eos/";

string dataset = "phys_margin/fill_4947/DoubleEG/alignment_2018_07_24.3";
//string dataset = "phys_no_margin/fill_5261/DoubleEG/alignment_2018_07_24.3";

string f = topDir + dataset + "/output.root";

xSizeDef = 10cm;
xTicksDef = LeftTicks(0.05, 0.01);

//----------------------------------------------------------------------------------------------------

NewRow();

NewPad(false);
label("{\SetFontSizesXX sector 45}");

NewPad("$\xi$");
AddToLegend("<single-RP reconstruction:");
draw(RootGetObject(f, "singleRPPlots/rp2/h_xi"), "vl", blue, "near");
draw(RootGetObject(f, "singleRPPlots/rp3/h_xi"), "vl", heavygreen, "far");
AddToLegend("<multi-RP reconstruction:");
draw(RootGetObject(f, "multiRPPlots/arm0/h_xi"), "vl", red, "near + far");
AttachLegend(NW, NE);

//----------------------------------------------------------------------------------------------------

NewRow();

NewPad(false);
label("{\SetFontSizesXX sector 56}");

NewPad("$\xi$");
AddToLegend("<single-RP reconstruction:");
draw(RootGetObject(f, "singleRPPlots/rp102/h_xi"), "vl", blue, "near");
draw(RootGetObject(f, "singleRPPlots/rp103/h_xi"), "vl", heavygreen, "far");
AddToLegend("<multi-RP reconstruction:");
draw(RootGetObject(f, "multiRPPlots/arm1/h_xi"), "vl", red, "near + far");
AttachLegend(NW, NE);

//----------------------------------------------------------------------------------------------------

GShipout(vSkip=1mm);
