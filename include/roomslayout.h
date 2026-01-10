#pragma once

#include <QJsonObject>
#include <QWidget>
#include <qcontainerfwd.h>
#include <qwidget.h>
#include <room.h>

class QLabel;
class QVBoxLayout;
class QScrollArea;
class QPushButton;
class QLineEdit;

class RoomsLayout : public QWidget {
  Q_OBJECT

public:
  explicit RoomsLayout(const QJsonObject &roomsData, Room *activeRoom,
                       QWidget *parent = nullptr);

signals:
  void addUser(QString username);

private:
  void setUpLayout();
};
