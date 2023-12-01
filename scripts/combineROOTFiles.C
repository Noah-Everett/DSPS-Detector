#include <cstdlib>  // Include for the system() function

void combineROOTFiles(vector<string> t_inputFilePaths, string t_outputFilePath) {
    // vector<string> tuples = {"photoSensor_hits", "photon", "primary"};
    // vector<TChain*> chains;

    // for (auto &tuple : tuples) {
    //     chains.push_back(new TChain(tuple.c_str()));
    //     for (auto &inputFilePath : t_inputFilePaths) {
    //         chains.back()->Add(inputFilePath.c_str());
    //     }
    // }

    // // Create output file
    // TFile *outputFile = new TFile(t_outputFilePath.c_str(), "RECREATE");

    // // Clone the tree and copy entries
    // for (auto &chain : chains) {
    //     TTree *outputTree = chain->CloneTree(-1, "fast");
    // }

    // Merge histograms
    string haddCommand = "hadd -f " + t_outputFilePath + " ";
    for (auto &inputFilePath : t_inputFilePaths) {
        haddCommand += inputFilePath + " ";
    }
    // gROOT->ProcessLine(haddCommand.c_str());
    system(haddCommand.c_str());

    // // Close files
    // outputFile->Write();
    // outputFile->Close();
}