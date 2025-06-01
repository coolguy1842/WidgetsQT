#include <Config/Widgets/Bar/bar.hpp>
#include <Opal/Core/Settings/settings.hpp>
#include <Opal/Core/application.hpp>
#include <Opal/Util/SassEmbedded/sass.hpp>
#include <QList>

#include "qobject.h"
#include "qvariant.h"

static QList<Opal::Widgets::Window*> createWindowsForScreen(QScreen* screen) {
    auto bar = new Config::Widgets::Bar(screen);
    bar->show();

    return { bar };
}

static void syncWindows(QApplication* app, QList<Opal::Widgets::Window*>& windows) {
    QList<QScreen*> screens = app->screens();

    for(auto it = windows.begin(); it != windows.end();) {
        auto window = *it;
        if(window->screen() == nullptr || !screens.contains(window->screen())) {
            window->deleteLater();
            it = windows.erase(it);
            continue;
        }

        screens.removeAll(window->screen());
        it++;
    }

    for(auto screen : screens) {
        for(auto window : createWindowsForScreen(screen)) {
            windows.push_back(window);
        }
    }
}

int main(int argc, char** argv) {
    Opal::Core::Application app(argc, argv);
    auto settings = Opal::Core::Settings::get("config");

    app.setStyleSheet(settings->value("style/quickSCSS").toString());
    QObject::connect(settings.get(), &Opal::Core::Settings::valueChanged, [&app](QString key, QVariant value) {
        if(key != "style/quickSCSS") {
            return;
        }

        app.setStyleSheet(value.toString());
    });

    app.setStyleSheetPath(settings->value("style/path").toString());
    QObject::connect(settings.get(), &Opal::Core::Settings::valueChanged, [&app](QString key, QVariant value) {
        if(key != "style/path") {
            return;
        }

        app.setStyleSheetPath(value.toString());
    });

    QList<Opal::Widgets::Window*> windows;
    syncWindows(&app, windows);

    QObject::connect(&app, &QApplication::screenAdded, [&app, &windows](QScreen*) { syncWindows(&app, windows); });
    QObject::connect(&app, &QApplication::screenRemoved, [&app, &windows](QScreen*) { syncWindows(&app, windows); });

    int exitCode = app.exec();
    app.closeAllWindows();

    for(auto window : windows) {
        window->deleteLater();
    }

    app.deleteLater();

    return exitCode;
}