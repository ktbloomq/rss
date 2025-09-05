#ifndef ARTICLEWIDGET_H
#define ARTICLEWIDGET_H

#include "post.h"
#include <QFrame>
#include <QProcess>

namespace Ui {
class ArticleWidget;
}

class ArticleWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ArticleWidget(QWidget *parent = nullptr, Post* post = nullptr);
    ~ArticleWidget();

private:
    Ui::ArticleWidget *ui;
    Post* post;
    QProcess mediaProcess;
    void openMedia();
};

#endif // ARTICLEWIDGET_H
