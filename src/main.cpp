
/*
 * main.cpp - The entry point for the Sodhi C++/Qt Client.
 * This file creates and shows our first application window.
 */

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QStyleFactory>
#include <QVBoxLayout>
#include <QWidget>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setStyle(QStyleFactory::create("Fusion"));

  QString darktheme = R"(
        QWidget {
            background-color: #1e1e1e; 
            color: #ffffff;           
            font-size: 14px;
        }
        
        QPushButton {
            background-color: #3c3c3c;
            border: 1px solid #555555;
            padding: 5px 10px;
            border-radius: 4px;
        }

        QPushButton:hover {
            background-color: #4a4a4a; 
        }

        QPushButton:pressed {
            background-color: #2a2a2a;
        }

        QLineEdit {
            background-color: #2e2e2e;
            border: 1px solid #555555;
            padding: 5px;
            border-radius: 4px;
        }

        QLabel#errorLabel {
            color: #e57373; 
        }
        
        QStackedWidget, QWidget {
            border: none; 
        }
    )";
  app.setStyleSheet(darktheme);

  MainWindow window;

  window.show();
  return app.exec();
}
