#include "QStringList"
#include "QMessageBox"
#include "articlewidget.h"
#include "ui_articlewidget.h"

ArticleWidget::ArticleWidget(QWidget *parent, Post* post): 
    QFrame(parent),
    ui(new Ui::ArticleWidget),
    post(post),
    mediaProcess(new QProcess(this))
{
    ui->setupUi(this);
    ui->title->setText(post->title);
    ui->description->setText(post->description);
    ui->published->setText(post->published.date().toString());
    connect(ui->playButton, &QPushButton::clicked, this, &ArticleWidget::openMedia);
}

ArticleWidget::~ArticleWidget()
{
    delete ui;
}

void ArticleWidget::openMedia() {
    if (post->link.isEmpty()) {
        QMessageBox::warning(this, "Error", "No URL provided for this post.");
        return;
    }
    // QMessageBox::information(this,"Info",postUrl);

    // Launch MPV with the URL
    QStringList arguments;
    arguments << "--force-window" << post->link;
    mediaProcess.start("mpv", arguments);

    if (!mediaProcess.waitForStarted(3000)) {
        QMessageBox::critical(this, "Error", "Failed to start MPV. Ensure MPV is installed and accessible.");
    }
}