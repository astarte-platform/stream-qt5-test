/*
 * This file is part of Astarte.
 *
 * Copyright 2018 Ispirata Srl
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "AstarteStreamQt5Test.h"

#include <AstarteDeviceSDK.h>

#include <HemeraCore/Operation>

#include <QtCore/QDir>
#include <QtCore/QTimer>

#define TELEMETRY_CONFIG_INTERFACE "org.astarte-platform.config.Telemetry"

AstarteStreamQt5Test::AstarteStreamQt5Test(const QByteArray &interface, const QByteArray &path,
    const QString &function, const QString &device, int interval, double scale, QObject *parent)
    : QObject(parent)
    , m_interface(interface)
    , m_path(path)
    , m_ready(false)
{

    m_sdk = new AstarteDeviceSDK(QDir::currentPath()
            + QStringLiteral("/astarte-device-%1-conf/transport-astarte.conf").arg(device),
        QDir::currentPath() + QStringLiteral("/interfaces"), device.toLatin1());
    AstarteStreamTelemetry::TelemetryFunction telemetryFunction
        = AstarteStreamTelemetry::getTelemetryFunction(function.toUtf8());
    AstarteStreamTelemetry *telemetry
        = new AstarteStreamTelemetry(m_sdk, interface, path, telemetryFunction, interval, scale);
    m_telemetries.insert(telemetryFunction, telemetry);

    connect(
        m_sdk->init(), &Hemera::Operation::finished, this, &AstarteStreamQt5Test::checkInitResult);
    connect(
        m_sdk, &AstarteDeviceSDK::dataReceived, this, &AstarteStreamQt5Test::handleIncomingData);
}

AstarteStreamQt5Test::~AstarteStreamQt5Test() {}

void AstarteStreamQt5Test::checkInitResult(Hemera::Operation *op)
{
    if (op->isError()) {
        qWarning() << "AstarteStreamQt5Test init error: " << op->errorName() << op->errorMessage();

    } else {
        m_ready = true;
        if (!m_telemetries.isEmpty()) {
            m_telemetries.first()->start();
        }
    }
}

void AstarteStreamQt5Test::handleIncomingData(
    const QByteArray &interface, const QByteArray &path, const QVariant &value)
{
    qDebug() << "Received data, interface: " << interface << "path: " << path
             << ", value: " << value << ", Qt type name: " << value.typeName();

    if (interface.compare(TELEMETRY_CONFIG_INTERFACE) == 0) {
        QList<QByteArray> paths = path.split('/');

        if (paths.isEmpty() || paths.length() <= 3) {
            return;
        }

        QByteArray functionName = paths[2];
        AstarteStreamTelemetry::TelemetryFunction telemetryFunction
            = AstarteStreamTelemetry::getTelemetryFunction(functionName);

        AstarteStreamTelemetry *telemetry;

        if (m_telemetries.contains(telemetryFunction)) {
            telemetry = m_telemetries.value(telemetryFunction);
        } else {
            telemetry = new AstarteStreamTelemetry(m_sdk, m_interface, m_path, telemetryFunction);
            m_telemetries.insert(telemetryFunction, telemetry);
        }

        QByteArray endpoint = paths[3];
        if (endpoint.compare("periodMS") == 0) {
            telemetry->setInterval(value.toInt());
        } else if (endpoint.compare("scale") == 0) {
            telemetry->setScale(value.toDouble());
        }

        if (endpoint.compare("enable") == 0) {
            telemetry->setEnable(value.toBool());
        } else {
            telemetry->restart();
        }
    }
}
