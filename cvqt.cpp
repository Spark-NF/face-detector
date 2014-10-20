#define _USE_MATH_DEFINES

#include "cvqt.h"
#include <QDebug>
#include <math.h>

double Distance(cv::Point2f a, cv::Point2f b)
{
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return sqrt(dx*dx + dy*dy);
}
cv::Mat ScaleRotateTranslate(cv::Mat image, double angle, cv::Point2f center, cv::Point2f new_center, double scale = 1.0)
{
    cv::Mat warp, rot;
    if (new_center.x >= 0)
    {
        cv::Point2f srcTri[3];
        cv::Point2f dstTri[3];
        srcTri[0] = cv::Point2f(center.x, center.y);
        srcTri[1] = cv::Point2f(center.x + 1, center.y);
        srcTri[2] = cv::Point2f(center.x, center.y + 1);
        dstTri[0] = cv::Point2f(new_center.x, new_center.y);
        dstTri[1] = cv::Point2f(new_center.x + 1, new_center.y);
        dstTri[2] = cv::Point2f(new_center.x, new_center.y + 1);
        warp = cv::Mat::zeros(image.rows, image.cols, image.type());
        auto warp_mat = cv::getAffineTransform(srcTri, dstTri);
        cv::warpAffine(image, warp, warp_mat, warp.size());
    }
    else
        warp = image;
    auto rot_mat = cv::getRotationMatrix2D(center, angle, scale);
    cv::warpAffine(warp, rot, rot_mat, warp.size());
    return rot;
}
cv::Mat CropFace(cv::Mat image, cv::Point2f eyeLeft, cv::Point2f eyeRight, cv::Point2f offset_pct, cv::Point dest_sz)
{
    auto offset_h = floor(float(offset_pct.x)*dest_sz.x);
    auto offset_v = floor(float(offset_pct.y)*dest_sz.y);
    cv::Point2f eye_direction(eyeRight.x - eyeLeft.x, eyeRight.y - eyeLeft.y);

    auto rotation = atan2(float(eye_direction.y), float(eye_direction.x)) * 180 / M_PI;
    if (rotation < -45 || rotation > 45)
        return cv::Mat::zeros(0, 0, image.type());

    auto dist = Distance(eyeLeft, eyeRight);
    auto reference = dest_sz.x - 2.0 * offset_h;
    float scale = float(dist) / float(reference);

    cv::Point2f noPoint(-1, -1);
    image = ScaleRotateTranslate(image, rotation, eyeLeft, noPoint, 1.0);

    cv::Point2f crop_xy(eyeLeft.x - scale * offset_h, eyeLeft.y - scale*offset_v);
    cv::Point2f crop_size(dest_sz.x * scale, dest_sz.y * scale);
    if (crop_size.x < 50 || crop_size.y < 50)
        return cv::Mat::zeros(0, 0, image.type());
    image = cv::Mat(image, cv::Rect(int(crop_xy.x), int(crop_xy.y), int(crop_size.x), int(crop_size.y)));

    cv::resize(image, image, dest_sz, 0.0, 0.0, cv::INTER_LINEAR);
    return image;
}

QImage cvMatToQImage( const cv::Mat &inMat )
{
  switch ( inMat.type() )
  {
     // 8-bit, 4 channel
     case CV_8UC4:
     {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

        return image;
     }

     // 8-bit, 3 channel
     case CV_8UC3:
     {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

        return image.rgbSwapped();
     }

     // 8-bit, 1 channel
     case CV_8UC1:
     {
        static QVector<QRgb>  sColorTable;

        // only create our color table once
        if ( sColorTable.isEmpty() )
        {
           for ( int i = 0; i < 256; ++i )
              sColorTable.push_back( qRgb( i, i, i ) );
        }

        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );

        return image;
     }

     default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
  }

  return QImage();
}

QPixmap cvMatToQPixmap( const cv::Mat &inMat )
{
  return QPixmap::fromImage( cvMatToQImage( inMat ) );
}
