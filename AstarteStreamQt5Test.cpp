/*
 * Copyright (C) 2018 Ispirata Srl
 *
 * This file is part of Astarte.
 * Astarte is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * Astarte is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Astarte.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AstarteStreamQt5Test.h"

#include <AstarteDeviceSDK.h>

#include <HemeraCore/Operation>

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QTimer>

#include <climits>
#include <math.h>

AstarteStreamQt5Test::AstarteStreamQt5Test(const QByteArray &interface, const QByteArray &path, const QString &function, const QString &device, int interval, double scale, QObject *parent)
    : QObject(parent)
    , m_interface(interface)
    , m_path(path)
    , m_function(function)
    , m_updateTimer(new QTimer(this))
    , m_scale(scale)
    , m_xValue(0)
    , m_ready(false)
{
    if (interval >= 0) {
        m_updateTimer->setInterval(interval);
    } else {
        qsrand(QDateTime::currentMSecsSinceEpoch() % UINT_MAX);
        m_updateTimer->setInterval(randomInterval());
        m_updateTimer->setSingleShot(true);
    }
    connect(m_updateTimer, &QTimer::timeout, this, &AstarteStreamQt5Test::sendValues);

    m_sdk = new AstarteDeviceSDK(QDir::currentPath() + QStringLiteral("/astarte-device-%1-conf/transport-astarte.conf").arg(device), QDir::currentPath() + QStringLiteral("/interfaces"), device.toLatin1());
    connect(m_sdk->init(), &Hemera::Operation::finished, this, &AstarteStreamQt5Test::checkInitResult);
    connect(m_sdk, &AstarteDeviceSDK::dataReceived, this, &AstarteStreamQt5Test::handleIncomingData);
}

AstarteStreamQt5Test::~AstarteStreamQt5Test()
{
}

void AstarteStreamQt5Test::checkInitResult(Hemera::Operation *op)
{
    if (op->isError()) {
        qWarning() << "AstarteStreamQt5Test init error: " << op->errorName() << op->errorMessage();

    } else {
        m_ready = true;
        m_updateTimer->start();
    }
}

void AstarteStreamQt5Test::sendValues()
{
    double x = m_xValue;
    double value;

    if (m_function == QStringLiteral("sin")) {
        value = sin(x);
    } else if (m_function == QStringLiteral("noisesin")) {
        value = sin(x) + ((double) qrand()) / ((double) RAND_MAX) * 0.2;
    } else if (m_function == QStringLiteral("randomspikessin")) {
        value = sin(x) + (((double) qrand()) / ((double) RAND_MAX) * 0.1) + ((((double) qrand()) / ((double) RAND_MAX)) > 0.999 ? 100 : 0);
    } else if (m_function == QStringLiteral("x")) {
        value = x;
    } else if (m_function == QStringLiteral("saw")) {
        value = (fmod(x, 2 * M_PI) - M_PI) / M_PI;
    } else if (m_function == QStringLiteral("rect")) {
        value = (fmod(x, 2 * M_PI) - M_PI) > 0 ? 1 : 0;
    } else if (m_function == QStringLiteral("sinc")) {
        double t = fmod(x, 10 * 2 * M_PI) - M_PI * 10;
        value = (t != 0) ? (sin(t) / t) : 1;
    } else if (m_function == QStringLiteral("random")) {
        value = ((double) qrand()) / ((double) RAND_MAX);
    } else {
        value = (4/M_PI)*sin(x) + (4/3)*M_PI*sin(3*x) + (4/5)*M_PI*sin(5*x) + (4/7)*M_PI*sin(7*x);
    }

    qDebug() << x << ": " << QString::number(value, 'f', 5);

    m_sdk->sendData(m_interface, m_path, value);

    if (m_updateTimer->isSingleShot()) {
        m_updateTimer->setInterval(randomInterval());
        m_updateTimer->start();
    }

    m_xValue += M_PI * 2 * ((double) m_updateTimer->interval()) * m_scale;
}

void AstarteStreamQt5Test::handleIncomingData(const QByteArray &interface, const QByteArray &path, const QVariant &value)
{
    qDebug() << "Received data, interface: " << interface << "path: " << path << ", value: " << value << ", Qt type name: " << value.typeName();
}

int AstarteStreamQt5Test::randomInterval()
{
    return (qrand() % 600) * 1000 + (qrand() % 1000);
}
