#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QWidget>

class LandingPage : public QWidget
{
    Q_OBJECT

public:
    explicit LandingPage(QWidget *parent = nullptr);

signals:
    void loginClicked();
    void registerClicked();
};

#endif // LANDINGPAGE_H
