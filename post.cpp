#include "post.h"

Post::Post(QString title, QString description, QString image, QString link, QDateTime published):
    title(title),
    description(description),
    image(image),
    link(link),
    published(published) {
}
