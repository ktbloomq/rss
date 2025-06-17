#include "post.h"

Post::Post(QStringView title, QStringView description, QStringView image, QStringView link, QDateTime published):
    title(title),
    description(description),
    image(image),
    link(link),
    published(published) {
}
