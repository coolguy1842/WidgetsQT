#ifndef __WIDGETS_CENTERBOX_HPP__
#define __WIDGETS_CENTERBOX_HPP__

#include <QBoxLayout>
#include <QScopedPointer>
#include <QWidget>

namespace Opal::Widgets {

class CenterBoxPrivate;
class CenterBox : public QWidget {
    Q_OBJECT;
    Q_DECLARE_PRIVATE(Opal::Widgets::CenterBox);

public:
    Q_PROPERTY(QBoxLayout* left READ left);
    Q_PROPERTY(QBoxLayout* center READ center);
    Q_PROPERTY(QBoxLayout* right READ right);

public:
    CenterBox(QBoxLayout::Direction direction, QWidget* parent = nullptr);
    ~CenterBox();

    QBoxLayout* left();
    QBoxLayout* center();
    QBoxLayout* right();

private:
    QScopedPointer<CenterBoxPrivate> d_ptr;
};

};  // namespace Opal::Widgets

#endif