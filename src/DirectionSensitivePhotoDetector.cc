//*/////////////////////////////////////////////////////////////////////////*//
//*//                    G4-DSPS-Detector-Simulation                      //*//
//*/////////////////////////////////////////////////////////////////////////*//
//*//                                                                     //*//
//*// Author:                                                             //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                       //*//
//*//                                                                     //*//
//*/////////////////////////////////////////////////////////////////////////*//
//*//                                                                     //*//
//*/////////////////////////////////////////////////////////////////////////*//

#include "DirectionSensitivePhotoDetector.hh"

DirectionSensitivePhotoDetector::DirectionSensitivePhotoDetector( const G4String& t_name, const G4String& t_index ) {
    m_name = t_name + "_" + t_index;

    m_lensSystem  = new LensSystem ( m_name + "_lensSystem" , true );
    m_photoSensor = new PhotoSensor( m_name + "_photoSensor" );
}

DirectionSensitivePhotoDetector::~DirectionSensitivePhotoDetector() {
    if( m_photoSensor ) delete m_photoSensor;
    if( m_lensSystem  ) delete m_lensSystem;
}

void DirectionSensitivePhotoDetector::place( G4RotationMatrix* t_rotationMatrix     , 
                                             G4ThreeVector     t_translationVector  , 
                                             G4LogicalVolume * t_parentLogicalVolume, 
                                             G4bool            t_isMany             ,
                                             const char      * t_relativePosition    ) {
    G4String relativePosition = G4String( t_relativePosition );
    to_lower( relativePosition );
    if( relativePosition == "front" || relativePosition == "f" ) {
        m_position_lensSystem  = t_translationVector;
        m_position_photoSensor = t_translationVector + *t_rotationMatrix * G4ThreeVector( 0, 0, get_depth() );
    } else if( relativePosition == "back" || relativePosition == "b" ) {
        m_position_lensSystem  = t_translationVector - *t_rotationMatrix * G4ThreeVector( 0, 0, get_depth() );
        m_position_photoSensor = t_translationVector;
    } else if( relativePosition == "center" || relativePosition == "c" ) {
        m_position_lensSystem  = t_translationVector - *t_rotationMatrix * G4ThreeVector( 0, 0, get_depth() / 2 );
        m_position_photoSensor = t_translationVector + *t_rotationMatrix * G4ThreeVector( 0, 0, get_depth() / 2 );
    } else
        G4Exception( "DirectionSensitivePhotoDetector::place"          , 
                     "InvalidArgument"                                 ,
                     FatalErrorInArgument                              ,
                     "Invalid relative position string. Valid positions are: "
                     "\"front\", \"f\", \"back\", \"b\", \"center\", \"c\"." );

    m_rotationMatrix      = t_rotationMatrix     ;
    m_parentLogicalVolume = t_parentLogicalVolume;
    m_isMany              = t_isMany             ;

    m_lensSystem ->place( t_rotationMatrix, m_position_lensSystem , t_parentLogicalVolume, t_isMany );
    m_photoSensor->place( t_rotationMatrix, m_position_photoSensor, t_parentLogicalVolume, t_isMany );
}

G4ThreeVector DirectionSensitivePhotoDetector::get_size() {
    return G4ThreeVector( get_width(), get_height(), get_depth() );
}

G4double DirectionSensitivePhotoDetector::get_width() {
    return ConstructionMessenger::get_instance()->get_photoSensor_surface_size_width();
}

G4double DirectionSensitivePhotoDetector::get_height() {
    return ConstructionMessenger::get_instance()->get_photoSensor_surface_size_height();
}

G4double DirectionSensitivePhotoDetector::get_depth() {
    return ConstructionMessenger::get_instance()->get_photoSensor_surface_size_depth()
         + ConstructionMessenger::get_instance()->get_photoSensor_body_size_depth();
}

