#include <Config/Widgets/SettingsMenu/settings_menu.hpp>
#include <Config/private/widgets/settingsmenu/settings_menu_p.hpp>
#include <Opal/Core/Settings/settings.hpp>
#include <QWindow>

constexpr uint64_t hash(std::string_view str) {
    uint64_t hash = 0;
    for(char c : str) {
        hash = (hash * 131) + c;
    }

    return hash;
}

constexpr uint64_t operator"" _hash(const char* str, size_t len) { return hash(std::string_view(str, len)); }

Config::Widgets::SettingsMenuPrivate::~SettingsMenuPrivate() {}
Config::Widgets::SettingsMenuPrivate::SettingsMenuPrivate(Config::Widgets::SettingsMenu* q)
    : q_ptr(q)
    , m_quickSCSS()
    , m_applyButton("Apply")
    , m_mainLayout(QBoxLayout::Direction::TopToBottom, q) {
    m_mainLayout.addWidget(&m_quickSCSS);
    m_mainLayout.addSpacing(0);
    m_mainLayout.addWidget(&m_applyButton);

    m_quickSCSS.setTextBackgroundColor(QColor(0, 0, 0, 0));
    m_quickSCSS.setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);

    m_quickSCSS.setPlainText(Opal::Core::Settings::get("config")->value("style/quickSCSS").toString());

    q_ptr->winId();

    auto format = QSurfaceFormat::defaultFormat();
    // needed so the compositor doesn't get sent an opaque region
    format.setAlphaBufferSize(8);
    q_ptr->windowHandle()->setFormat(format);

    connect(Opal::Core::Settings::get("config").get(), &Opal::Core::Settings::valueChanged, this, &SettingsMenuPrivate::onConfigValueChanged);
    connect(&m_applyButton, &QPushButton::clicked, this, &SettingsMenuPrivate::applyConfigValues);
}

void Config::Widgets::SettingsMenuPrivate::onConfigValueChanged(QString key, QVariant value) {
    switch(hash(key.toStdString())) {
    case "style/quickSCSS"_hash:
        m_quickSCSS.setPlainText(value.toString());
        break;
    }
}

void Config::Widgets::SettingsMenuPrivate::applyConfigValues() {
    Opal::Core::Settings::get("config")->setValue("style/quickSCSS", m_quickSCSS.toPlainText());
}

Config::Widgets::SettingsMenu::~SettingsMenu() {}
Config::Widgets::SettingsMenu::SettingsMenu(QWidget* parent)
    : QWidget(parent)
    , d_ptr(new SettingsMenuPrivate(this)) {
}