# Net-Robot
Net Robot is an IOT robot that can be controlled over a local network connection. 
## Progress (WORK IN PROGRESS)
This code works, but is a work in progress. As of now it lacks certain features, such as preventing an unauthorized local client from sending servo control commands. 
## Modularity
One of the main goals of this project is to make a robot where costum modules can be easily added to fit one's needs. For example, I am currently developing a module for network diagnostics where an engineer can perform network tests with this robot via a web GUI.

## Current hardware used
* PCA9685 servo driver
* ESP8266 (NodeMCU)
* 2 continuous servos
* 9v power source for NodeMCU
* 6v power source for PCA9685