LensSystem * DirectionSensitivePhotoDetector::get_lensSystem() {
    return m_lensSystem;
}

PhotoSensor* DirectionSensitivePhotoDetector::get_photoSensor() {
    return m_photoSensor;
}

G4String DirectionSensitivePhotoDetector::get_name() {
    return m_name;
}

G4RotationMatrix* DirectionSensitivePhotoDetector::get_rotationMatrix() {
    return m_rotationMatrix;
}

G4ThreeVector DirectionSensitivePhotoDetector::get_position_lensSystem() {
    return m_position_lensSystem;
}

G4ThreeVector DirectionSensitivePhotoDetector::get_position_photoSensor() {
    return m_position_photoSensor;
}

G4ThreeVector DirectionSensitivePhotoDetector::get_position( const char* t_relativePosition ) {
    G4String relativePosition = G4String( t_relativePosition );
    to_lower( relativePosition );
    if( relativePosition == "front" || relativePosition == "f" )
        return get_position_front();
    else if( relativePosition == "back" || relativePosition == "b" )
        return get_position_back();
    else if( relativePosition == "center" || relativePosition == "c" )
        return get_position_center();
    else
        G4Exception( "DirectionSensitivePhotoDetector::get_position" , 
                     "InvalidArgument"                               ,
                     FatalErrorInArgument                            ,
                     "Invalid relative position string. Valid positions are: "
                     "\"front\", \"f\", \"back\", \"b\", \"center\", \"c\"." );
    return G4ThreeVector();
}

G4ThreeVector DirectionSensitivePhotoDetector::get_position_front() {
    return m_lensSystem->get_position_front();
}

G4ThreeVector DirectionSensitivePhotoDetector::get_position_back() {
    return m_photoSensor->get_position_back();
}

G4ThreeVector DirectionSensitivePhotoDetector::get_position_center() {
    return ( get_position_front() + get_position_back() ) / 2;
}

G4LogicalVolume * DirectionSensitivePhotoDetector::get_parentLogicalVolume() {
    return m_parentLogicalVolume;
}

G4bool DirectionSensitivePhotoDetector::get_isMany() {
    return m_isMany;
}

void DirectionSensitivePhotoDetector::set_name( const G4String& t_name ) {
    m_name = t_name;
    m_lensSystem ->set_name( m_name + "_lensSystem"  );
    m_photoSensor->set_name( m_name + "_photoSensor" );
}

G4ThreeVector DirectionSensitivePhotoDetector::get_position_front( G4RotationMatrix* t_rotationMatrix   , 
                                                                   G4ThreeVector     t_translationVector, 
                                                                   const char      * t_relativePosition  ) {
    G4String relativePosition = G4String( t_relativePosition );
    to_lower( relativePosition );

    if( relativePosition == "front" || relativePosition == "f" ) {
        return t_translationVector;
    } else if( relativePosition == "back" || relativePosition == "b" ) {
        return LensSystem::get_position_front( t_rotationMatrix, PhotoSensor::get_position_front( t_rotationMatrix, t_translationVector, t_relativePosition ), t_relativePosition );
    } else if( relativePosition == "center" || relativePosition == "c" ) {
        G4Exception( "DirectionSensitivePhotoDetector::get_position_front", "InvalidArgument", FatalErrorInArgument, "Not implemented" );
        return ( PhotoSensor::get_position_back( t_rotationMatrix, t_translationVector, t_relativePosition ) 
               + LensSystem::get_position_front( t_rotationMatrix, t_translationVector, t_relativePosition ) ) / 2;
    } else
        G4Exception( "DirectionSensitivePhotoDetector::get_position_front" , 
                     "InvalidArgument"                                     ,
                     FatalErrorInArgument                                  ,
                     "Invalid relative position string. Valid positions are: "
                     "\"front\", \"f\", \"back\", \"b\", \"center\", \"c\"." );

    return G4ThreeVector();
}

