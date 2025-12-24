#include "roomslayout.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qjsondocument.h>

RoomsLayout::RoomsLayout(const QJsonObject &roomsData, QWidget *parent)
    : QWidget(parent) {
  setMinimumHeight(600);
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(10);

  QLabel *roomsTitle = new QLabel("Rooms", this);
  roomsTitle->setStyleSheet("font-size: 20px; font-weight: bold;");

  QScrollArea *roomsScrollArea = new QScrollArea(this);
  QWidget *roomsScrollContent = new QWidget(roomsScrollArea);
  QVBoxLayout *roomsScrollLayout = new QVBoxLayout(roomsScrollContent);
  roomsScrollLayout->setSpacing(15);
  roomsScrollLayout->setAlignment(Qt::AlignTop);

  const QJsonArray roomsArr = roomsData["rooms"].toArray();

  // so i have rooms
  // now move the logic here and add the user fucntionality to this class.
  //
  for (const QJsonValue &r : roomsArr) {
    const QJsonObject roomObj = r.toObject();
    QString roomName = roomObj["name"].toString();

    QLabel *roomLabel =
        new QLabel(QString("# %1").arg(roomName), roomsScrollContent);
    roomLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #999");
    roomsScrollLayout->addWidget(roomLabel);

    const QJsonArray members = roomObj["members"].toArray();
    for (const QJsonValue &memberValue : members) {
      const QJsonObject memberObject = memberValue.toObject();
      QString username = memberObject["username"].toString();
      QLabel *memberLabel =
          new QLabel(QString("@ %1").arg(username), roomsScrollContent);
      memberLabel->setStyleSheet("font-size: 11px; color: #999");
      roomsScrollLayout->addWidget(memberLabel);
    }
  }
  roomsScrollContent->setLayout(roomsScrollLayout);
  roomsScrollArea->setWidget(roomsScrollContent);
  mainLayout->addWidget(roomsScrollArea, 1);
  setLayout(mainLayout);
}
