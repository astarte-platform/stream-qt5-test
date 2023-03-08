/*
 * This file is part of Edgehog.
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

#include "AstarteStreamTelemetry.h"

#include <QtCore/QDir>

#include <climits>
#include <cmath>

AstarteStreamTelemetry::AstarteStreamTelemetry(AstarteDeviceSDK *sdk, const QByteArray &interface,
    const QByteArray &path, TelemetryFunction function, int interval, double scale, QObject *parent)
    : QObject(parent)
    , m_sdk(sdk)
    , m_interface(interface)
    , m_path(path)
    , m_function(function)
    , m_updateTimer(new QTimer(this))
    , m_scale(scale)
    , m_xValue(0)

{
    this->setInterval(interval);
    connect(m_updateTimer, &QTimer::timeout, this, &AstarteStreamTelemetry::sendValues);
}

AstarteStreamTelemetry::~AstarteStreamTelemetry() {}

void AstarteStreamTelemetry::start()
{
    if (!m_updateTimer->isActive()) {
        m_updateTimer->start();
    }
}

void AstarteStreamTelemetry::restart()
{
    if (m_updateTimer->isActive()) {
        m_updateTimer->stop();
    }

    m_updateTimer->start();
}

void AstarteStreamTelemetry::stop()
{
    if (m_updateTimer->isActive()) {
        m_updateTimer->stop();
    }
}

void AstarteStreamTelemetry::setInterval(int interval)
{
    if (interval > 0) {
        m_updateTimer->setInterval(interval);
    } else {
        qsrand(QDateTime::currentMSecsSinceEpoch() % UINT_MAX);
        m_updateTimer->setInterval(randomInterval());
        m_updateTimer->setSingleShot(true);
    }
}

void AstarteStreamTelemetry::setScale(double scale)
{
    m_scale = scale;
    this->restart();
}

void AstarteStreamTelemetry::setEnable(bool enable)
{
    if (enable) {
        this->start();
    } else {
        this->stop();
    }
}

void AstarteStreamTelemetry::sendValues()
{
    double x = m_xValue;
    double value;

    switch (m_function) {
        case TelemetryFunction::Default: {
            value = (4 / M_PI) * sin(x) + (4 / 3) * M_PI * sin(3 * x) + (4 / 5) * M_PI * sin(5 * x)
                + (4 / 7) * M_PI * sin(7 * x);
        } break;
        case TelemetryFunction::Sin: {
            value = sin(x);
        } break;
        case TelemetryFunction::NoiseSin: {
            value = sin(x) + ((double) qrand()) / ((double) RAND_MAX) * 0.2;
        } break;
        case TelemetryFunction::RandomSpikesSin: {
            value = sin(x) + (((double) qrand()) / ((double) RAND_MAX) * 0.1)
                + ((((double) qrand()) / ((double) RAND_MAX)) > 0.999 ? 100 : 0);
        } break;
        case TelemetryFunction::X: {
            value = x;
        } break;
        case TelemetryFunction::Saw: {
            value = (fmod(x, 2 * M_PI) - M_PI) / M_PI;
        } break;
        case TelemetryFunction::Rect: {
            value = (fmod(x, 2 * M_PI) - M_PI) > 0 ? 1 : 0;
        } break;
        case TelemetryFunction::Sinc: {
            double t = fmod(x, 10 * 2 * M_PI) - M_PI * 10;
            value = (t != 0) ? (sin(t) / t) : 1;
        } break;
        case TelemetryFunction::Random: {
            value = ((double) qrand()) / ((double) RAND_MAX);
        } break;
    }

    qInfo() <<"functionId: " << m_function <<" " << x << ": " << QString::number(value, 'f', 5);

    m_sdk->sendData(m_interface, m_path, value, QDateTime::currentDateTime());

    if (m_updateTimer->isSingleShot()) {
        m_updateTimer->setInterval(randomInterval());
        m_updateTimer->start();
    }

    m_xValue += M_PI * 2 * ((double) m_updateTimer->interval()) * m_scale;
}

int AstarteStreamTelemetry::randomInterval()
{
    return (qrand() % 600) * 1000 + (qrand() % 1000);
}

AstarteStreamTelemetry::TelemetryFunction AstarteStreamTelemetry::getTelemetryFunction(
    const QByteArray &functionName)
{
    if (functionName == QStringLiteral("sin")) {
        return AstarteStreamTelemetry::TelemetryFunction::Sin;
    } else if (functionName == QStringLiteral("noisesin")) {
        return AstarteStreamTelemetry::TelemetryFunction::NoiseSin;
    } else if (functionName == QStringLiteral("randomspikessin")) {
        return AstarteStreamTelemetry::TelemetryFunction::RandomSpikesSin;
    } else if (functionName == QStringLiteral("x")) {
        return AstarteStreamTelemetry::TelemetryFunction::X;
    } else if (functionName == QStringLiteral("saw")) {
        return AstarteStreamTelemetry::TelemetryFunction::Saw;
    } else if (functionName == QStringLiteral("rect")) {
        return AstarteStreamTelemetry::TelemetryFunction::Rect;
    } else if (functionName == QStringLiteral("sinc")) {
        return AstarteStreamTelemetry::TelemetryFunction::Sinc;
    } else if (functionName == QStringLiteral("random")) {
        return AstarteStreamTelemetry::TelemetryFunction::Random;
    } else {
        return AstarteStreamTelemetry::TelemetryFunction::Default;
    }
}
