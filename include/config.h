#pragma once

#include <QString>
#include <QMap>

class Config {
public:
    explicit Config(const QString& envFilePath = ".env");
    QString get(const QString& key, const QString& defaultValue = "") const;

    // Helper getters for specific keys
    QString apiBaseUrl() const;
    QString wsBaseUrl() const;

private:
    void loadEnv(const QString& filePath);
    QMap<QString, QString> m_envVars;
};
