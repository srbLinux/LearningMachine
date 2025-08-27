#include <QMainWindow>
#include <QApplication>

#include "loginwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LM::LoginWidget* loginWidget = new LM::LoginWidget;
    loginWidget->setWindowTitle("LearningMachine登录");
    loginWidget->resize(700, 350); loginWidget->show();
    QMainWindow window;
    QObject::connect(loginWidget, &LM::LoginWidget::loginSuccess, [&] {
        window.show();
    });
    return app.exec();
}
