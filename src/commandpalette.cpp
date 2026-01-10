#include "commandpalette.h"
#include <QEvent>
#include <QKeyEvent>

CommandPalette *CommandPalette::m_instance = nullptr;

CommandPalette *CommandPalette::instance(QWidget *parent) {
  if (!m_instance) {
    m_instance = new CommandPalette(parent);
  } else {
    // If reusing, we reset the state and ensure it's properly parented/centered
    m_instance->setParent(parent);
    m_instance->reset();
  }
  return m_instance;
}

CommandPalette::CommandPalette(QWidget *parent)
    : QDialog(parent), m_isInputMode(false) {
  setupUI();
  commands["Create Room"] = [this]() { showCreateRoomInput(); };
  commands["Join Room"] = [this]() { showAllAvailableRooms(); };
  showCommands();
}

CommandPalette::~CommandPalette() { m_instance = nullptr; }

void CommandPalette::setupUI() {
  setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
  setFixedSize(600, 400);

  // Simple styling to mimic a palette
  setStyleSheet(
      "QDialog { background-color: #252526; border: 1px solid #454545; }"
      "QLineEdit { background-color: #3c3c3c; color: #cccccc; border: 1px "
      "solid #3c3c3c; padding: 5px; font-size: 14px; }"
      "QListWidget { background-color: #252526; color: #cccccc; border: none; "
      "font-size: 14px; }"
      "QListWidget::item:selected { background-color: #094771; }");

  QVBoxLayout *layout = new QVBoxLayout(this);
  m_input = new QLineEdit(this);
  m_input->setPlaceholderText(">");
  m_input->installEventFilter(this);

  m_list = new QListWidget(this);

  layout->addWidget(m_input);
  layout->addWidget(m_list);

  connect(m_list, &QListWidget::itemClicked, this,
          &CommandPalette::onCommandSelected);
  connect(m_input, &QLineEdit::returnPressed, this,
          &CommandPalette::onInputReturnPressed);
}

void CommandPalette::showCommands() {
  m_list->clear();

  for (const auto &[key, value] : commands) {
    m_list->addItem(key);
  }
  if (m_list->count() > 0) {
    m_list->setCurrentRow(0);
  }

  m_input->setPlaceholderText("Type a command...");
  m_isInputMode = false;
  m_input->setFocus();
}

bool CommandPalette::eventFilter(QObject *watched, QEvent *event) {
  if (watched == m_input && event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() == Qt::Key_Down) {
      int nextRow = m_list->currentRow() + 1;
      if (nextRow < m_list->count()) {
        m_list->setCurrentRow(nextRow);
      }
      return true;
    } else if (keyEvent->key() == Qt::Key_Up) {
      int prevRow = m_list->currentRow() - 1;
      if (prevRow >= 0) {
        m_list->setCurrentRow(prevRow);
      }
      return true;
    }
  }
  return QDialog::eventFilter(watched, event);
}

void CommandPalette::onCommandSelected(QListWidgetItem *item) {
  if (!item) return;

  auto it = commands.find(item->text());
  if (it != commands.end() && it->second) {
    it->second();
  }
}

void CommandPalette::onInputReturnPressed() {
  if (m_isInputMode) {
    QString text = m_input->text();
    if (!text.trimmed().isEmpty()) {
      emit createRoomRequested(text);
      accept();
    }
  } else {
    QListWidgetItem *current = m_list->currentItem();
    if (current) {
      onCommandSelected(current);
    }
  }
}

void CommandPalette::showCreateRoomInput() {
  m_isInputMode = true;
  m_list->clear();
  m_input->clear();
  m_input->setPlaceholderText("Enter room name...");
}

void CommandPalette::showAllAvailableRooms() {}

void CommandPalette::reset() {
  m_input->clear();
  showCommands();
}
