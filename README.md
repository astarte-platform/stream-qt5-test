Astarte Stream Qt5 Test
=======================

Astarte Qt5 SDK based data stream test.

Requirements
============

* Astarte Qt5 SDK and Qt5 devel tools

Getting started
===============

Create a configuration directory using the given template (astarte-device-fbZ1F-TUpH1O7ryuLsGTzw-conf in this example) and customize it.

Build and run the application using following commands:
```
$ qmake .
$ make
./stream-qt5-test --device gbZ1F-TUpH1O7ryuLsGTzw --function sin --interval 100 --scale 0.0001
```
