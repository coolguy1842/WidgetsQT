#include <application.hpp>

class Core::ApplicationPrivate {
public:
    Q_DECLARE_PUBLIC(Core::Application);
    Core::Application* q_ptr;

    ApplicationPrivate(Core::Application* q)
        : q_ptr(q) {}
};

Core::Application::~Application() {}
Core::Application::Application(int argc, char** argv)
    : QApplication(argc, argv)
    , d_ptr(new Core::ApplicationPrivate(this)) {}