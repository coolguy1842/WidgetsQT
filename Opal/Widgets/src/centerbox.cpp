#include <Opal/Widgets/centerbox.hpp>

class Opal::Widgets::CenterBoxPrivate {
public:
    Q_DECLARE_PUBLIC(Opal::Widgets::CenterBox);
    CenterBox* q_ptr;

    QBoxLayout* m_left;
    QBoxLayout* m_center;
    QBoxLayout* m_right;

    QBoxLayout* m_mainLayout;
    QMargins m_defaultMargins;

    CenterBoxPrivate(QBoxLayout::Direction boxDirection, Opal::Widgets::CenterBox* q)
        : q_ptr(q)
        , m_left(new QBoxLayout(boxDirection))
        , m_center(new QBoxLayout(boxDirection))
        , m_right(new QBoxLayout(boxDirection))
        , m_mainLayout(new QBoxLayout(boxDirection, q_ptr))
        , m_defaultMargins(0, 0, 0, 0) {
        m_left->setContentsMargins(m_defaultMargins);
        m_center->setContentsMargins(m_defaultMargins);
        m_right->setContentsMargins(m_defaultMargins);
        m_mainLayout->setContentsMargins(m_defaultMargins);

        m_mainLayout->addLayout(m_left, 0);
        m_mainLayout->setAlignment(m_left, Qt::AlignmentFlag::AlignLeading);

        m_mainLayout->addLayout(m_center, 0);
        m_mainLayout->setAlignment(m_center, Qt::AlignmentFlag::AlignCenter);

        m_mainLayout->addLayout(m_right, 0);
        m_mainLayout->setAlignment(m_right, Qt::AlignmentFlag::AlignTrailing);
    }
};

Opal::Widgets::CenterBox::~CenterBox() {}
Opal::Widgets::CenterBox::CenterBox(QBoxLayout::Direction direction, QWidget* parent)
    : QWidget(parent)
    , d_ptr(new Opal::Widgets::CenterBoxPrivate(direction, this)) {}

QBoxLayout* Opal::Widgets::CenterBox::left() { return d_ptr->m_left; }
QBoxLayout* Opal::Widgets::CenterBox::center() { return d_ptr->m_center; }
QBoxLayout* Opal::Widgets::CenterBox::right() { return d_ptr->m_right; }