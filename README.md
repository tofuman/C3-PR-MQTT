# C3-PR-MQTT
C3 PresenceRobot rewritten to utilize MQTT

This is a refactor/rewrite of [C3-PR](https://github.com/c3d2/C3-PR). 

A number of points contibute to this work:
* C++ based class structure for easier maintanablity.
* Use MQTT based control and status protocl
  * Robot can report status back to server, rather than polling
  * Server can easier controll multiple robots
* Config parser with split config files
  * provisioning different robots does not require multiple FW builds
  * general config (i.e. settings which are the same for all robots) split from the per robot settings
  * video config to enable persistent storage of tweaked video settings
  

## TODOs:
- [ ] settings file is currently ignored, both for video as well as motor calibration
- [ ] No Status reports are send back to server
- [ ] No "All" cmds are implemented. E.g. reboot and halt movement.
- [ ] Motor controll is implemented but no tweaks on strength and duration are done
 - [ ] Make settings persisten by writing them to flash
