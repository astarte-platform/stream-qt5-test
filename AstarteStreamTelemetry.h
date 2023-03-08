/*
 * This file is part of Astarte.
 *
 * Copyright 2023 SECO Mind Srl
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ASTARTESTREAMTELEMETRY_H
#define ASTARTESTREAMTELEMETRY_H

#include <AstarteDeviceSDK.h>

#include <QtCore/QObject>
#include <QtCore/QTimer>

class AstarteStreamTelemetry : public QObject
{
    Q_OBJECT

public:
    enum class TelemetryFunction
    {
        Default = 0,
        Sin = 1,
        NoiseSin = 2,
        RandomSpikesSin = 3,
        X = 4,
        Saw = 5,
        Rect = 6,
        Sinc = 7,
        Random = 8
    };
    Q_ENUM(TelemetryFunction);

    AstarteStreamTelemetry(AstarteDeviceSDK *sdk, const QByteArray &interface,
        const QByteArray &path, AstarteStreamTelemetry::TelemetryFunction function, int interval = 1000, double scale = 1,
        QObject *parent = nullptr);
    ~AstarteStreamTelemetry();

    void start();
    void stop();
    void restart();
    void setInterval(int interval);
    void setScale(double scale);
    void setEnable(bool enable);

    static TelemetryFunction getTelemetryFunction(const QByteArray &functionName);

private slots:
    void sendValues();

private:
    AstarteDeviceSDK *m_sdk;
    QByteArray m_interface;
    QByteArray m_path;
    TelemetryFunction m_function;
    QTimer *m_updateTimer;
    double m_scale;
    double m_xValue;

    static int randomInterval();
};

#endif // ASTARTESTREAMTELEMETRY_H
