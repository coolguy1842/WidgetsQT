#include <widgets/window.hpp>

class Core::Widgets::WindowPrivate {
public:
    Q_DECLARE_PUBLIC(Core::Widgets::Window);
    Core::Widgets::Window* q_ptr;

    LayerShellQt::Window* layerShell;
    Core::Widgets::Window::ExclusiveType exclusiveEdgeType;
    Core::Widgets::Window::ExclusiveType exclusiveZoneType;

    WindowPrivate(Core::Widgets::Window* q)
        : q_ptr(q)
        , layerShell(nullptr)
        , exclusiveEdgeType(Core::Widgets::Window::ExclusiveType::NORMAL)
        , exclusiveZoneType(Core::Widgets::Window::ExclusiveType::NORMAL) {
        q_ptr->winId();
        layerShell = LayerShellQt::Window::get(q_ptr->windowHandle());
        layerShell->setScreenConfiguration(LayerShellQt::Window::ScreenFromQWindow);

        auto format = QSurfaceFormat::defaultFormat();
        // needed so the compositor doesn't get sent an opaque region
        format.setAlphaBufferSize(8);

        q_ptr->windowHandle()->setFormat(format);
    }

    void recalculateExclusive() {
        using Anchor = LayerShellQt::Window::Anchor;

        switch(exclusiveEdgeType) {
        case Window::AUTO:
            if(layerShell->anchors() == 0b1111 || layerShell->anchors() == Anchor::AnchorNone) {
                break;
            }

            // vertical
            if(
                (layerShell->anchors() & Anchor::AnchorLeft && layerShell->anchors() & Anchor::AnchorRight) &&
                (layerShell->anchors() & Anchor::AnchorTop || layerShell->anchors() & Anchor::AnchorBottom)
            ) {
                layerShell->setExclusiveEdge(layerShell->anchors() & Anchor::AnchorTop ? Anchor::AnchorTop : Anchor::AnchorBottom);
                break;
            }
            // horizontal
            else if(
                (layerShell->anchors() & Anchor::AnchorTop && layerShell->anchors() & Anchor::AnchorBottom) &&
                (layerShell->anchors() & Anchor::AnchorLeft || layerShell->anchors() & Anchor::AnchorRight)
            ) {
                layerShell->setExclusiveEdge(layerShell->anchors() & Anchor::AnchorLeft ? Anchor::AnchorLeft : Anchor::AnchorRight);
                break;
            }

            layerShell->setExclusiveEdge(Anchor::AnchorNone);

            break;
        default: break;
        }

        switch(exclusiveZoneType) {
        case Core::Widgets::Window::ExclusiveType::AUTO:
            switch(layerShell->exclusiveEdge()) {
            case Anchor::AnchorLeft:
            case Anchor::AnchorRight: layerShell->setExclusiveZone(qMax(q_ptr->size().width(), q_ptr->sizeHint().width())); break;

            case Anchor::AnchorTop:
            case Anchor::AnchorBottom: layerShell->setExclusiveZone(qMax(q_ptr->size().height(), q_ptr->sizeHint().height())); break;

            default:
                if(layerShell->anchors() == 0b1111 || layerShell->anchors() == 0b0000) {
                    layerShell->setExclusiveZone(0);
                    break;
                }

                if((layerShell->anchors() & 0b0011) == 0b0011) {
                    layerShell->setExclusiveZone(qMax(q_ptr->width(), q_ptr->sizeHint().width()));
                }
                else if((layerShell->anchors() & 0b1100) == 0b1100) {
                    layerShell->setExclusiveZone(qMax(q_ptr->height(), q_ptr->sizeHint().height()));
                }
                else {
                    layerShell->setExclusiveZone(0);
                }

                break;
            }
            break;
        default: break;
        }

        printf("edge: %d, zone: %d\n", layerShell->exclusiveEdge(), layerShell->exclusionZone());
    }
};

Core::Widgets::Window::~Window() {}
Core::Widgets::Window::Window(QScreen* screen, QWidget* parent)
    : QWidget(parent)
    , d_ptr(new WindowPrivate(this)) {
    if(screen != nullptr) {
        setScreen(screen);
    }
    else {
        d_ptr->recalculateExclusive();
    }
}

LayerShellQt::Window* Core::Widgets::Window::layerShell() { return d_ptr->layerShell; }
const LayerShellQt::Window* Core::Widgets::Window::layerShell() const { return d_ptr->layerShell; }

Core::Widgets::Window::ExclusiveType Core::Widgets::Window::exclusiveEdgeType() const { return d_ptr->exclusiveEdgeType; }
Core::Widgets::Window::ExclusiveType Core::Widgets::Window::exclusiveZoneType() const { return d_ptr->exclusiveZoneType; }

void Core::Widgets::Window::setScreen(QScreen* screen) {
    QWidget::setScreen(screen);
    windowHandle()->setScreen(screen);
}

void Core::Widgets::Window::setExclusiveEdgeType(Core::Widgets::Window::ExclusiveType type) {
    if(type == d_ptr->exclusiveEdgeType) {
        return;
    }

    if(d_ptr->exclusiveEdgeType == ExclusiveType::AUTO) {
        layerShell()->setExclusiveEdge(LayerShellQt::Window::Anchor::AnchorNone);
    }

    d_ptr->exclusiveEdgeType = type;
    emit exclusiveEdgeTypeChanged(type);

    d_ptr->recalculateExclusive();
}

void Core::Widgets::Window::setExclusiveZoneType(Core::Widgets::Window::ExclusiveType type) {
    if(type == d_ptr->exclusiveZoneType) {
        return;
    }

    if(d_ptr->exclusiveZoneType == ExclusiveType::AUTO) {
        layerShell()->setExclusiveZone(0);
    }

    d_ptr->exclusiveZoneType = type;
    emit exclusiveZoneTypeChanged(type);

    d_ptr->recalculateExclusive();
}

void Core::Widgets::Window::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    d_ptr->recalculateExclusive();
}