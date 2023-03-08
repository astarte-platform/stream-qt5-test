Astarte Stream Qt5 Test
=======================

Astarte Qt5 SDK based data stream test.

Requirements
============

* Astarte Qt5 SDK and Qt5 devel tools

Getting started
===============

## Choosing a Device ID

A Base64 url-encoded uuid should be used, you can use [astartectl](https://github.com/astarte-platform/astartectl#installation) to generate one:

```
astartectl utils device-id generate-random
```

## Configuring the application

Create a configuration directory using the given template ([astarte-device-DEVICE_ID_HERE-conf](https://github.com/astarte-platform/stream-qt5-test/tree/release-1.0/astarte-device-DEVICE_ID_HERE-conf)), substituting the previously generated device ID to `DEVICE_ID_HERE`.
Make sure to have your realm's private key at hand.
Then customize the following fields:

- `pairingJwt`: If you don't already have an Astarte pairing JWT, you can generate one with:
`
astartectl utils gen-jwt pairing -k <realm_private_key>
`
- `pairingUrl`: the URL of the Astarte Pairing endpoint. It should be something like `https://<api url>/pairing`.
- `realm`: the name of the Astarte realm.
- `persistencyDir`: When using Docker, allows to save the container's persistency through a Docker Volume.


## Build and run

Build the application using following commands:
```
$ qmake .
$ make
```
Then run the application with:
```
./stream-qt5-test --device <your device ID>
```
Several options are available:

- `--device` allows to set the device ID;
- `--function` allows to choose the data generation function (one between `sin`, `noisesin`, `randomspikessin`, `saw`, `rect`, `sinc`, `random`, `x` and a default one);
- `--interval` allows to set the sending interval;
- `--scale` allows to scale the generated result;
- `--interface` allows to specify the Astarte interface installed on the device;
- `--path` allows to specify the mapping endpoint for the interface.

## Configuring the streaming simulator via Astarte Interface

Configuring the streaming options [`function`, `interval`, `scale`] can be accomplished by sending some data from Astarte to the device via `org.astarte-platform.config.Telemetry` interface. This can be done using the Astarte AppEngine API or with astartectl command.
### `Sin` example:

- Enable the `sin` data generation function, with default period 
``` shell
astartectl appengine --appengine-url https://<api url>/appengine    \
 --token $APPENGINE_TOKEN --realm-name $REALM_NAME                  \ 
 devices send-data $DEVICE_ID                                       \
 org.astarte-platform.config.Telemetry                           \
 --interface-type properties --payload-type boolean                 \
 /request/sin/enable true  
```

- Change the `sin` sending interval to 1000ms
``` shell
astartectl appengine --appengine-url https://<api url>/appengine    \
 --token $APPENGINE_TOKEN --realm-name $REALM_NAME                  \ 
 devices send-data $DEVICE_ID                                       \
 org.astarte-platform.config.Telemetry                           \
 --interface-type properties --payload-type integer                 \
 /request/sin/periodMS 1000  
```

- Change the `sin` scale to 0.0001
``` shell
astartectl appengine --appengine-url https://<api url>/appengine    \
 --token $APPENGINE_TOKEN --realm-name $REALM_NAME                  \ 
 devices send-data $DEVICE_ID                                       \
 org.astarte-platform.config.Telemetry                           \
 --interface-type properties --payload-type double                 \
 /request/sin/scale 0.0001
```

Using the Docker image
====

Make sure to have [astartectl](https://github.com/astarte-platform/astartectl#installation) installed and your realm's private key at hand.

Pull the image with:
```
docker pull astarte/astarte-stream-qt5-test:1.0.4
```

If you are running the container on a different machine than your Astarte instance, you can invoke the application with:
```
docker run -e "DEVICE_ID=$(astartectl utils device-id generate-random)" -e "PAIRING_URL=<your pairing url>" -e "REALM=<realm name>" -e "PAIRING_JWT=$(astartectl utils gen-jwt pairing -k <realm private key>)" -e astarte/astarte-stream-qt5-test:1.0.4
```

Otherwise, to make `localhost` work, you need to specify an additional docker option:
```
docker run --net="host" -e "DEVICE_ID=$(astartectl utils device-id generate-random)" -e "PAIRING_URL=http://localhost:4003" -e "REALM=<realm name>" -e "PAIRING_JWT=$(astartectl utils gen-jwt pairing -k <realm private key>)" -e astarte/astarte-stream-qt5-test:1.0.4
```
Both commands will generate a random datastream from a brand new, random Device ID.

You can have the same Device ID send longer streams of data by saving the container's persistency through a Docker Volume. If you wish to do so, simply add `-v /persistency:<your persistency path>` to your `docker run` invocation.

You can refer to the previous section for more details on which variables (function, interface, etc...) can be passed to the container.
