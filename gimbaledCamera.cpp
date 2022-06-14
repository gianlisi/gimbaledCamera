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

#include <iostream>
#include <cmath>
#include "gimbaledCamera.h"

namespace gimbaledCamera {

  // Class constructor for Vessel
  Vessel::Vessel(
      const double latin, 
      const double lonin, 
      const std::string namein) {
    lat = latin*M_PI/180.0;
    lon = lonin*M_PI/180.0;
    name = namein;
  }

  // Class constructor for RelativeVessel 
  RelativeVessel::RelativeVessel(
      const double latin, 
      const double lonin, 
      const std::string namein, 
      const Vessel drone, 
      const double margin
      ) : Vessel(latin, lonin, namein) 
  {
    bearing = computeBearing (drone);
    dist    = computeDistance(drone);
    bearingMargin = asin(margin/dist);
  }

  // Compute vessel bearing relative to drone
  double RelativeVessel::computeBearing(const Vessel drone) const {
    const double dLon = (lon - drone.getLon()); 

    const double X = cos(lat) * sin(dLon);
    const double Y = cos(drone.getLat())*sin(lat) 
                   - sin(drone.getLat())*cos(lat)*cos(dLon);
    return atan2(Y, X);
  }

  // Compute vessel distance relative to drone using Haversine distance 
  double RelativeVessel::computeDistance(const Vessel drone) const {
    const double dLat = (lat - drone.getLat());
    const double dLon = (lon - drone.getLon()); 
    
    const double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(drone.getLat()) * cos(lat);
    const double rad = 6371000;
    const double c = 2 * asin(sqrt(a));
    return rad*c;
  }

  // Class constructor for Picture
  Picture::Picture(
      double FOVin,
      std::list<RelativeVessel>::iterator begin,
      std::list<RelativeVessel>::iterator end
      ) {
    FOV = FOVin;
    vessels.assign(begin,end);
  }

  // Compute the camera angle (in degrees) as the average point between
  // the leftmost and rightmost vessel in the picture.
  double Picture::getCameraAngleDeg(char referenceSystem) const { 
    const double bearing1 = vessels.front().getBearing(),
                 bearing2 = vessels.back().getBearing();

    // we use complex numbers to take into account how angles wrap around
    const std::complex<double> a = std::polar(1., bearing1),
	                       b = std::polar(1., bearing2);
    const std::complex<double> mean = (a+b)/2.;

    if (referenceSystem == 'N')
      return fmod(450-std::arg(mean)*180/M_PI, 360.);
    else if (referenceSystem == 'C')
      return fmod(450-(std::arg(mean)+FOV/2.)*180/M_PI, 360.);
    else // defaults to E
      return std::arg(mean)*180/M_PI; 
  }

  // Compute the maximum angular distance (in degrees) between the
  // leftmost and rightmost vessel in the picture.
  double Picture::getMaxAngularDistanceDeg() const {
    const double bearing1 = vessels.front().getBearing(),
	         bearing2 = vessels.back().getBearing();

    // we use complex numbers to take into account how angles wrap around
    const std::complex<double> a = std::polar(1., bearing1),
               	               b = std::polar(1., bearing2);
    const std::complex<double> maxAngularDistance = b/a;

    return std::arg(maxAngularDistance)*180/M_PI; 
  }

  // Produce a list of pictures by binning the vessels if FOV-wide bins 
  std::list<Picture> makePictures(double FOV, std::list<RelativeVessel> vessels) {

    FOV *= M_PI/180;                             // convert field of view from degrees to radians
    vessels.sort(gimbaledCamera::sortByBearing); // sort the vessels by bearing

    std::list<gimbaledCamera::Picture> pictures;                                     // create list of pictures
    std::list<gimbaledCamera::RelativeVessel>::iterator refVessel = vessels.begin(); // the first reference vessel

    for (auto ptr = vessels.begin(); ptr!=vessels.end(); ++ptr) {

      // compute the delta in bearing, including the marings
      double dBearing = ( ptr->getBearing()       + ptr->getMargin()       ) 
                      - ( refVessel->getBearing() - refVessel->getMargin() ); 

      if (dBearing >= FOV) {                                  // if bearings' difference is greater than FOV
        gimbaledCamera::Picture picture(FOV,refVessel,ptr--); // ... create a new picture and assign vessels between refVessels and ptr--
        ++ptr;                                                // ... restore ptr
        pictures.push_back(picture);                          // ... add the picture to the picture list
        refVessel = ptr;                                      // ... and update the reference Vessel
      }
    }
    gimbaledCamera::Picture picture(FOV,refVessel,vessels.end());; // then add the last picture to the list
    pictures.push_back(picture);		            

    // check if the first and last picture can be merged
    RelativeVessel vessel1 = pictures.front().getVessels().back(); // last bearing of first picture
    RelativeVessel vessel2 = pictures.back().getVessels().front(); // last bearing of last picture
    double dBearing =  vessel1.getBearing() + vessel1.getMargin() + (2*M_PI-vessel2.getBearing()) + vessel2.getMargin();
    if (dBearing < FOV) {
      pictures.front().addVessels(pictures.back().getVessels());
      pictures.pop_back();
    }
    return pictures;
  }

}
