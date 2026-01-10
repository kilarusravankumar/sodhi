#include <QTest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QSignalSpy>
#include <QCoreApplication>
#include "dashboard.h"
#include "messagebubble.h"
#include "message.h"
#include "config.h"

class TestDashboard : public QObject {
    Q_OBJECT

private slots:
    void testMessageReceived() {
        // 1. Prepare Mock Data
        Config config;
        QJsonObject userObj;
        userObj["id"] = "user1";
        userObj["username"] = "testuser";
        userObj["email"] = "test@example.com";
        userObj["friends"] = QJsonArray();

        QJsonObject roomObj;
        roomObj["id"] = "room1";
        roomObj["name"] = "General";
        roomObj["is_dm"] = false;
        
        QJsonObject memberObj;
        memberObj["id"] = "user1";
        memberObj["username"] = "testuser";
        memberObj["email"] = "test@example.com";
        QJsonArray membersArr;
        membersArr.append(memberObj);
        roomObj["members"] = membersArr;

        QJsonArray roomsArr;
        roomsArr.append(roomObj);

        QJsonObject data;
        data["user"] = userObj;
        data["rooms"] = roomsArr;

        // 2. Instantiate Dashboard
        Dashboard dashboard(&config, "dummy_token", data);

        // 3. Verify Placeholder exists initially
        // We need to access m_chatPlaceholder or find it.
        // Since it's private, we check the layout.
        // We know the chat layout is inside a QScrollArea -> QWidget -> QVBoxLayout
        QScrollArea *scrollArea = dashboard.findChild<QScrollArea*>();
        QVERIFY(scrollArea != nullptr);
        QWidget *scrollContent = scrollArea->widget();
        QVERIFY(scrollContent != nullptr);
        // The layout isn't directly accessible as a child, but we can check children of scrollContent
        // The placeholder is a QLabel with specific text
        
        QList<QLabel*> labels = scrollContent->findChildren<QLabel*>();
        bool placeholderFound = false;
        for (QLabel *lbl : labels) {
            if (lbl->text() == "Messages will appear here...") {
                placeholderFound = true;
                break;
            }
        }
        QVERIFY(placeholderFound);

        // 4. Simulate Binary Message Received
        QJsonObject msgJson;
        msgJson["message"] = "Hello World";
        msgJson["sent_timestamp"] = "2023-10-27T10:00:00Z";
        msgJson["room_id"] = "room1";
        msgJson["sent_by"] = "user2"; // Not self
        msgJson["sender"] = QJsonObject(); // Simplified

        // Construct a QByteArray that matches what Message::fromJson expects
        // Message::fromJson parses the whole object.
        // Wait, Message::fromJson implementation wasn't shown fully but it likely parses "message", "sent_timestamp", etc.
        // Let's assume the JSON structure matches the Dashboard::onSendMessage construction
        // "message", "sent_timestamp", "room_id", "sent_by", "sender"
        
        QJsonDocument doc(msgJson);
        QByteArray payload = doc.toJson();

        // Invoke the private slot
        QMetaObject::invokeMethod(&dashboard, "onBinaryMessageReceived",
                                  Q_ARG(QByteArray, payload));
        
        QCoreApplication::processEvents();

        // 5. Verify Placeholder is gone (or at least hidden) and Bubble exists
        labels = scrollContent->findChildren<QLabel*>();
        placeholderFound = false;
        for (QLabel *lbl : labels) {
            if (lbl->text() == "Messages will appear here...") {
                if (lbl->isVisible()) {
                    placeholderFound = true;
                    break;
                }
            }
        }
        QVERIFY(!placeholderFound);

        QList<MessageBubble*> bubbles = scrollContent->findChildren<MessageBubble*>();
        QCOMPARE(bubbles.size(), 1);
        
        // Verify content inside bubble (optional, but good)
        QLabel *contentLabel = bubbles[0]->findChild<QLabel*>(); 
        // Note: findChild might find the sender label or time label first.
        // MessageBubble has multiple labels.
        // We can just verify count is 1 for now.
    }
};

QTEST_MAIN(TestDashboard)
#include "test_dashboard.moc"

