void visulization() {
    auto temp = gGeoManager->FindVolumeFast("calorimeter");
    if( temp )
        temp->SetLineColor(kRed);
    else
        cout << "calorimeter not found" << endl;

    temp = gGeoManager->FindVolumeFast("calorimeter_middle");
    if( temp )
        temp->SetLineColor(kRed);
    else
        cout << "calorimeter_middle not found" << endl;

    temp = gGeoManager->FindVolumeFast("surface");
    if( temp )
        temp->SetLineColor(kWhite);
    else
        cout << "surface not found" << endl;

    temp = gGeoManager->FindVolumeFast("body");
    if( temp )
        temp->SetLineColor(kWhite);
    else
        cout << "body not found" << endl;

    temp = gGeoManager->FindVolumeFast("lens_0");
    if( temp )
        temp->SetLineColor(kBlue);
    else
        cout << "lens_0 not found" << endl;

    temp = gGeoManager->FindVolumeFast("lens_1");
    if( temp )
        temp->SetLineColor(kBlue);
    else
        cout << "lens_1 not found" << endl;
}