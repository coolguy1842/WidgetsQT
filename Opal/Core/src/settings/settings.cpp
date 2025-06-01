#include <Opal/Core/Settings/settings.hpp>
#include <QFileSystemWatcher>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include <memory>

class Opal::Core::SettingsPrivate {
public:
    Q_DECLARE_PUBLIC(Core::Settings);
    Core::Settings* q_ptr;

    int m_testid;
    QString m_id;
    QSettings m_settings;

    QFileSystemWatcher m_watcher;

    SettingsPrivate(Core::Settings* q, QString id)
        : q_ptr(q)
        , m_id(id)
        , m_settings("coolguy1842", id)
        , m_watcher(QStringList() << m_settings.fileName()) {
        QObject::connect(&m_watcher, &QFileSystemWatcher::fileChanged, [this]() {
            m_watcher.addPath(m_settings.fileName());
            QTimer::singleShot(50, [this]() { resyncSettings(); });
        });
    }

    void resyncSettings() {
        std::unordered_map<QString, QVariant> oldValues;

        for(auto key : m_settings.allKeys()) {
            oldValues[key] = m_settings.value(key);
        }

        m_settings.sync();

        QStringList newKeys = m_settings.allKeys();
        for(auto key : newKeys) {
            QVariant newValue = m_settings.value(key);
            if(!oldValues.contains(key)) {
                emit q_ptr->valueChanged(key, newValue);
                continue;
            }

            if(oldValues[key] != newValue) {
                emit q_ptr->valueChanged(key, newValue);
            }
        }

        for(auto pair : oldValues) {
            if(newKeys.contains(pair.first)) {
                continue;
            }

            emit q_ptr->valueChanged(pair.first, QVariant());
        }
    }
};

Opal::Core::Settings::~Settings() {}
Opal::Core::Settings::Settings(QString id)
    : d_ptr(new Opal::Core::SettingsPrivate(this, id)) {}

static QMap<QString, std::weak_ptr<Opal::Core::Settings>> settingsInstances;
std::shared_ptr<Opal::Core::Settings> Opal::Core::Settings::get(QString id) {
    auto it = settingsInstances.find(id);
    if(it != settingsInstances.end()) {
        if(settingsInstances[id].expired()) {
            settingsInstances.erase(it);
        }
        else {
            return settingsInstances[id].lock();
        }
    }

    std::shared_ptr<Opal::Core::Settings> ptr = std::shared_ptr<Opal::Core::Settings>(new Opal::Core::Settings(id));
    settingsInstances[id]                     = ptr;

    return ptr;
}

QVariant Opal::Core::Settings::value(const QAnyStringView& key) const { return d_ptr->m_settings.value(key); }
QVariant Opal::Core::Settings::value(const QAnyStringView& key, const QVariant& defaultValue) const { return d_ptr->m_settings.value(key, defaultValue); }

void Opal::Core::Settings::setValue(const QAnyStringView& key, const QVariant& value) {
    if(this->value(key) == value) {
        return;
    }

    d_ptr->m_settings.setValue(key, value);
    d_ptr->m_settings.sync();

    emit valueChanged(key.toString(), value);
}