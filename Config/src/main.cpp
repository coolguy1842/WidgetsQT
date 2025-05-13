#include <application.hpp>
#include <widgets/bar.hpp>

int main(int argc, char** argv) {
    Core::Application app = Core::Application(argc, argv);

    Config::Widgets::Bar bar;
    bar.show();

    return app.exec();
}