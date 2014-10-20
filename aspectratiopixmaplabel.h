#ifndef ASPECTRATIOPIXMAPLABEL_H
#define ASPECTRATIOPIXMAPLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class AspectRatioPixmapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit AspectRatioPixmapLabel(QWidget *parent = 0);
    virtual int heightForWidth( int width ) const;
    virtual QSize sizeHint() const;
    void setEyes(QPoint eye1, int r1, QPoint eye2, int r2);
    QPoint leftEye();
    QPoint rightEye();
    bool changed();

protected:
    void updateEyes();

public slots:
    void setPixmap(const QPixmap &);
    void resizeEvent(QResizeEvent *);
    void mouseReleaseEvent(QMouseEvent* e);

private:
    QPixmap original;
    QPixmap pix;
    QPoint _eye1, _eye2;
    int _r1, _r2;
    int currEye;
    bool _changed;
};

#endif // ASPECTRATIOPIXMAPLABEL_H
