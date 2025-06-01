#ifndef __CORE_APPLICATION_PRIVATE_HPP__
#define __CORE_APPLICATION_PRIVATE_HPP__

#include <Opal/Core/application.hpp>
#include <QFileSystemWatcher>
#include <QWidget>

namespace Opal::Core {

class ApplicationPrivate : public QObject {
    Q_OBJECT;
    Q_DECLARE_PUBLIC(Application);

public:
    ApplicationPrivate(Core::Application* q);
    ~ApplicationPrivate();

    Core::Application* q_ptr;

    QString m_styleSheet;
    QString m_styleSheetPath;

    bool m_styleSheetCSSValid;
    bool m_styleSheetPathCSSValid;

    QString m_generatedCSS;
    QFileSystemWatcher m_styleSheetPathWatcher;

    void initStylePathWatcher();
    void updateStyleSheets();
};
};  // namespace Opal::Core

#endif