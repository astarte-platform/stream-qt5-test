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

#ifndef ASTARTESTREAMQT5TEST_H
#define ASTARTESTREAMQT5TEST_H

#include "AstarteStreamTelemetry.h"

#include <QtCore/QObject>

namespace Hemera {
class Operation;
}

class QTimer;

class AstarteDeviceSDK;

class AstarteStreamQt5Test : public QObject
{
    Q_OBJECT

public:
    AstarteStreamQt5Test(const QByteArray &interface, const QByteArray &path, const QString &function, const QString &device, int interval = 1, double scale = 1, QObject *parent = nullptr);
    ~AstarteStreamQt5Test();

private slots:
    void checkInitResult(Hemera::Operation *op);
    void handleIncomingData(const QByteArray &interface, const QByteArray &path, const QVariant &value);

private:
    AstarteDeviceSDK *m_sdk;
    QByteArray m_interface;
    QByteArray m_path;
    bool m_ready;
    QMap<AstarteStreamTelemetry::TelemetryFunction, AstarteStreamTelemetry*> m_telemetries;
};

#endif
