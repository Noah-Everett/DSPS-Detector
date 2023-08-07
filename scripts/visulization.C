void visulization() {
    gGeoManager->FindVolumeFast("calorimeter"       )->SetLineColor(kRed  );
    gGeoManager->FindVolumeFast("calorimeter_middle")->SetLineColor(kRed  );
    gGeoManager->FindVolumeFast("surface"           )->SetLineColor(kWhite);
    gGeoManager->FindVolumeFast("body"              )->SetLineColor(kWhite);
    gGeoManager->FindVolumeFast("lens_0"            )->SetLineColor(kBlue );
    gGeoManager->FindVolumeFast("lens_1"            )->SetLineColor(kBlue );
}