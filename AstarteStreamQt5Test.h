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

#ifndef ASTARTESTREAMQT5TEST_H
#define ASTARTESTREAMQT5TEST_H

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
    void sendValues();

    void handleIncomingData(const QByteArray &interface, const QByteArray &path, const QVariant &value);

private:
    AstarteDeviceSDK *m_sdk;
    QByteArray m_interface;
    QByteArray m_path;
    QString m_function;
    QTimer *m_updateTimer;
    double m_scale;
    double m_xValue;
    bool m_ready;

    static int randomInterval();
};

#endif
