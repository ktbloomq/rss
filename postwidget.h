#ifndef POSTWIDGET_H
#define POSTWIDGET_H

#include "post.h"
#include "qnetworkreply.h"
#include <QFrame>
#include <QMouseEvent>

namespace Ui {
class PostWidget;
}

class PostWidget : public QFrame
{
    Q_OBJECT

public:
    explicit PostWidget(QWidget *parent = nullptr);
    PostWidget(QWidget *parent = nullptr, Post* = nullptr);
    ~PostWidget();

private slots:
    void onImageDownloaded(QNetworkReply*);

private:
    Ui::PostWidget *ui;
    QNetworkAccessManager *networkManager;
    const Post *post;
signals:
    void postClicked(const QString &postUrl);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // POSTWIDGET_H
