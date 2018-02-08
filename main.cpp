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

#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QObject>

#include <HemeraCore/Operation>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    app.setApplicationName(QObject::tr("Astarte Qt5 Stream Test"));
    app.setOrganizationDomain(QStringLiteral("astarte-platform.org"));
    app.setOrganizationName(QStringLiteral("Astarte"));

    QCommandLineParser parser;
    parser.setApplicationDescription(QObject::tr("Astarte stream test."));
    parser.addVersionOption();
    parser.addHelpOption();

    parser.addOptions({
        {
            QStringList{QStringLiteral("f"), QStringLiteral("function")},
            QObject::tr("Function that will be generated."),
            QStringLiteral("function"),
            QStringLiteral("random")
        },
        {
            QStringList{QStringLiteral("s"), QStringLiteral("scale")},
            QObject::tr("Scale that will be used."),
            QStringLiteral("scale"),
            QStringLiteral("0.0001")
        },
        {
            QStringList{QStringLiteral("d"), QStringLiteral("device")},
            QObject::tr("Device ID that will be used."),
            QStringLiteral("device")
        },
        {
            QStringList{QStringLiteral("i"), QStringLiteral("interval")},
            QObject::tr("Interval in ms between two samples."),
            QStringLiteral("interval"),
            QStringLiteral("100")
        },
        {
            QStringList{QStringLiteral("n"), QStringLiteral("interface")},
            QObject::tr("Target interface"),
            QStringLiteral("interface"),
            QStringLiteral("org.astarteplatform.Values")
        },
        {
            QStringList{QStringLiteral("p"), QStringLiteral("path")},
            QObject::tr("Target path."),
            QStringLiteral("path"),
            QStringLiteral("/realValue")
        }
    });

    parser.process(app);

    bool ok;
    QByteArray interface = parser.value(QStringLiteral("interface")).toLatin1();
    QByteArray path = parser.value(QStringLiteral("path")).toLatin1();
    QString function = parser.value(QStringLiteral("function"));
    QString deviceId = parser.value(QStringLiteral("device"));

    QByteArray decodedDeviceId = QByteArray::fromBase64(deviceId.toLatin1(), QByteArray::Base64UrlEncoding);
    if (decodedDeviceId.count() != 16) {
        qWarning() << deviceId << " doesn't seem to be a valid device ID! Make sure to use an URL encoded base64 128 bits ID.";
        return EXIT_FAILURE;
    }

    int interval = parser.value(QStringLiteral("interval")).toInt(&ok);
    if (!ok) {
        qWarning() << "Invalid interval.";
        return EXIT_FAILURE;
    }
    double scale = parser.value(QStringLiteral("scale")).toDouble(&ok);
    if (!ok) {
        qWarning() << "Invalid scale.";
        return EXIT_FAILURE;
    }

    AstarteStreamQt5Test astarte(interface, path, function, deviceId, interval, scale);
    return app.exec();
}
