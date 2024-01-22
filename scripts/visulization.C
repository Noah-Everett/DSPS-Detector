void visulization(string filename = "geometry.gdml") {
    // Import geometry from the GDML file
    TGeoManager::Import(filename.c_str());
    TGeoVolume *top = gGeoManager->GetTopVolume();

    // Set visualization properties for different volumes
    TObjArray *allVolumes = gGeoManager->GetListOfVolumes();
    std::map<std::string, TGeoVolume*> volumeNames;
    for (int i = 0; i < allVolumes->GetEntries(); ++i) {
        TGeoVolume *vol = (TGeoVolume*)allVolumes->At(i);
        volumeNames[vol->GetName()] = vol;
    }

    for (auto &vol : volumeNames) {
        if (vol.first.find("lens") != std::string::npos) {
            vol.second->SetLineColor(kBlue);
            vol.second->SetTransparency(kMaxUChar);
            vol.second->SetFillColorAlpha(kBlue, 100);
            std::cout << "Setting " << vol.first << " to blue" << std::endl;
        }
        else if (vol.first.find("calorimeter") != std::string::npos) {
            vol.second->SetLineColor(kRed);
            vol.second->SetTransparency(kMaxUChar);
            vol.second->SetFillColorAlpha(kRed, 100);
            std::cout << "Setting " << vol.first << " to red" << std::endl;
        }
        else if (vol.first.find("surface") != std::string::npos || vol.first.find("body") != std::string::npos) {
            vol.second->SetLineColor(kWhite);
            vol.second->SetTransparency(kMaxUChar);
            vol.second->SetFillColorAlpha(kWhite, 100);
            std::cout << "Setting " << vol.first << " to white" << std::endl;
        }
        else {
            vol.second->SetInvisible();
            std::cout << "Setting " << vol.first << " to invisible" << std::endl;
        }
    }

    double origin[3] = {100, 100 , 75};
    TGeoBBox* clipBox = new TGeoBBox(145, 145, 165, origin);
    gGeoManager->SetClippingShape(clipBox);
    gGeoManager->GetTopVolume()->Raytrace();

    // J/K = zoom out and in respectively
    // U/I = move view up and down respectively
}