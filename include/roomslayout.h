#pragma once

#include <QJsonObject>
#include <QWidget>
#include <qcontainerfwd.h>
#include <qwidget.h>

class QLabel;
class QVBoxLayout;
class QScrollArea;
class QPushButton;
class QLineEdit;

class RoomsLayout : public QWidget {
  Q_OBJECT

public:
  explicit RoomsLayout(const QJsonObject &roomsData, QWidget *parent = nullptr);

signals:
  void addUser(QString username);

private:
  void setUpLayout();
};
