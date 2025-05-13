#ifndef __CORE_WINDOW_HPP__
#define __CORE_WINDOW_HPP__

#include <LayerShellQt/Window>
#include <QWidget>

namespace Core::Widgets {

class WindowPrivate;
class Window : public QWidget {
    Q_OBJECT;

public:
    enum ExclusiveType {
        NORMAL,
        AUTO
    };

    Q_ENUMS(ExclusiveType);

    Q_PROPERTY(LayerShellQt::Window* layerShell READ layerShell);
    Q_PROPERTY(Core::Widgets::Window::ExclusiveType exclusiveEdgeType READ exclusiveEdgeType WRITE setExclusiveEdgeType NOTIFY exclusiveEdgeTypeChanged);
    Q_PROPERTY(Core::Widgets::Window::ExclusiveType exclusiveZoneType READ exclusiveZoneType WRITE setExclusiveZoneType NOTIFY exclusiveZoneTypeChanged);

    ~Window();
    Window(QScreen* screen = nullptr, QWidget* parent = nullptr);

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
    Q_DECLARE_PRIVATE(Window);
    WindowPrivate* d_ptr;
};

};  // namespace Core::Widgets

#endif