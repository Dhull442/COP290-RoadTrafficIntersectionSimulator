# Traffic light Simulator 

This is a configurable program to simulate traffic behavior near a traffic signal. It is written from scratch in OpenGL and supports both 2D and 3D visualizations. The simulation is driven by a `.ini` configuration file which can specify the driving behavior of individual cars and the traffic signal timing.

## Compiling 

Ensure that you have the following dependencies installed

```
-glfw
-Xinerama
-X11
-GL
-XCursor
-XRandr
-Xi
```

To compile the 2D version

```
make all
```

To compile the 3D version

```
make dim=D3
```

## Running

To run you must specify a configuration `.ini` file as a command-line argument. You can use one of the pre-defined files or create your own. For example, 

```
./main jam.ini
```

## Examples

1. This is a vanilla configuration showing the lane-changing and decelerating/accelerating behavior. 

<table>
  <tr>
    <td><img src="https://github.com/Dhull442/COP290-RoadTrafficIntersectionSimulator/raw/AnagBackend/gifs/Normal_2D.gif"></td>
    <td><img src="https://github.com/Dhull442/COP290-RoadTrafficIntersectionSimulator/raw/AnagBackend/gifs/Normal_3D.gif"></td>
  </tr>
 </table>
 
 2. This shows how drivers navigate when there is congestion. Most drivers lane change aggressively to get out of the jam as soon as possible.
 
 
 



