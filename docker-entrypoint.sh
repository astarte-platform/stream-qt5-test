#!/bin/bash

set -e

# Support old JWT naming, print warning
if [[ -v $AGENT_KEY && ! -v $PAIRING_JWT ]]; then
    echo "AGENT_KEY is deprecated and will be removed in a future release."
    echo "Start using PAIRING_JWT instead."
    PAIRING_JWT=$AGENT_KEY
fi

# Support old PAIRING_HOST, print warning
if [[ -v $PAIRING_HOST && ! -v $PAIRING_URL ]]; then
    echo "PAIRING_HOST is deprecated and will be removed in a future release."
    echo "Start using PAIRING_URL instead."
    PAIRING_URL=$PAIRING_HOST
fi

# Configure the device directory
mkdir astarte-device-$DEVICE_ID-conf
cat > astarte-device-$DEVICE_ID-conf/transport-astarte.conf <<EOF
[AstarteTransport]
pairingJwt=$PAIRING_JWT
pairingUrl=$PAIRING_URL
realm=$REALM
persistencyDir=$PERSISTENCY_DIR
ignoreSslErrors=$IGNORE_SSL_ERRORS
keepAliveSeconds=$KEEPALIVE
EOF

exec "$@"
