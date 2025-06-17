#include "postwidget.h"
#include "post.h"
#include "qnetworkaccessmanager.h"
#include "qnetworkreply.h"
#include "ui_postwidget.h"

PostWidget::PostWidget(QWidget *parent):
    QFrame(parent),
    ui(new Ui::PostWidget)
{
    ui->setupUi(this);
}

PostWidget::PostWidget(QWidget *parent, Post *post):
    QFrame(parent),
    ui(new Ui::PostWidget),
    networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    this->post = post;
    connect(networkManager, &QNetworkAccessManager::finished, this, &PostWidget::onImageDownloaded);

    ui->title->setText(post->title);
    ui->description->setText(post->description.left(150)+"...");
    ui->published->setText(post->published.toString());

    if (!post->image.isEmpty()) {
        QNetworkRequest request(post->image);
        networkManager->get(request);
        ui->image->setText("Loading..."); // Placeholder while loading
    } else {
        ui->image->setText("");
    }
}

PostWidget::~PostWidget()
{
    delete ui;
}

void PostWidget::onImageDownloaded(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray imageData = reply->readAll();
        QPixmap pixmap;
        if (pixmap.loadFromData(imageData)) {
            // Scale image to fit the label while preserving aspect ratio
            pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->image->setPixmap(pixmap);
        } else {
            ui->image->setText("Failed to load image");
        }
    } else {
        ui->image->setText("Error loading image");
        qDebug() << "Image download error:" << reply->errorString();
    }
    reply->deleteLater();
}

void PostWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !post->link.isEmpty()) {
        // ui->image->setText(post->title);
        emit postClicked(post->link);
    }
    QWidget::mousePressEvent(event);
}
