#!/bin/bash

set -e

# Support old JWT naming, print warning
if [[ -v AGENT_KEY && ! -v PAIRING_JWT ]]; then
    echo "AGENT_KEY is deprecated and will be removed in a future release."
    echo "Start using PAIRING_JWT instead."
    PAIRING_JWT=$AGENT_KEY
fi

# Support old PAIRING_HOST, print warning
if [[ -v PAIRING_HOST && ! -v PAIRING_URL ]]; then
    echo "PAIRING_HOST is deprecated and will be removed in a future release."
    echo "Start using PAIRING_URL instead."
    PAIRING_URL=$PAIRING_HOST
fi

# Configure the device directory
mkdir astarte-device-$DEVICE_ID-conf
TRANSPORT_CONF_PATH="astarte-device-${DEVICE_ID}-conf/transport-astarte.conf"
cat > $TRANSPORT_CONF_PATH <<EOF
[AstarteTransport]
pairingUrl=$PAIRING_URL
realm=$REALM
persistencyDir=$PERSISTENCY_DIR
ignoreSslErrors=$IGNORE_SSL_ERRORS
keepAliveSeconds=$KEEPALIVE
EOF

# Check if CREDENTIALS_SECRET and PAIRING_JWT are both set, print warning
if [[ -v CREDENTIALS_SECRET && -v PAIRING_JWT ]]; then
    echo "CREDENTIALS_SECRET and PAIRING_JWT both set, CREDENTIALS_SECRET will be preferred."
    echo "credentialsSecret=$CREDENTIALS_SECRET" >> $TRANSPORT_CONF_PATH
elif [[ -v CREDENTIALS_SECRET ]]; then
    echo "credentialsSecret=$CREDENTIALS_SECRET" >> $TRANSPORT_CONF_PATH
else
    echo "pairingJwt=$PAIRING_JWT" >> $TRANSPORT_CONF_PATH
fi

exec "$@"
