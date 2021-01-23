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
- [ ] video settings file is currently ignored
- [ ] MQTT messages are currently just printed. config is ignored for topics
- [ ] No Motor controll is implemented yet, but drivers are initated.
