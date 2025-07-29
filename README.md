# FABRIK

The code is set up to load an 8 joint structure and to load and trace a spline.
To change input mode, comment and uncomment the appropriate command in the start.tcl

If swapping to or from spline mode, be sure to comment or uncomment the load command as well

## HOW TO RUN:
1) Open the project folder in an IDE
2) Select anim.cpp inside the anim subfolder
3) Run the program

## COMMANDS:

To run/pause simulator : Click on window and press "s"

To manually set target : simulator fabrikSim target <x> <y> <z>

To read in a spline : simulator fabrikSim read <spline .txt file>

To change mode : simulator fabrikSim mode <mode name>
	(modes : click, drag, spline, command)


To change the number of joints:
	1) Add/remove desired points from start.tcl
		(command: system structure point <index> <x> <y> <z>
	2) Edit numPoints in Structure.h line 28 to be the number of points in the start.tcl in the Build folder
