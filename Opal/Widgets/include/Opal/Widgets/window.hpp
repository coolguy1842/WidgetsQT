#ifndef __WIDGETS_WINDOW_HPP__
#define __WIDGETS_WINDOW_HPP__

#include <LayerShellQt/Window>
#include <QScopedPointer>
#include <QWidget>

namespace Opal::Widgets {

class WindowPrivate;
class Window : public QWidget {
    Q_OBJECT;
    Q_DECLARE_PRIVATE(Opal::Widgets::Window);

public:
    enum ExclusiveType {
        NORMAL,
        AUTO
    };

    Q_ENUMS(ExclusiveType);

    Q_PROPERTY(LayerShellQt::Window* layerShell READ layerShell);
    Q_PROPERTY(Opal::Widgets::Window::ExclusiveType exclusiveEdgeType READ exclusiveEdgeType WRITE setExclusiveEdgeType NOTIFY exclusiveEdgeTypeChanged);
    Q_PROPERTY(Opal::Widgets::Window::ExclusiveType exclusiveZoneType READ exclusiveZoneType WRITE setExclusiveZoneType NOTIFY exclusiveZoneTypeChanged);

public:
    Window(QScreen* screen = nullptr, QWidget* parent = nullptr);
    ~Window();

    LayerShellQt::Window* layerShell();
    const LayerShellQt::Window* layerShell() const;

    ExclusiveType exclusiveEdgeType() const;
    ExclusiveType exclusiveZoneType() const;

    void setExclusiveEdgeType(ExclusiveType type);
    void setExclusiveZoneType(ExclusiveType type);
    void setScreen(QScreen* screen);

signals:
    void exclusiveEdgeTypeChanged(ExclusiveType);
    void exclusiveZoneTypeChanged(ExclusiveType);

protected:
    virtual void resizeEvent(QResizeEvent* event);

private:
    QScopedPointer<WindowPrivate> d_ptr;
};

};  // namespace Opal::Widgets

#endif