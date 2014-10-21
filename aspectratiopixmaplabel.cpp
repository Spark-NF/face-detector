#include "aspectratiopixmaplabel.h"
#include <QPainter>

AspectRatioPixmapLabel::AspectRatioPixmapLabel(QWidget *parent)
    : QLabel(parent), currEye(0), _changed(false)
{
    this->setMinimumSize(1,1);
}

void AspectRatioPixmapLabel::setPixmap ( const QPixmap & p)
{
    original = p;
    pix = p;
    QLabel::setPixmap(p);
}

int AspectRatioPixmapLabel::heightForWidth(int width) const
{
    return ((qreal)pix.height() * width) / pix.width();
}

QSize AspectRatioPixmapLabel::sizeHint() const
{
    int w = this->width();
    return QSize(w, heightForWidth(w));
}

void AspectRatioPixmapLabel::setEyes(QPoint eye1, int r1, QPoint eye2, int r2)
{
    _eye1 = eye1;
    _r1 = r1;
    _eye2 = eye2;
    _r2 = r2;

    updateEyes();
}

void AspectRatioPixmapLabel::updateEyes()
{
    pix = original;
    QPainter *paint = new QPainter(&pix);

    QPen pen;
    pen.setColor(QColor(255, 0, 0, 255));
    pen.setWidth(pix.width() / 150);
    paint->setPen(pen);
    paint->drawEllipse(_eye1, _r1, _r1);
    paint->drawEllipse(_eye2, _r2, _r2);
    paint->end();

    QLabel::setPixmap(pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void AspectRatioPixmapLabel::resizeEvent(QResizeEvent*)
{
    if (!pix.isNull())
        QLabel::setPixmap(pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void AspectRatioPixmapLabel::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton
            || e->button() == Qt::MiddleButton
            || e->button() == Qt::RightButton)
    {
        float ratio = qMax((float)pix.width() / width(), (float)pix.height() / height());
        int x = e->x() * ratio;
        int y = e->y() * ratio;
        if ((currEye == 0 && e->button() == Qt::MiddleButton) || e->button() == Qt::LeftButton)
        {
            _eye1 = QPoint(x, y);
        }
        else
        {
            _eye2 = QPoint(x, y);
        }
        if (e->button() == Qt::MiddleButton)
            currEye = (currEye + 1) % 2;
        _changed = true;
        updateEyes();
    }
    QLabel::mouseReleaseEvent(e);
}

QPoint AspectRatioPixmapLabel::leftEye()
{
    return _eye1.x() > _eye2.x() ? _eye2 : _eye1;
}
QPoint AspectRatioPixmapLabel::rightEye()
{
    return _eye1.x() > _eye2.x() ? _eye1 : _eye2;
}
bool AspectRatioPixmapLabel::changed()
{
    return _changed;
}