G4ThreeVector DirectionSensitivePhotoDetector::get_position_back( G4RotationMatrix* t_rotationMatrix   , 
                                                                  G4ThreeVector     t_translationVector, 
                                                                  const char      * t_relativePosition  ) {
    G4String relativePosition = G4String( t_relativePosition );
    to_lower( relativePosition );
    if( relativePosition == "front" || relativePosition == "f" ) {
        G4Exception( "DirectionSensitivePhotoDetector::get_position_back", "InvalidArgument", FatalErrorInArgument, "Not implemented" );
        return t_translationVector + *t_rotationMatrix * G4ThreeVector( 0, 0, get_depth() );
    } else if( relativePosition == "back" || relativePosition == "b" ) {
        return t_translationVector;
    } else if( relativePosition == "center" || relativePosition == "c" ) {
        G4Exception( "DirectionSensitivePhotoDetector::get_position_back", "InvalidArgument", FatalErrorInArgument, "Not implemented" );
        return t_translationVector + *t_rotationMatrix * G4ThreeVector( 0, 0, get_depth() / 2 );
    } else
        G4Exception( "DirectionSensitivePhotoDetector::get_position_back" , 
                     "InvalidArgument"                                    ,
                     FatalErrorInArgument                                 ,
                     "Invalid relative position string. Valid positions are: "
                     "\"front\", \"f\", \"back\", \"b\", \"center\", \"c\"." );
    return G4ThreeVector();
}

// G4ThreeVector DirectionSensitivePhotoDetector::get_position_center( G4RotationMatrix* t_rotationMatrix   , 
//                                                                     G4ThreeVector     t_translationVector, 
//                                                                     const char      * t_relativePosition  ) {
//     G4String relativePosition = G4String( t_relativePosition );
//     to_lower( relativePosition );
//     if( relativePosition == "front" || relativePosition == "f" )
//         return t_translationVector - *t_rotationMatrix * G4ThreeVector( 0, 0, get_depth() / 2 );
//     else if( relativePosition == "back" || relativePosition == "b" )
//         return t_translationVector + *t_rotationMatrix * G4ThreeVector( 0, 0, get_depth() / 2 );
//     else if( relativePosition == "center" || relativePosition == "c" )
//         return t_translationVector;
//     else
//         G4Exception( "DirectionSensitivePhotoDetector::get_position_center" , 
//                      "InvalidArgument"                                      ,
//                      FatalErrorInArgument                                   ,
//                      "Invalid relative position string. Valid positions are: "
//                      "\"front\", \"f\", \"back\", \"b\", \"center\", \"c\"." );
//     return G4ThreeVector();
// }


// G4ThreeVector DirectionSensitivePhotoDetector::get_position( const char      * t_relativePosition_want ,
//                                                              G4RotationMatrix* t_rotationMatrix        , 
//                                                              G4ThreeVector     t_translationVector     , 
//                                                              const char      * t_relativePosition_given ) {
//     G4String relativePosition_want = G4String( t_relativePosition_want );
//     to_lower( relativePosition_want );
//     if( relativePosition_want == "front" || relativePosition_want == "f" )
//         return get_position_front( t_rotationMatrix, t_translationVector, t_relativePosition_given );
//     else if( relativePosition_want == "back" || relativePosition_want == "b" )
//         return get_position_back( t_rotationMatrix, t_translationVector, t_relativePosition_given );
//     else if( relativePosition_want == "center" || relativePosition_want == "c" )
//         return get_position_center( t_rotationMatrix, t_translationVector, t_relativePosition_given );
//     else
//         G4Exception( "DirectionSensitivePhotoDetector::get_position" , 
//                      "InvalidArgument"                               ,
//                      FatalErrorInArgument                            ,
//                      "Invalid relative position string. Valid positions are: "
//                      "\"front\", \"f\", \"back\", \"b\", \"center\", \"c\"." );
//     return G4ThreeVector();
// }