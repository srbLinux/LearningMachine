#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QStyle>
#include <QMovie>
#include <QLabel>
#include <QTimer>
#include <QWidget>
#include <QLineEdit>
#include <QTabWidget>
#include <QPushButton>
#include <QJsonObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QStackedWidget>
#include <QNetworkAccessManager>

class QParallelAnimationGroup;

namespace LM
{

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);
private:
    void setupUI();
    void setupConnections();
    QWidget* createQRCodeTab();
    QWidget* createPhoneLoginTab();

    QPushButton* createSwitchButton(const QString&, bool);
    void createPageSwitchAnimation(QParallelAnimationGroup *, int);
    void createButtonSwitchAnimation(QParallelAnimationGroup *, bool);
    void createIndicatorAnimation(QParallelAnimationGroup *, QWidget *);
private slots:
    void loadQRCode();
    void onPhoneLogin();
    void switchToQRLogin();
    void switchToPhoneLogin();
signals:
    void loginSuccess();
    void switchToRegister();
private:
    QWidget *indicator;
    QLabel *qrcodeLabel;
    QLabel *loadingLabel;
    QLineEdit *phoneEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginBtn;
    QPushButton *registerBtn;
    QPushButton *phoneLoginBtn;
    QPushButton *qrcodeLoginBtn;
    QStackedWidget *stackedWidget;
    QNetworkAccessManager manager;

    QPoint phoneBtnPos;     // 手机按钮原始位置
    QPoint qrcodeBtnPos;    // 二维码按钮原始位置
};

}

#endif // LOGINWIDGET_H
