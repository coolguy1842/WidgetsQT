#ifndef __CONFIG_BAR_HPP__
#define __CONFIG_BAR_HPP__

#include <Opal/Widgets/centerbox.hpp>
#include <Opal/Widgets/window.hpp>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScreen>
#include <QWidget>

namespace Config::Widgets {

class Bar : public Opal::Widgets::Window {
    Q_OBJECT;

    Opal::Widgets::CenterBox centerbox;
    QLabel label;
    QPushButton settingsButton;
    QPushButton close;

    QBoxLayout mainLayout;

public:
    ~Bar();
    Bar(QScreen* screen = nullptr, QWidget* parent = nullptr);
};

};  // namespace Config::Widgets

#endif