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

#include <gtest/gtest.h> 
#include "gimbaledCamera.h"

  struct Data { 
    std::string name; 
    double lat; 
    double lon; 
    double dist; 
    double bearing; 
  };

  const Data testDataDrone = {"drone", 37.760132, -122.3264815, NAN, NAN};

  const std::vector<Data> testData = {
    // name      , lat     , lon       , dist, bearing (counterclockwise from EAST)
    { "Neo"      , 37.77308, -122.33451, 1602, 116.1  }, 
    { "Morpheus" , 37.77728, -122.34192, 2338, 125.4  }, 
    { "Trinity"  , 37.75784, -122.31716,  857, -17.2  }, 
    { "Smith"    , 37.76822, -122.34187, 1623, 146.3  }, 
    { "Cypher"   , 37.76002, -122.30260, 2098, -0.3   }, 
    { "SmithS"   , 37.75913, -122.34187, 1357, -175.3 } 
  };

/* Test basic functionalities of the Vessel class 
 * (conversion from degrees to radians and back) 
 */
TEST(Vessel, LatLon) {

  gimbaledCamera::Vessel drone(testDataDrone.lat, testDataDrone.lon, testDataDrone.name);

  EXPECT_NEAR(testDataDrone.lat          , drone.getLatDeg() , 1e-13);
  EXPECT_NEAR(testDataDrone.lon          , drone.getLonDeg() , 1e-13);
  EXPECT_NEAR(testDataDrone.lat*M_PI/180 , drone.getLat()    , 1e-13);
  EXPECT_NEAR(testDataDrone.lon*M_PI/180 , drone.getLon()    , 1e-13);
  ASSERT_STREQ(testDataDrone.name.c_str(), drone.getName().c_str());
}

/* Test computation of bearing and distance from the RelativeVessel class.
 * Reference values (the last two columns of testData) are computed independently
 * using python.  We allow an error of 0.1 degrees and 10m because a different
 * algorithm is used in python to compute bearing and distance,
 */
TEST(RelativeVessel, BearingAndDistance) {

  gimbaledCamera::Vessel drone(testDataDrone.lat, testDataDrone.lon, testDataDrone.name);
  
  for (auto & testVessel : testData){
    gimbaledCamera::RelativeVessel vessel(testVessel.lat, testVessel.lon, testVessel.name, drone);
    EXPECT_NEAR(testVessel.bearing , vessel.getBearingDeg() , .1);
    EXPECT_NEAR(testVessel.dist    , vessel.getDistance()   , 10);
  }
}

/* Test the generation of the Picture class */
TEST(Pictures, MakePictures) {
  gimbaledCamera::Vessel drone(testDataDrone.lat, testDataDrone.lon, testDataDrone.name);

  std::list<gimbaledCamera::RelativeVessel> vessels;
  for (auto & testVessel : testData) {
    gimbaledCamera::RelativeVessel vessel(testVessel.lat, testVessel.lon, testVessel.name, drone);
    vessels.push_back(vessel);
  }

  std::list<gimbaledCamera::Picture> pictures = makePictures(80., vessels);
  
  EXPECT_EQ(pictures.size()                 , 2)                    << "The number of pictures produced is not as expected";
  EXPECT_EQ(pictures.front().countVessels() , 4)                    << "The first picture does not have the expected number of vessels";
  EXPECT_EQ(pictures.back().countVessels()  , 2)                    << "The last picture does not have the expected number of vessels";
  EXPECT_NEAR(pictures.front().getCameraAngleDeg()    , 150.4 , .1) << "The Camera angle for the first picture in the 'E' reference system is incorrect";
  EXPECT_NEAR(pictures.front().getCameraAngleDeg('N') , 299.6 , .1) << "The Camera angle for the first picture in the 'N' reference system is incorrect";
  EXPECT_NEAR(pictures.front().getCameraAngleDeg('C') , 259.6 , .1) << "The Camera angle for the first picture in the 'C' reference system is incorrect";
  EXPECT_NEAR(pictures.back().getCameraAngleDeg()     , -8.8  , .1) << "The camera angle for the last picture in the 'E' reference system is incorrect";
  EXPECT_NEAR(pictures.back().getCameraAngleDeg('N')  , 98.8  , .1) << "The camera angle for the last picture in the 'N' reference system is incorrect";
  EXPECT_NEAR(pictures.back().getCameraAngleDeg('C')  , 58.8  , .1) << "The camera angle for the last picture in the 'C' reference system is incorrect";
}

/*********************
 * THE MAIN FUNCTION *
 *********************/
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv); 
  return RUN_ALL_TESTS();
}
