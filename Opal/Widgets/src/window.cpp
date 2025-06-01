#include <Opal/Widgets/window.hpp>

using Anchor = LayerShellQt::Window::Anchor;

class Opal::Widgets::WindowPrivate {
public:
    Q_DECLARE_PUBLIC(Opal::Widgets::Window);
    Opal::Widgets::Window* q_ptr;

    LayerShellQt::Window* m_layerShell;
    Opal::Widgets::Window::ExclusiveType m_exclusiveEdgeType;
    Opal::Widgets::Window::ExclusiveType m_exclusiveZoneType;

    WindowPrivate(Opal::Widgets::Window* q)
        : q_ptr(q)
        , m_layerShell(nullptr)
        , m_exclusiveEdgeType(Window::ExclusiveType::NORMAL)
        , m_exclusiveZoneType(Window::ExclusiveType::NORMAL) {
        q_ptr->winId();
        m_layerShell = LayerShellQt::Window::get(q_ptr->windowHandle());
        m_layerShell->setScreenConfiguration(LayerShellQt::Window::ScreenFromQWindow);

        auto format = QSurfaceFormat::defaultFormat();
        // needed so the compositor doesn't get sent an opaque region
        format.setAlphaBufferSize(8);

        q_ptr->windowHandle()->setFormat(format);
    }

    void recalculateExclusive() {
        switch(m_exclusiveEdgeType) {
        case Window::AUTO:
            if(m_layerShell->anchors() == 0b1111 || m_layerShell->anchors() == Anchor::AnchorNone) {
                break;
            }

            // vertical
            if(
                (m_layerShell->anchors() & Anchor::AnchorLeft && m_layerShell->anchors() & Anchor::AnchorRight) &&
                (m_layerShell->anchors() & Anchor::AnchorTop || m_layerShell->anchors() & Anchor::AnchorBottom)
            ) {
                m_layerShell->setExclusiveEdge(m_layerShell->anchors() & Anchor::AnchorTop ? Anchor::AnchorTop : Anchor::AnchorBottom);
                break;
            }
            // horizontal
            else if(
                (m_layerShell->anchors() & Anchor::AnchorTop && m_layerShell->anchors() & Anchor::AnchorBottom) &&
                (m_layerShell->anchors() & Anchor::AnchorLeft || m_layerShell->anchors() & Anchor::AnchorRight)
            ) {
                m_layerShell->setExclusiveEdge(m_layerShell->anchors() & Anchor::AnchorLeft ? Anchor::AnchorLeft : Anchor::AnchorRight);
                break;
            }

            m_layerShell->setExclusiveEdge(Anchor::AnchorNone);

            break;
        default: break;
        }

        switch(m_exclusiveZoneType) {
        case Window::ExclusiveType::AUTO:
            switch(m_layerShell->exclusiveEdge()) {
            case Anchor::AnchorLeft:
            case Anchor::AnchorRight: m_layerShell->setExclusiveZone(qMax(q_ptr->size().width(), q_ptr->sizeHint().width())); break;

            case Anchor::AnchorTop:
            case Anchor::AnchorBottom: m_layerShell->setExclusiveZone(qMax(q_ptr->size().height(), q_ptr->sizeHint().height())); break;

            default:
                if(m_layerShell->anchors() == 0b1111 || m_layerShell->anchors() == 0b0000) {
                    m_layerShell->setExclusiveZone(0);
                    break;
                }

                if((m_layerShell->anchors() & 0b0011) == 0b0011) {
                    m_layerShell->setExclusiveZone(qMax(q_ptr->width(), q_ptr->sizeHint().width()));
                }
                else if((m_layerShell->anchors() & 0b1100) == 0b1100) {
                    m_layerShell->setExclusiveZone(qMax(q_ptr->height(), q_ptr->sizeHint().height()));
                }
                else {
                    m_layerShell->setExclusiveZone(0);
                }

                break;
            }
            break;
        default: break;
        }
    }
};

Opal::Widgets::Window::~Window() {}
Opal::Widgets::Window::Window(QScreen* screen, QWidget* parent)
    : QWidget(parent)
    , d_ptr(new WindowPrivate(this)) {
    if(screen != nullptr) {
        setScreen(screen);
    }
    else {
        d_ptr->recalculateExclusive();
    }
}

LayerShellQt::Window* Opal::Widgets::Window::layerShell() { return d_ptr->m_layerShell; }
const LayerShellQt::Window* Opal::Widgets::Window::layerShell() const { return d_ptr->m_layerShell; }

Opal::Widgets::Window::ExclusiveType Opal::Widgets::Window::exclusiveEdgeType() const { return d_ptr->m_exclusiveEdgeType; }
Opal::Widgets::Window::ExclusiveType Opal::Widgets::Window::exclusiveZoneType() const { return d_ptr->m_exclusiveZoneType; }

void Opal::Widgets::Window::setScreen(QScreen* screen) {
    QWidget::setScreen(screen);
    windowHandle()->setScreen(screen);
}

void Opal::Widgets::Window::setExclusiveEdgeType(ExclusiveType type) {
    if(type == d_ptr->m_exclusiveEdgeType) {
        return;
    }

    if(d_ptr->m_exclusiveEdgeType == ExclusiveType::AUTO) {
        layerShell()->setExclusiveEdge(Anchor::AnchorNone);
    }

    d_ptr->m_exclusiveEdgeType = type;
    emit exclusiveEdgeTypeChanged(type);

    d_ptr->recalculateExclusive();
}

void Opal::Widgets::Window::setExclusiveZoneType(ExclusiveType type) {
    if(type == d_ptr->m_exclusiveZoneType) {
        return;
    }

    if(d_ptr->m_exclusiveZoneType == ExclusiveType::AUTO) {
        layerShell()->setExclusiveZone(0);
    }

    d_ptr->m_exclusiveZoneType = type;
    emit exclusiveZoneTypeChanged(type);

    d_ptr->recalculateExclusive();
}

void Opal::Widgets::Window::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    d_ptr->recalculateExclusive();
}