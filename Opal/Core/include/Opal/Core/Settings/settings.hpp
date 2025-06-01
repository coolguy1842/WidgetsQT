#ifndef __CORE_SETTINGS_HPP__
#define __CORE_SETTINGS_HPP__

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QVariant>
#include <memory>

namespace Opal::Core {

class SettingsPrivate;
class Settings : public QObject {
    Q_OBJECT;
    Q_DECLARE_PRIVATE(Settings);

public:
    ~Settings();

    // id is the namespace
    static std::shared_ptr<Settings> get(QString id);

    QVariant value(const QAnyStringView& key) const;
    QVariant value(const QAnyStringView& key, const QVariant& defaultValue) const;

public slots:
    void setValue(const QAnyStringView& key, const QVariant& value);

signals:
    void valueChanged(QString key, QVariant value);

private:
    Settings(QString id);
    QScopedPointer<SettingsPrivate> d_ptr;
};

};  // namespace Opal::Core

#endif