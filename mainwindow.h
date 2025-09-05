#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtNetwork/qnetworkreply.h"
#include "post.h"
#include "qtreewidget.h"
#include "qxmlstream.h"
#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readRss(QNetworkReply*);
    void parseItem(QXmlStreamReader&);

private slots:
    void openPost(Post* post);
    void onFeedSelectionChanged();
    // void onMediaError(QProcess::ProcessError error);

signals:
    void feedProcessingFinished();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QList<Post*> posts;
    QProcess *mediaProcess;
    QTreeWidgetItem* addFeedGroup(const QString &name);
    void addFeedSource(const QString &name, const QString &url, QTreeWidgetItem* parent);
    void populateFeed();
    void fetchFeed(const QString &url);
    void clearPosts();
    int pendingFeedRequests;
};
#endif // MAINWINDOW_H
