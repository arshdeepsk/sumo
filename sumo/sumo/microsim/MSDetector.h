#ifndef MSDetector_H
#define MSDetector_H

//---------------------------------------------------------------------------//
//                        MSDetector.h  -  Simple detector that emulates 
//                        induction loops.
//                           -------------------
//  begin                : Thu, 14 Mar 2002
//  copyright            : (C) 2002 by Christian Roessel
//  organisation         : ZAIK http://www.zaik.uni-koeln.de/AFS
//  email                : roessel@zpr.uni-koeln.de
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//          
//   This program is free software; you can redistribute it and/or modify  
//   it under the terms of the GNU General Public License as published by  
//   the Free Software Foundation; either version 2 of the License, or     
//   (at your option) any later version.                                   
//
//---------------------------------------------------------------------------//

// $Log$
// Revision 1.2  2002/04/10 15:16:15  croessel
// Get the job done version.
//
// Revision 1.2  2002/03/27 17:56:05  croessel
// Updated version.
//
// Revision 1.1  2002/03/14 18:48:54  croessel
// Initial commit.
//


#include "MSNet.h"
#include "MSVehicle.h"
#include "MSLane.h"
#include <string>
#include <fstream>
#include <functional>

/**
 */
class MSDetector
{
public:

    /** We support two output-styles, gnuplot and "Comma Separated Variable" 
        (CSV). */
    enum OutputStyle { GNUPLOT, CSV };

    /** Constructor: Detector detects on lane at position pos. He collects
        during samplIntervall seconds data and writes them in style to file.
     */ 
    MSDetector( std::string    id,
                MSLane*        lane,
                float          position,
                MSNet::Time    sampleIntervall,
                OutputStyle    style,
                std::ofstream& file );
    
    /// Destructor.
    ~MSDetector();

    /// Call sample every timestep to update the detector.
    void sample( double currSimSeconds );

protected:
    // Add up the local density.
    double localDensity( const MSVehicle& veh, double currSimSeconds );

    // Write the data according to OutputStyle when the
    // sampleIntervall is over.
    void writeData();

    // Write in gnuplot-style to myFile.
    void writeGnuPlot( MSNet::Time endOfInterv,
                       double avgDensity,
                       double avgFlow,
                       double avgSpeed,
                       double Occup,
                       double avgLength );

    // Write in CSV-style to myFile.
    void writeCSV( MSNet::Time endOfInterv,
                   double avgDensity,
                   double avgFlow,
                   double avgSpeed,
                   double Occup,
                   double avgLength );

private:
    // Function-object in order to find the vehicle, that has just
    // passed the detector.
    struct VehPosition : public std::binary_function< const MSVehicle*,
                         float, bool > 
    {
        bool operator() ( const MSVehicle* cmp, float pos ) const {
            return cmp->pos() > pos;
        }
    };

    /// Object's Id.
    std::string myID;

    /// Lane where detector works on.
    MSLane* myLane;

    /// Detectors position on myLane.
    float myPos;

    /// Sample-intervall in seconds.
    MSNet::Time mySampleIntervall;

    /// Ouput-style.
    OutputStyle myStyle;

    /// File where output goes to.
    std::ofstream& myFile;

    /// Last vehicle that passed the detector.
    MSVehicle* myPassedVeh;
    
    /// Speed of the last vehicle, that has passed the detector.
    double myPassingSpeed;
    
    /// Time when last vehicle has passed the detector.
    double myPassingTime;

    /// Number of finished sampleIntervalls.
    unsigned myNSamples;

    /** Number of vehicles which have already passed the detector in
        this sample intervall. */    
    unsigned myNPassedVeh;

    /// Sum of local-densities sampled during the current sample-intervall.
    double myLocalDensitySum;
    
    /// Sum of speeds sampled during the current sample-intervall.
    double mySpeedSum;

    /// Sum of occupancy-times sampled during the current sample-intervall.
    double myOccupSum;

    /// Sum of veh-lengthes sampled during the current sample-intervall.
    double myVehLengthSum;

    // Number of already processed sampleIntervalls
    unsigned myNIntervalls;

    /// Default constructor.
    MSDetector();
    
    /// Copy constructor.
    MSDetector( const MSDetector& );
    
    /// Assignment operator.
    MSDetector& operator=( const MSDetector& );     
};


//----------- DO NOT DECLARE OR DEFINE ANYTHING AFTER THIS POINT ------------//

//#ifndef DISABLE_INLINE
//#include "MSDetector.icc"
//#endif

#endif

// Local Variables:
// mode:C++
// End:










