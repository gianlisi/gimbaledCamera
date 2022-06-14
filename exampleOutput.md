<h1> Example output </h1>

This is an example of the expected output from ./main < test1.dat

The output contains the name and location of the drone, the name and location
of the identified vessels, and a list of pictures.  For each picture, the
camera angle in three reference systems (counterclockwise from East, clockwise
from North, and clockwise from North referenced to the left side of the camera
frame) is provided, together with a list of vessels contained in the picture.
The last four lines contain the camera trigger angles and the name of the file
to which the angles have been written. 

      ===================== The drone and its position ========================
      Vessel name: Rhinheart       | lat, lon: 37.76013, -122.32648
      
      ===================== List of identified vessels ========================
      Vessel name: Neo             | lat, lon: 37.77308, -122.33451 | Bearing:    116.1 degrees | Margin :    3.6 degrees | Distance:  1603.4 m
      Vessel name: Morpheus        | lat, lon: 37.77728, -122.34192 | Bearing:    125.4 degrees | Margin :    2.4 degrees | Distance:  2340.4 m
      Vessel name: Trinity         | lat, lon: 37.75784, -122.31716 | Bearing:    -17.3 degrees | Margin :    6.7 degrees | Distance:   858.2 m
      Vessel name: Smith           | lat, lon: 37.76822, -122.34187 | Bearing:    146.4 degrees | Margin :    3.5 degrees | Distance:  1624.4 m
      Vessel name: Cypher          | lat, lon: 37.76002, -122.30260 | Bearing:     -0.3 degrees | Margin :    2.7 degrees | Distance:  2099.4 m
      Vessel name: SmithS          | lat, lon: 37.75913, -122.34187 | Bearing:   -175.3 degrees | Margin :    4.2 degrees | Distance:  1357.4 m
      
      =====================      List of pictures      ========================
      
      ### PICTURE ###
      Camera angle: ............................... 150.4 degrees (counterclockwise from East)
      Camera angle: ............................... 299.6 degrees (clockwise from North)
      Camera trigger angle: ....................... 259.6 degrees (clockwise from North, left side of camera frame)
      Number of vessels: .......................... 4
      Maximum angular distance between vessels: ... 68.6 degrees (not including margin)
      List of vessels:
      -- Vessel name: Neo             | lat, lon: 37.77308, -122.33451 | Bearing:    116.1 degrees | Margin :    3.6 degrees | Distance:  1603.4 m
      -- Vessel name: Morpheus        | lat, lon: 37.77728, -122.34192 | Bearing:    125.4 degrees | Margin :    2.4 degrees | Distance:  2340.4 m
      -- Vessel name: Smith           | lat, lon: 37.76822, -122.34187 | Bearing:    146.4 degrees | Margin :    3.5 degrees | Distance:  1624.4 m
      -- Vessel name: SmithS          | lat, lon: 37.75913, -122.34187 | Bearing:   -175.3 degrees | Margin :    4.2 degrees | Distance:  1357.4 m
      
      ### PICTURE ###
      Camera angle: ............................... -8.8 degrees (counterclockwise from East)
      Camera angle: ............................... 98.8 degrees (clockwise from North)
      Camera trigger angle: ....................... 58.8 degrees (clockwise from North, left side of camera frame)
      Number of vessels: .......................... 2
      Maximum angular distance between vessels: ... 16.9 degrees (not including margin)
      List of vessels:
      -- Vessel name: Trinity         | lat, lon: 37.75784, -122.31716 | Bearing:    -17.3 degrees | Margin :    6.7 degrees | Distance:   858.2 m
      -- Vessel name: Cypher          | lat, lon: 37.76002, -122.30260 | Bearing:     -0.3 degrees | Margin :    2.7 degrees | Distance:  2099.4 m
      
      *************************************************************************
      ** The camera trigger angles are: 260, 59, 
      ** Trigger angles written to output.txt
      *************************************************************************

