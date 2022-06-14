/* 
 * Copyright (c) 2021 Gianluca Meneghello (gianluca.meneghello@gmail.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <iomanip>
#include <cmath>
#include <complex>
#include <list>
#include <vector>

#ifndef GIMBALEDCAMERA_H
#define GIMBALEDCAMERA_H

/** @file */

/** The namespace containing Vessel, the subclassed RelativeVessel, Picture and
 * a function makePictures to partition vessels into pictures.  
 * */
namespace gimbaledCamera {
  /** A class describing a vessel.
   *  Include its name, latitude and longitude.
   */
  class Vessel {

    public:

      /// Class constructor.
      Vessel(
          const double      /** vessel latitude, in degrees */, 
          const double      /** vessel longitude, in degrees */, 
          const std::string /** vessel name */
          ); 

      /// Returns the vessel latitude in radians.
      inline double getLon() const { 
        return lon; 
      };

      /// Returns the vessel latitude in radians.
      inline double getLat() const { 
        return lat; 
      };                 

      /// Returns the vessel longitude in degrees.
      inline double getLonDeg() const { 
        return lon*180/M_PI; 
      };     

      /// Returns the vessel latitude in degrees.
      inline double getLatDeg() const { 
        return lat*180/M_PI; 
      };     

      /// Returns the vessel name.
      inline std::string getName() const { 
        return name; 
      };     

      /// Overloads the << operator
      friend std::ostream &operator<<(std::ostream &output, Vessel &v) { 
        output << std::fixed;
        output << "Vessel name: " << std::setw(15) 
               << std::left       << v.getName() << std::right
               << std::setprecision(5) << std::setw(10)   
               << " | lat, lon: " << v.getLatDeg()
               << ", "            << v.getLonDeg()
               << std::setprecision(6) << std::endl;
        return output;
      }

    protected: 

      double lon;       ///< Vessel longitude, in radians
      double lat;	///< Vessel latitude, in radians
      std::string name; ///< Vessel name

  };

  /** A class describing a vessel positioned relative to another vessel. 
   *  It includes bearing and distance. 
   */ 
  class RelativeVessel: public Vessel {

    public:

      /// Class constructor.
      RelativeVessel(
          const double      /** vessel latitude, in degrees */, 
          const double      /** vessel longitude, in degrees */, 
          const std::string /** vessel name */, 
          const Vessel      /** drone instance */,
          const double=100  /** the radius of the region around lat, lon we want to capture, in meters */
          ); 

      /// Returns distance from reference vessel in meters.
      inline double getDistance() const { 
        return dist; 
      };                                 

      /// Returns bearing from reference vessels, in radians.
      inline double getBearing() const { 
        return bearing; 
      };

      
      /// Returns bearing from reference vessel, in degrees.
      inline double getBearingDeg() const { 
        return bearing*180/M_PI; 
      };                    
      
      /// Returns reference vessel margin, in degrees.
      inline double getMargin() const { 
        return bearingMargin;
      };                    

      /// Returns reference vessel margin, in degrees.
      inline double getMarginDeg() const { 
        return bearingMargin*180/M_PI; 
      };                    
      
      double computeBearing(const Vessel) const;  ///< Compute vessel bearing relative to drone
      double computeDistance(const Vessel) const ; ///< Compute vessel distance relative to drone using Haversine distance 

      /// Overloads the << operator.
      friend std::ostream &operator<<(std::ostream &output, RelativeVessel &v) {
        output << std::fixed;
        output << "Vessel name: " << std::setw(15) 
               << std::left << v.getName() << std::right
               << std::setprecision(5) << std::setw(10)   
               << " | lat, lon: "   <<  v.getLatDeg()
               << ", "              << v.getLonDeg()
               << std::setprecision(1) 
               << " | Bearing:  "  << std::setw(7) << v.getBearingDeg() << " degrees"
               << " | Margin :  "  << std::setw(5) << v.getMarginDeg() << " degrees"
               << " | Distance: "  << std::setw(7) << v.getDistance()   << " m"
               << std::setprecision(6) << std::endl;
        return output;
      }

