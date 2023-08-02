//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

#include "LensParameterFileReader.hh"

#include <regex>


LensParameterFileReader::LensParameterFileReader( G4String t_path ) 
    : m_path( t_path )
{
    readParameters();
}

LensParameterFileReader::~LensParameterFileReader()
{
    for( lens : m_lenses )
        delete lens;
    delete m_lenses;
}


// readParameters credit: mostly written by ChatGPT 3.5 (:
void LensParameterFileReader::readParameters()
{
    std::ifstream infile(m_path);
    if (!infile)
    {
        std::cerr << "Error: Could not open the file " << m_path << std::endl;
        return;
    }

    lensList.clear(); // Clear the list of lenses before populating it.

    std::string line;
    while (std::getline(infile, line))
    {
        if (line.empty() || line[0] == '#')
        {
            // Skip empty lines and comments.
            continue;
        }

        std::regex tuple_regex("\\(\\s*([-+]?\\d*\\.?\\d+)\\s*,\\s*([-+]?\\d*\\.?\\d+)\\s*,\\s*(-?\\d+)\\s*,\\s*(-?\\d+)\\s*\\)");
        std::regex value_regex("[-+]?\\d*\\.?\\d+");

        std::vector<double> values;
        std::smatch matches;

        // Find all occurrences of values in the line using regex.
        while (std::regex_search(line, matches, value_regex))
        {
            double value = std::stod(matches[0].str());
            values.push_back(value);

            line = matches.suffix().str(); // Move to the next match in the line.
        }

        if (values.size() != 11)
        {
            std::cerr << "Error: Invalid number of values in the line: " << line << std::endl;
            continue;
        }

        Surface* left_surface { new Surface() };
        Surface* right_surface{ new Surface() };
        double d, n, x_l;

        // Extract the values for each tuple from the 'values' vector.
        left_surface->set_radius_x   = values[ 0 ];
        left_surface->set_radius_y   = values[ 1 ];
        left_surface->set_yLimit_min = values[ 2 ];
        left_surface->set_yLimit_max = values[ 3 ];

        d = values[ 4 ];
        n = values[ 5 ];

        right_surface->set_radius_x   = values[ 6 ];
        right_surface->set_radius_y   = values[ 7 ];
        right_surface->set_yLimit_min = values[ 8 ];
        right_surface->set_yLimit_max = values[ 9 ];

        x_l = values[ 10 ];

        // Create a Lens object with the extracted parameters and add it to the lensList.
        Lens* lens = new Lens(left_surface, d, n, right_surface, x_l);
        m_lenses->push_back(lens);
    }
}