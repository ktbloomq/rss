#include "mainwindow.h"
#include "post.h"
#include "postwidget.h"
#include "articlewidget.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    networkManager(new QNetworkAccessManager(this)),
    posts(),
    mediaProcess(new QProcess(this)),
    // Semaphore standin
    pendingFeedRequests(0)
{
    ui->setupUi(this);

    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::readRss);
    connect(ui->feedsWidget, &QTreeWidget::itemSelectionChanged, this, &MainWindow::onFeedSelectionChanged);
    connect(this, &MainWindow::feedProcessingFinished, this, &MainWindow::populateFeed);

    QTreeWidgetItem* yt = addFeedGroup("Youtube");
    addFeedSource("LMG Clips","https://www.youtube.com/feeds/videos.xml?channel_id=UCFLFc8Lpbwt4jPtY1_Ai5yA",yt);
    addFeedSource("Fools Gold Sands", "https://feeds.simplecast.com/rckmAK_v",yt);
}

MainWindow::~MainWindow()
{
    qDeleteAll(posts);
    delete ui;
}

void MainWindow::readRss(QNetworkReply *reply) {
    // Check for network errors
    pendingFeedRequests++;
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Network error:" << reply->errorString();
        reply->deleteLater();
        pendingFeedRequests--;
        emit feedProcessingFinished();
        return;
    }

    // Read the XML data
    QByteArray data = reply->readAll();

    // Set up QXmlStreamReader
    QXmlStreamReader xml(data);

    // Parse the XML
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        // Look for <item> elements
        if (xml.isStartElement() && (xml.name() == "entry" || xml.name() == "item")) {
            parseItem(xml);
        }
    }

    // Check for XML parsing errors
    if (xml.hasError()) {
        qDebug() << "XML error:" << xml.errorString();
    }

    // Clean up
    reply->deleteLater();
    pendingFeedRequests--;
    emit feedProcessingFinished();
}

void MainWindow::parseItem(QXmlStreamReader &xml) {
    QString title, description, link, image;
    QDateTime published;

    // Read until the end of the <item> element
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        // Stop if we reach the end of the <item>
        if (xml.isEndElement() && (xml.name() == "entry" || xml.name() == "item")) {
            break;
        }

        // Extract data from start elements
        if (xml.isStartElement()) {
            QStringView name = xml.name();
            // qDebug() << name;

            if (name == QString("title")) {
                title = xml.readElementText();
            } else if (name == QString("description")) {
                description = xml.readElementText();
            } else if (name == QString("link")) {
                link = xml.attributes().value("href").toString();
                // If no href attribute, fall back to element text
                if (link.isEmpty()) {
                    link = xml.readElementText();
                }
            } else if (name == QString("thumbnail")) {
                image = xml.attributes().value("url").toString();
            }
            else if (name == QString("pubDate")) {
                QString publishedString = xml.readElementText();
                published = QDateTime::fromString(publishedString, Qt::RFC2822Date);
            } else if (name == QString("published")) {
                QString publishedString = xml.readElementText();
                published = QDateTime::fromString(publishedString, Qt::ISODate);
            }
        }
    }
    Post *post = new Post(title, description, image, link, published);
    posts.append(post);
}

void MainWindow::populateFeed() {
    // Don't populate until all feeds are added
    if(pendingFeedRequests==0) {
        std::sort(posts.begin(), posts.end(),[](Post* a, Post* b){return a->published>b->published;});
        for(int i=0; i<posts.length(); i++) {
            PostWidget *postWidget = new PostWidget(this,posts[i]);
            connect(postWidget, &PostWidget::postClicked, this, &MainWindow::openPost);
            ui->postWidgetContainer->addWidget(postWidget);
        }
    }
}

QTreeWidgetItem* MainWindow::addFeedGroup(const QString &name) {
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->feedsWidget);
    item->setText(0, name);
    return item;
}

void MainWindow::addFeedSource(const QString &name, const QString &url, QTreeWidgetItem* parent) {
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, name);
    item->setData(0, Qt::UserRole, url); // Store URL in item data
    // feedSources.append({name, url});
}

void MainWindow::onFeedSelectionChanged() {
    QList<QTreeWidgetItem*> selectedItems = ui->feedsWidget->selectedItems();
    QTreeWidgetItem *item = selectedItems.first();
    int childCount = item->childCount();
    QString feedUrl = item->data(0, Qt::UserRole).toString();
    clearPosts();
    if (!feedUrl.isEmpty()) {
        fetchFeed(feedUrl);
    }
    for(int i=0; i<childCount;i++) {
        feedUrl = item->child(i)->data(0, Qt::UserRole).toString();
        qDebug() << feedUrl;
        if (!feedUrl.isEmpty()) {
            fetchFeed(feedUrl);
        }
    }
}

void MainWindow::fetchFeed(const QString &feedUrl) {
    QUrl url(feedUrl);
    if (url.isValid()) {
        QNetworkRequest request(url);
        networkManager->get(request);
    } else {
        qDebug() << "Invalid server URL.";
    }
}

void MainWindow::clearPosts()
{
    qDeleteAll(posts);
    posts.clear();
    QLayoutItem *item;
    while ((item = ui->postWidgetContainer->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

void MainWindow::openPost(Post* post) {
    QLayoutItem *item;
    while ((item = ui->articleContainer->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    ArticleWidget *articleWidget = new ArticleWidget(this,post);
    ui->articleContainer->addWidget(articleWidget);
}