#include <Config/Widgets/Bar/bar.hpp>
#include <QApplication>

#include "Config/Widgets/SettingsMenu/settings_menu.hpp"

Config::Widgets::Bar::~Bar() {}
Config::Widgets::Bar::Bar(QScreen* screen, QWidget* parent)
    : Opal::Widgets::Window(screen, parent)
    , centerbox(QBoxLayout::Direction::LeftToRight, this)
    , mainLayout(QBoxLayout::Direction::LeftToRight, this) {
    using Anchor = LayerShellQt::Window::Anchor;

    layerShell()->setAnchors({ Anchor::AnchorBottom, Anchor::AnchorLeft, Anchor::AnchorRight });
    layerShell()->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);

    setExclusiveZoneType(ExclusiveType::AUTO);
    setMinimumHeight(32);

    mainLayout.setContentsMargins(8, 0, 8, 0);

    setLayout(&mainLayout);
    mainLayout.addWidget(&centerbox);

    label.setText("testing");
    settingsButton.setText("settings");
    close.setText("close");

    centerbox.left()->addWidget(&label);
    centerbox.center()->addWidget(&settingsButton);
    centerbox.right()->addWidget(&close);

    connect(&settingsButton, &QPushButton::clicked, []() {
        static Config::Widgets::SettingsMenu menu;
        menu.setVisible(!menu.isVisible());
    });

    connect(&close, &QPushButton::clicked, []() {
        QApplication::instance()->exit();
    });
}