    private:

      double bearing;        ///< Bearing, in radians [-pi, +pi], zero is east
      double dist;           ///< distance, in meters
      double bearingMargin;  ///< asin(100./distance): used to make sure to capture an area 100 m around the target vessel

      friend bool sortByBearing(RelativeVessel a, RelativeVessel b);

  };

  /** Given two vessels a and b, returns True if the bearing of a is less
   *  than the bearing of b.
   *  Used for sorting a list of vessels by bearing.
   */
  inline bool sortByBearing(RelativeVessel a, RelativeVessel b) { 
    return (a.bearing < b.bearing); 
  }

  /** A class describing a picture. 
   *  Contains a list of vessels.
   *  Can compute the angle to which the camera should be pointed to.
   */
  class Picture {

    public:

      /// Class constructor.
      Picture(
          double FOVin,
          std::list<RelativeVessel>::iterator begin,
          std::list<RelativeVessel>::iterator end
          ) ;

      /// Compute the camera angle (in degrees) as the average point between
      /// the leftmost and rightmost vessel in the picture.
      double getCameraAngleDeg(
          char referenceSystem='E' /** Possible values: \n
                                     'E' (counterclockwise, East=0) [default]\n
                                     'N' (clockwise, North=0) \n
                                     'C' (clockwise, North=0, from left side of the camera frame)\n
                                     */
          ) const;

      /// Compute the maximum angular distance (in degrees) between the
      /// leftmost and rightmost vessel in the picture.
      double getMaxAngularDistanceDeg() const;

      /// Adds a vessel to the picture.
      inline void addVessel(RelativeVessel vessel) { 
        vessels.push_back(vessel); 
      };

      /// Counts the number of vessels in the picture 
      inline int countVessels() const {
        return vessels.size();
      };

      /// Transfers vessels from newVessel into this picture. Sorts them by bearing if sort=true
      inline void addVessels( 
          std::list<gimbaledCamera::RelativeVessel> newVessels, 
          bool sort=false
          ) {
        vessels.splice(vessels.begin(), newVessels);
        if (sort)
          vessels.sort(sortByBearing); 
      }

      /// Returns the list of vessels.
      inline std::list<RelativeVessel> getVessels() const { 
        return vessels; 
      };  

      /// Overloads the << operator.
      friend std::ostream &operator<<(std::ostream &output, Picture &p) {
        output << std::fixed << std::setw(7) << std::setprecision(1)
          << "### PICTURE ###" << std::endl
          << "Camera angle: ............................... " << p.getCameraAngleDeg()        << " degrees (counterclockwise from East)" << std::endl
          << "Camera angle: ............................... " << p.getCameraAngleDeg('N')     << " degrees (clockwise from North)" << std::endl
          << "Camera trigger angle: ....................... " << p.getCameraAngleDeg('C')     << " degrees (clockwise from North, left side of camera frame)" << std::endl
          << "Number of vessels: .......................... " << p.countVessels()             << std::endl
          << "Maximum angular distance between vessels: ... " << p.getMaxAngularDistanceDeg() << " degrees (not including margin)"   << std::endl
          << "List of vessels:" << std::endl
          << std::setprecision(6);

        for (std::list<gimbaledCamera::RelativeVessel>::iterator ptr=p.vessels.begin(); ptr!=p.vessels.end(); ptr++)
          output << "-- " << *ptr;

        return output;
      }

    private:

      std::list<RelativeVessel> vessels;        ///< The list of vessels in the picture 
      double FOV;                               ///< The picture field of view

  };

  /** Groups vessels into pictures.
   * 
   *  Given a list or RelatedVessels, it partitions them into pictures based on
   *  the FOV.  Returns a list containing the minimum number of pictures given
   *  the vessel position and the drone's FOV.
   * */
  std::list<Picture> makePictures(
      double FOV                        /** the camera field of view, in degrees */,
      std::list<RelativeVessel> vessels /** the vessels to be partitioned into pictures */
      );

}

#endif


