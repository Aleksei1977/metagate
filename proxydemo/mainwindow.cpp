#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "proxy/UPnPDevices.h"
#include "proxy/ProxyServer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    proxy::UPnPDevices *sock = new proxy::UPnPDevices();
    connect(sock, &proxy::UPnPDevices::discovered, this, &MainWindow::routerDiscovered);
    //void discovered(bt::UPnPRouter* router);
    sock->discover();

    proxy::ProxyServer *server = new proxy::ProxyServer(this);
    server->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::routerDiscovered(proxy::UPnPRouter *router)
{
    routers.append(router);
    ui->listWidget->addItem(router->server());
}

void MainWindow::on_addMappingButton_clicked()
{
    for (proxy::UPnPRouter *router : routers)
        router->addPortMapping(1234, 1234, proxy::TCP);
}

void MainWindow::on_removeMappingButton_clicked()
{
    for (proxy::UPnPRouter *router : routers)
        router->deletePortMapping(1234, proxy::TCP);
}
