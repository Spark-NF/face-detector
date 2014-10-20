#ifndef CVQT_H
#define CVQT_H

#include <QImage>
#include <QPixmap>
#include <opencv2/contrib/contrib.hpp>

QImage cvMatToQImage(const cv::Mat &inMat);
QPixmap cvMatToQPixmap(const cv::Mat &inMat);
cv::Mat CropFace(cv::Mat image, cv::Point2f eyeLeft, cv::Point2f eyeRight, cv::Point2f offset_pct, cv::Point dest_sz);

#endif // CVQT_H
