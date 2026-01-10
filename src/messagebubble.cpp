#include "messagebubble.h"
#include <QDateTime>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <qdebug.h>
#include <qlogging.h>

MessageBubble::MessageBubble(const Message &message, bool isSelf,
                             QWidget *parent)
    : QWidget(parent) {
  setupUi(message, isSelf);
}

void MessageBubble::setupUi(const Message &message, bool isSelf) {
  // --- Styling Variables ---
  QString selfBgColor = "#DCF8C6";      // Light green
  QString otherBgColor = "#FFFFFF";     // White
  QString primaryTextColor = "#000000"; // Black
  QString senderTextColor = "#555555";  // Dark grey
  QString timeTextColor = "#777777";    // Grey
  QString borderColor = "#CCCCCC";
  int borderRadius = 10;

  QString senderFontSize = "10pt";
  QString contentFontSize = "11pt";
  QString timeFontSize = "8pt";

  QString bgColor = isSelf ? selfBgColor : otherBgColor;

  // Main layout for the row (contains spacer + bubble)
  QHBoxLayout *mainLayout = new QHBoxLayout(this);
  mainLayout->setContentsMargins(0, 5, 0, 5);

  // The bubble container
  QFrame *bubbleFrame = new QFrame(this);
  bubbleFrame->setFrameShape(QFrame::StyledPanel);
  bubbleFrame->setFrameShadow(QFrame::Raised);

  // Style the bubble
  bubbleFrame->setStyleSheet(
      QString(
          "QFrame { background-color: %1; border-radius: %2px; border: 1px "
          "solid %3; }"
          "QLabel { color: %4; border: none; background-color: transparent; }")
          .arg(bgColor, QString::number(borderRadius), borderColor,
               primaryTextColor));

  QVBoxLayout *bubbleLayout = new QVBoxLayout(bubbleFrame);
  bubbleLayout->setContentsMargins(10, 8, 10, 8);
  bubbleLayout->setSpacing(2);

  if (!isSelf) {
    QLabel *senderLabel = new QLabel(message.sentBy().username(), bubbleFrame);
    senderLabel->setStyleSheet(
        QString("font-weight: bold; font-size: %1; color: %2;")
            .arg(senderFontSize, senderTextColor));
    bubbleLayout->addWidget(senderLabel);
  }

  QLabel *contentLabel = new QLabel(message.msgContent(), bubbleFrame);
  contentLabel->setWordWrap(true);
  contentLabel->setStyleSheet(QString("font-size: %1;").arg(contentFontSize));
  bubbleLayout->addWidget(contentLabel);

  QLabel *timeLabel = new QLabel(message.sentTimestamp(), bubbleFrame);
  timeLabel->setAlignment(Qt::AlignRight);
  timeLabel->setStyleSheet(
      QString("font-size: %1; color: %2;").arg(timeFontSize, timeTextColor));
  bubbleLayout->addWidget(timeLabel);

  // Add to main layout with spacers for alignment
  if (isSelf) {
    mainLayout->addStretch();
    mainLayout->addWidget(bubbleFrame);
  } else {
    mainLayout->addWidget(bubbleFrame);
    mainLayout->addStretch();
  }
}
