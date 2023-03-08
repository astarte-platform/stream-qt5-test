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

Add `pairingJwt` or  `credentialsSecret` ( if you add both credentialsSecret will be preferred ) to the configuration file:
- `pairingJwt`: If you don't already have an Astarte pairing JWT, you can generate one with:
  ```shell
  astartectl utils gen-jwt pairing -k <realm_private_key>
  ``` 
- `credentialsSecret`: You can register a Device ID via astartectl:
  ``` shell
  astartectl pairing agent --astarte-url https://<api url>/appengine \
  --realm-name <realm name> --token $PAIRING_JWT                     \
  register $DEVICE_ID --compact-output
  ```
Then customize the following fields:
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


Using the Docker image
====

Make sure to have [astartectl](https://github.com/astarte-platform/astartectl#installation) installed and your realm's private key at hand.

Pull the image with:
``` shell
docker pull astarte/astarte-stream-qt5-test:1.1.0
```

If you are running the container on a different machine than your Astarte instance, you can invoke the application with:
``` shell
docker run -e "DEVICE_ID=$(astartectl utils device-id generate-random)" -e "PAIRING_URL=<your pairing url>" -e "REALM=<realm name>" -e "PAIRING_JWT=$(astartectl utils gen-jwt pairing -k <realm private key>)" astarte/astarte-stream-qt5-test:1.1.0
```

Otherwise, to make `localhost` work, you need to specify an additional docker option:
``` shell
docker run --net="host" -e "DEVICE_ID=$(astartectl utils device-id generate-random)" -e "PAIRING_URL=http://localhost:4003" -e "REALM=<realm name>" -e "PAIRING_JWT=$(astartectl utils gen-jwt pairing -k <realm private key>)" astarte/astarte-stream-qt5-test:1.1.0
```

Both commands will generate a random datastream from a brand new, random Device ID.

You can have the same Device ID send longer streams of data by saving the container's persistency through a Docker Volume. If you wish to do so, simply add `-v /persistency:<your persistency path>` to your `docker run` invocation.

If you are running the container using a credentials secret instead of pairing token, you can invoke the application with:
``` shell
export DEVICE_ID=$(astartectl utils device-id generate-random)
export CREDENTIALS_SECRET=$(astartectl pairing agent --astarte-url https://<api url>/appengine --realm-name <realm name> --token $(astartectl utils gen-jwt pairing -k <realm private key>) register $DEVICE_ID --compact-output)
docker run --net="host" -e "DEVICE_ID=$(astartectl utils device-id generate-random) -e "DEVICE_ID=$DEVICE_ID" -e "PAIRING_URL=<your pairing url>" -e "REALM=<realm name>" -e "CREDENTIALS_SECRET=$CREDENTIALS_SECRET" -e "PERSISTENCY_DIR=./" astarte/astarte-stream-qt5-test:1.1.0
```

You can refer to the previous section for more details on which variables (function, interface, etc...) can be passed to the container.
