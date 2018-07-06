#!/bin/bash

set -e

# Configure the device directory
mkdir astarte-device-$DEVICE_ID-conf
cat > astarte-device-$DEVICE_ID-conf/transport-astarte.conf <<EOF
[AstarteTransport]
agentKey=$AGENT_KEY
endpoint=$PAIRING_HOST/v1/$REALM
persistencyDir=$PERSISTENCY_DIR
ignoreSslErrors=$IGNORE_SSL_ERRORS
EOF

exec "$@"
