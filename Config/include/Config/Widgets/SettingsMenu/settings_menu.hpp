#ifndef __CONFIG_SETTINGS_MENU_HPP__
#define __CONFIG_SETTINGS_MENU_HPP__

#include <QLabel>
#include <QPushButton>
#include <QScreen>
#include <QTextEdit>
#include <QWidget>

namespace Config::Widgets {

class SettingsMenuPrivate;
class SettingsMenu : public QWidget {
    Q_OBJECT;
    Q_DECLARE_PRIVATE(SettingsMenu);

public:
    ~SettingsMenu();
    SettingsMenu(QWidget* parent = nullptr);

private:
    QScopedPointer<SettingsMenuPrivate> d_ptr;
};

};  // namespace Config::Widgets

#endif