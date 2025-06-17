#ifndef POST_H
#define POST_H
#include "qdatetime.h"

class Post
{
public:
    Post(QStringView, QStringView, QStringView, QStringView, QDateTime);
    QString title, description, image, link;
    QDateTime published;
};

#endif // POST_H
