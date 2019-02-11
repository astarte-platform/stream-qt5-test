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

    int interval;
    if (parser.value(QStringLiteral("interval")) == QStringLiteral("random")) {
        interval = -1;
    } else {
        interval = parser.value(QStringLiteral("interval")).toInt(&ok);
        if (!ok) {
            qWarning() << "Invalid interval.";
            return EXIT_FAILURE;
        }
    }
    double scale = parser.value(QStringLiteral("scale")).toDouble(&ok);
    if (!ok) {
        qWarning() << "Invalid scale.";
        return EXIT_FAILURE;
    }

    AstarteStreamQt5Test astarte(interface, path, function, deviceId, interval, scale);
    return app.exec();
}
