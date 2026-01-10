#include "config.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

Config::Config(const QString& envFilePath) {
    // Check current directory
    QString path = envFilePath;
    
    // If not found, check project root (assuming we are in build dir)
    if (!QFile::exists(path)) {
         path = "../" + envFilePath;
    }

    loadEnv(path);
}

void Config::loadEnv(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Warning: Could not open .env file at:" << QDir::current().absoluteFilePath(filePath);
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("#")) continue;

        int equalPos = line.indexOf('=');
        if (equalPos > 0) {
            QString key = line.left(equalPos).trimmed();
            QString value = line.mid(equalPos + 1).trimmed();
            m_envVars[key] = value;
        }
    }
    file.close();
    qDebug() << "Loaded configuration from" << filePath;
}

QString Config::get(const QString& key, const QString& defaultValue) const {
    return m_envVars.value(key, defaultValue);
}

QString Config::apiBaseUrl() const {
    return get("API_BASE_URL", "http://localhost:8080/api/v1");
}

QString Config::wsBaseUrl() const {
    return get("WS_BASE_URL", "ws://localhost:8080/api/v1");
}
