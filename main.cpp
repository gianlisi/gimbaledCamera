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
#include <fstream>
#include <list>
#include <vector>
#include "gimbaledCamera.h"

int main(int argc, char **argv) {

  // the vessel positions and names (can be read from file)
  struct Data { 
    std::string name; 
    double lat; 
    double lon; 
  };

  std::vector<Data> data;

  // load the vessels' name and position from command line  
  while ((!(std::cin >> std::ws).eof())) {
    Data tmp;
    std::cin >> tmp.name >> tmp.lat >> tmp.lon;
    data.push_back(tmp);
  }

  // initialize the drone and print its description to screenn
  gimbaledCamera::Vessel drone(data[0].lat, data[0].lon, data[0].name);
  data.erase(data.begin()); // remove the drone from the list

  std::cout << std::endl <<
    "===================== The drone and its position ========================" 
    << std::endl;
  std::cout << drone; 

  // initialize a list of vessels and print their description to screen 
  std::list<gimbaledCamera::RelativeVessel> vessels;
  for (auto const& tmp : data) {
    gimbaledCamera::RelativeVessel vessel(tmp.lat, tmp.lon, tmp.name, drone);
    vessels.push_back(vessel);
  }

  std::cout  << std::endl <<
    "===================== List of identified vessels ========================"
    << std::endl;
  for (auto & vessel : vessels)
    std::cout << vessel;

  // generate a list of pictures and print their description to screen
  std::list<gimbaledCamera::Picture> pictures;
  pictures = makePictures(80., vessels);

  std::cout  << std::endl <<
    "=====================      List of pictures      ========================"
    << std::endl;
  for (auto & picture : pictures) 
    std::cout << std::endl << picture;

  // print the final result to screen
  std::cout << std::endl << 
    "*************************************************************************\n";
  std::cout << "** The camera trigger angles are: ";
  std::cout << std::setprecision(0) << std::fixed;
  for (auto & picture : pictures) 
    std::cout <<  picture.getCameraAngleDeg('C') << ", ";
  std::cout << std::endl;

  // save the final result to file 
  std::ofstream myfile;
  myfile.open ("output.txt");
  for (auto & picture : pictures) 
    myfile << picture.getCameraAngleDeg('C') << std::endl;
  myfile.close();
  std::cout << "** Trigger angles written to output.txt\n";
  std::cout << 
    "*************************************************************************\n";

  return 0;


}
