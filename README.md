# UML-Engineering-Camp

7/14/2016

This is a collection of code written for use on the Photon or in the case of the actual camp project, the RedBear Duo which is functionally equivalent to the Photon except that it also includes Bluetooth Low Energy capabilities. The aim of the camp project is to introduce highschool students to a simple Internet of Things (IoT) example. The Photon or RedBear boards are perfect for this in that they have intergrated WiFi and the ability to connect to various web services, such as Plotly. 

Files in this repository are currently in flux as I develop the examples and clean up the code. This is my first GitHub repository, so appologies in advance for any greivous errors I may have made.

The current files are:

Weather.ino: This is the main code for the board. In its current implementation the code connects to the BMP-180 pressure/temperature sensor that is attached to the board via I2C. Temperature and pressure readings are taken every five minutes and added to a Plotly graph.

SFE_BMP180.cpp and SFE_BMP180.h: These are modified code taken from Sparkfun's code for accessing the BMP180 sensor. 

Spark-plotly.cpp and spark-plotly.h: These are modified code taken from build.particle.io's user contributed library repository. The modifications fix known errors in the library provided on particle. A timeout was also added to prevent the board from becomming unresponsive if no response to the plot init function is received. Otherwise the board looses the connection to the particle cloud while waiting for plotly and has to be manually set to be reflashed.

Other files being added are weather related since the goal of the project is to make a home weather station that can communicate changes in the weather to the user from anywhere.
