#ifndef __CORE_APPLICATION_HPP__
#define __CORE_APPLICATION_HPP__

#include <QApplication>

namespace Core {

class ApplicationPrivate;
class Application : public QApplication {
    Q_OBJECT;

public:
    ~Application();
    Application(int argc, char** argv);

private:
    Q_DECLARE_PRIVATE(Application);
    Q_D(Application);

    ApplicationPrivate* d_ptr;
};

};  // namespace Core

#endif