#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <map>

#include <QMainWindow>
#include <QWebChannel>

#include "client.h"
#include "uploader.h"

#include "PagesMappings.h"
#include "CallbackWrapper.h"

class WebSocketClient;
class JavascriptWrapper;
class MHUrlSchemeHandler;
namespace auth {
class AuthJavascript;
class Auth;
}
namespace messenger {
class MessengerJavascript;
}
namespace proxy {
class ProxyJavascript;
}

namespace Ui {
class MainWindow;
}

namespace initializer {
class InitializerJavascript;
}

class EvFilter: public QObject {
    Q_OBJECT
public:

    EvFilter(QObject *parent, const QString &icoActive, const QString &icoHover)
        : QObject(parent)
        , icoActive(icoActive)
        , icoHover(icoHover)
    {}

    bool eventFilter(QObject * watched, QEvent * event);

    QIcon icoActive;
    QIcon icoHover;
};

struct TypedException;

namespace transactions {
class TransactionsJavascript;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    using SetJavascriptWrapperCallback = CallbackWrapper<void()>;

    using SetAuthCallback = CallbackWrapper<void()>;

    using SetMessengerJavascriptCallback = CallbackWrapper<void()>;

    using SetTransactionsJavascriptCallback = CallbackWrapper<void()>;

    using SetProxyJavascriptCallback = CallbackWrapper<void()>;

public:

    explicit MainWindow(initializer::InitializerJavascript &initializerJs, QWidget *parent = 0);

    ~MainWindow();

    void showExpanded();

    QString getServerIp(const QString &text, const std::set<QString> &excludesIps);

    LastHtmlVersion getCurrentHtmls() const;

signals:

    void setJavascriptWrapper(JavascriptWrapper *jsWrapper, const SetJavascriptWrapperCallback &callback);

    void setAuth(auth::AuthJavascript *authJavascript, auth::Auth *authManager, const SetAuthCallback &callback);

    void setMessengerJavascript(messenger::MessengerJavascript *messengerJavascript, const SetMessengerJavascriptCallback &callback);

    void setTransactionsJavascript(transactions::TransactionsJavascript *transactionsJavascript, const SetTransactionsJavascriptCallback &callback);

    void setProxyJavascript(proxy::ProxyJavascript *transactionsJavascript, const SetProxyJavascriptCallback &callback);

    void initFinished();

private slots:

    void onSetJavascriptWrapper(JavascriptWrapper *jsWrapper, const SetJavascriptWrapperCallback &callback);

    void onSetAuth(auth::AuthJavascript *authJavascript, auth::Auth *authManager, const SetAuthCallback &callback);

    void onSetMessengerJavascript(messenger::MessengerJavascript *messengerJavascript, const SetMessengerJavascriptCallback &callback);

    void onSetTransactionsJavascript(transactions::TransactionsJavascript *transactionsJavascript, const SetTransactionsJavascriptCallback &callback);

    void onSetProxyJavascript(proxy::ProxyJavascript *proxyJavascript, const SetProxyJavascriptCallback &callback);

    void onInitFinished();

private:

    void loadPagesMappings();

    void softReloadPage();

    void softReloadApp();

    void loadUrl(const QString &page);

    void loadFile(const QString &pageName);

    bool currentFileIsEqual(const QString &pageName);

    void configureMenu();

    void doConfigureMenu();

    void registerCommandLine();

    void unregisterCommandLine();

    void enterCommandAndAddToHistory(const QString &text1, bool isAddToHistory, bool isNoEnterDuplicate);

    void addElementToHistoryAndCommandLine(const QString &text, bool isAddToHistory, bool isReplace);

    void qtOpenInBrowser(QString url);

    void setUserName(QString userName);

public slots:

    void updateHtmlsEvent();

    void updateAppEvent(const QString appVersion, const QString reference, const QString message);

private slots:

    void onCallbackCall(SimpleClient::ReturnCallback callback);

    void onShowContextMenu(const QPoint &point);

    void onJsRun(QString jsString);

    void onSetHasNativeToolbarVariable();

    void onSetCommandLineText(QString text);

    void onSetMappings(QString mapping);

    void onEnterCommandAndAddToHistory(const QString &text);

    void onEnterCommandAndAddToHistoryNoDuplicate(const QString &text);

    void onUrlChanged(const QUrl &url2);

    void onLogined(bool isInit, const QString &login);

private:

    MHUrlSchemeHandler *shemeHandler = nullptr;

    std::unique_ptr<Ui::MainWindow> ui;

    std::unique_ptr<QWebChannel> channel;

    JavascriptWrapper *jsWrapper = nullptr;

    LastHtmlVersion last_htmls;
    mutable std::mutex mutLastHtmls;

    QString currentTextCommandLine;

    PagesMappings pagesMappings;

    QString hardwareId;

    size_t countFocusLineEditChanged = 0;

    std::vector<QString> history;
    size_t historyPos = 0;

    SimpleClient client;

    QString prevUrl;

    QString prevTextCommandLine;

    QString currentUserName;

    QString urlDns;

    QString netDns;

    bool lineEditUserChanged = false;

    bool isInitFinished = false;
};

#endif // MAINWINDOW_H
