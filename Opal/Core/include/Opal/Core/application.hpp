#ifndef __CORE_APPLICATION_HPP__
#define __CORE_APPLICATION_HPP__

#include <QApplication>
#include <QScopedPointer>

namespace Opal::Core {

class ApplicationPrivate;
class Application : public QApplication {
    Q_OBJECT;
    Q_DECLARE_PRIVATE(Application);

public:
    Application(int argc, char** argv);
    ~Application();

    void setStyleSheetPath(QString path);
    bool styleSheetPathValid() const;

    // if stylesheet path is set, setstylesheet will append after
    void setStyleSheet(QString scss);

    // stylesheet will return the setStyleSheet value, not css generated from setStyleSheetPath
    QString styleSheet() const;
    bool styleSheetCSSValid() const;

    QString styleSheetPath() const;
    bool styleSheetPathCSSValid() const;

    // css generated from styleSheetPath & styleSheet
    QString generatedStyleSheet() const;
    void updateGeneratedStyleSheet();
    void reloadGeneratedStyleSheet();

private:
    QScopedPointer<ApplicationPrivate> d_ptr;
};

};  // namespace Opal::Core

#endif