#pragma once

#include <QWidget>
#include "message.h"

class MessageBubble : public QWidget {
  Q_OBJECT
public:
  explicit MessageBubble(const Message &message, bool isSelf, QWidget *parent = nullptr);

private:
  void setupUi(const Message &message, bool isSelf);
};
