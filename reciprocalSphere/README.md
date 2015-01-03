reciprocalSphere
================

openFrameworks application to load x-ray crystallography files in .sca (scalePack) format and display HKL data in form of grid. Apply Ewald sphere algorithm and rotate crystal to mimic view of actual data capture. 

<p align="center">
	<img src="https://raw.githubusercontent.com/evsc/XRCrystals/master/reciprocalSphere/screenshot.png"/>
</p>



OSC output
---
Output on localhost, port 8888

Currently sound triggers are output via OSC when dot coordinates intersect with the Ewald Sphere. 

	/soundOn iffff
	(1) coordinate as integer composite: (dot->mirrorno)*1000000 + (dot->h+50)*10000 + (dot->k+50)*100 + dot->l+50
	(2) intensity
	(3) latitude
	(4) longitude
	(5) phase

	/soundOff if
	(1) coordinate as integer composite: (dot->mirrorno)*1000000 + (dot->h+50)*10000 + (dot->k+50)*100 + dot->l+50
	(2) latitude ?


