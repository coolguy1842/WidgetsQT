#ifndef __CONFIG_PRIVATE_SETTINGS_MENU_HPP__
#define __CONFIG_PRIVATE_SETTINGS_MENU_HPP__

#include <Config/Widgets/SettingsMenu/settings_menu.hpp>
#include <QBoxLayout>
#include <QWidget>

namespace Config::Widgets {

class SettingsMenuPrivate : public QObject {
    Q_OBJECT;
    Q_DECLARE_PUBLIC(SettingsMenu);

public:
    SettingsMenuPrivate(SettingsMenu* q);
    ~SettingsMenuPrivate();

    Config::Widgets::SettingsMenu* q_ptr;

    QTextEdit m_quickSCSS;
    QPushButton m_applyButton;

    QBoxLayout m_mainLayout;

public slots:
    void onConfigValueChanged(QString key, QVariant value);
    void applyConfigValues();
};

};  // namespace Config::Widgets

#endif