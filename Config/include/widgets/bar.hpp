#ifndef __CONFIG_BAR_HPP__
#define __CONFIG_BAR_HPP__

#include <QBoxLayout>
#include <QLabel>
#include <widgets/window.hpp>

namespace Config::Widgets {

class Bar : public Core::Widgets::Window {
    Q_OBJECT;

    QLabel label;
    QBoxLayout layout;

public:
    ~Bar();
    Bar(QScreen* screen = nullptr);
};

};  // namespace Config::Widgets

#endif