#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <map>
#include <functional>

class CommandPalette : public QDialog {
  Q_OBJECT

public:
  static CommandPalette *instance(QWidget *parent = nullptr);
  ~CommandPalette() override;

  void reset();

signals:
  void createRoomRequested(const QString &roomName);

private slots:
  void onCommandSelected(QListWidgetItem *item);
  void onInputReturnPressed();

private:
  explicit CommandPalette(QWidget *parent = nullptr);
  Q_DISABLE_COPY(CommandPalette)

  static CommandPalette *m_instance;

  void setupUI();
  void showCommands();
  void showCreateRoomInput();
  void showAllAvailableRooms();
  bool eventFilter(QObject *watched, QEvent *event) override;

  QLineEdit *m_input;
  QListWidget *m_list;
  std::map<QString, std::function<void()>> commands;
  bool m_isInputMode; // false = command selection, true = parameter input
};
