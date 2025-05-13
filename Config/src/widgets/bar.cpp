#include <widgets/bar.hpp>

Config::Widgets::Bar::~Bar() {}
Config::Widgets::Bar::Bar(QScreen* screen)
    : Core::Widgets::Window(screen)
    , layout(QBoxLayout::Direction::LeftToRight) {
    using Anchor = LayerShellQt::Window::Anchor;

    layerShell()->setAnchors({ Anchor::AnchorBottom, Anchor::AnchorLeft, Anchor::AnchorRight });
    layerShell()->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);

    setExclusiveZoneType(ExclusiveType::AUTO);

    setMinimumHeight(32);
    layout.setContentsMargins(QMargins(0, 0, 0, 0));

    label.setText("testing");
    layout.addWidget(&label);

    setLayout(&layout);
}