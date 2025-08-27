#include "lmdef.h"
#include "loginwidget.h"


#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QRegularExpressionValidator>

using LM::LoginWidget;

LoginWidget::LoginWidget(QWidget* parent) : QWidget(parent) {
    setupUI();
    setupConnections();
    loadQRCode();
    // 初始选中手机登录
    switchToPhoneLogin();
}

void LoginWidget::setupUI() {
    // ====== 主布局 ======
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(40, 30, 20, 10);

    // ====== 标题 ======
    QLabel *titleLabel = new QLabel("LearningMachine", this);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #3498db;");
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    // ====== 登录方式切换按钮 ======
    QHBoxLayout *switchLayout = new QHBoxLayout();
    switchLayout->setSpacing(0);

    phoneLoginBtn = createSwitchButton("手机登录", true);
    qrcodeLoginBtn = createSwitchButton("扫码登录", false);

    switchLayout->addWidget(phoneLoginBtn);
    switchLayout->addWidget(qrcodeLoginBtn);

    mainLayout->addLayout(switchLayout);

    QTimer::singleShot(0, [this]() {
        phoneBtnPos = phoneLoginBtn->pos();
        qrcodeBtnPos = qrcodeLoginBtn->pos();
    });

    // ====== 登录内容区域 ======
    stackedWidget = new QStackedWidget(this);

    // 手机登录表单
    QWidget *phoneTab = createPhoneLoginTab();
    stackedWidget->addWidget(phoneTab);

    // 二维码登录
    QWidget *qrcodeTab = createQRCodeTab();
    stackedWidget->addWidget(qrcodeTab);

    mainLayout->addWidget(stackedWidget);

    // ====== 注册跳转 ======
    QHBoxLayout *footerLayout = new QHBoxLayout();
    QLabel *registerPrompt = new QLabel("没有账号？", this);
    registerBtn = new QPushButton("立即注册", this);
    registerBtn->setStyleSheet("color: #3498db; border: none; background: transparent;");
    footerLayout->addStretch();
    footerLayout->addWidget(registerPrompt);
    footerLayout->addWidget(registerBtn);
    mainLayout->addLayout(footerLayout);

    // ====== 白色简洁风格 ======
    setStyleSheet(R"(
        QWidget {
            background-color: #ffffff;
            font-family: 'Segoe UI', 'Microsoft YaHei';
        }
        QLineEdit {
            padding: 12px;
            border: 1px solid #e0e0e0;
            border-radius: 6px;
            font-size: 14px;
            background: #fafafa;
        }
        QLineEdit:focus {
            border-color: #3498db;
            background: #ffffff;
        }
        QPushButton {
            padding: 12px 24px;
            background-color: #3498db;
            color: white;
            border-radius: 6px;
            font-weight: 500;
            border: none;
            min-width: 120px;
        }
        QPushButton:hover {
            background-color: #2980b9;
        }
        QPushButton:disabled {
            background-color: #bdc3c7;
        }
        QLabel#loadingLabel {
            qproperty-alignment: AlignCenter;
            color: #7f8c8d;
            font-size: 14px;
        }
    )");

    // 初始尺寸
    resize(480, 420);
}

QPushButton* LoginWidget::createSwitchButton(const QString& text, bool active) {
    QPushButton *btn = new QPushButton(text, this);
    btn->setCheckable(true);
    btn->setChecked(active);
    btn->setStyleSheet(R"(
        QPushButton {
            padding: 12px 24px;
            background: transparent;
            color: #7f8c8d;
            font-weight: 500;
            border: none;
            border-radius: 6px;
            transition: all 0.3s ease;
        }
        QPushButton:checked {
            color: #3498db;
            background-color: #e1f0fa;
            transform: scale(1.05);
        }
        QPushButton:hover {
            background-color: #f8f9fa;
        }
    )");
    return btn;
}

QWidget* LoginWidget::createPhoneLoginTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setSpacing(20);
    layout->setContentsMargins(20, 30, 10, 5);

    // 手机号输入
    phoneEdit = new QLineEdit(tab);
    phoneEdit->setPlaceholderText("请输入手机号");
    phoneEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("1[0-9]{10}"), this));
    layout->addWidget(new QLabel("手机号:", tab));
    layout->addWidget(phoneEdit);

    // 密码输入
    passwordEdit = new QLineEdit(tab);
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(new QLabel("密码:", tab));
    layout->addWidget(passwordEdit);

    // 登录按钮
    loginBtn = new QPushButton("登录", tab);
    layout->addWidget(loginBtn, 0, Qt::AlignTop | Qt::AlignHCenter);
    // layout->addStretch();

    return tab;
}

QWidget* LoginWidget::createQRCodeTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(20, 30, 10, 10);
    layout->setSpacing(25);

    // 二维码显示区域
    QWidget *qrcodeContainer = new QWidget(tab);
    QVBoxLayout *qrcodeLayout = new QVBoxLayout(qrcodeContainer);
    qrcodeLayout->setContentsMargins(0, 0, 0, 0);

    qrcodeLabel = new QLabel(qrcodeContainer);
    qrcodeLabel->setAlignment(Qt::AlignCenter);
    qrcodeLabel->setMinimumSize(220, 220);
    qrcodeLabel->setStyleSheet("background: #ffffff; border: 1px solid #e0e0e0; border-radius: 8px;");
    qrcodeLayout->addWidget(qrcodeLabel);

    loadingLabel = new QLabel("加载中...", qrcodeContainer);
    loadingLabel->setObjectName("loadingLabel");
    qrcodeLayout->addWidget(loadingLabel);

    layout->addWidget(qrcodeContainer, 0, Qt::AlignHCenter);

    // 刷新按钮
    QPushButton *refreshBtn = new QPushButton("刷新二维码", tab);
    layout->addWidget(refreshBtn, 0, Qt::AlignHCenter);

    return tab;
}

void LoginWidget::setupConnections() {
    // 登录方式切换
    connect(phoneLoginBtn, &QPushButton::clicked, this, &LoginWidget::switchToPhoneLogin);
    connect(qrcodeLoginBtn, &QPushButton::clicked, this, &LoginWidget::switchToQRLogin);

    // 手机登录
    connect(loginBtn, &QPushButton::clicked, this, &LoginWidget::onPhoneLogin);

    // 二维码刷新
    connect(findChild<QPushButton*>("刷新二维码", Qt::FindDirectChildrenOnly),
            &QPushButton::clicked, this, &LoginWidget::loadQRCode);

    // 注册跳转
    connect(registerBtn, &QPushButton::clicked, this, &LoginWidget::switchToRegister);
}

void LoginWidget::switchToPhoneLogin() {
    if (stackedWidget->currentIndex() == 0) return;

    // 创建动画组
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(this);

    // 页面切换动画
    createPageSwitchAnimation(animationGroup, 0);

    // 按钮位移动画
    createButtonSwitchAnimation(animationGroup, true);

    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginWidget::switchToQRLogin() {
    if (stackedWidget->currentIndex() == 1) return;

    // 创建动画组
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(this);

    // 页面切换动画
    createPageSwitchAnimation(animationGroup, 1);

    // 按钮位移动画
    createButtonSwitchAnimation(animationGroup, false);

    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

// 添加新的动画函数
void LoginWidget::createButtonSwitchAnimation(QParallelAnimationGroup* group, bool toPhone) {
    // 按钮状态变化
    QPushButton *activeBtn = toPhone ? phoneLoginBtn : qrcodeLoginBtn;
    QPushButton *inactiveBtn = toPhone ? qrcodeLoginBtn : phoneLoginBtn;

    activeBtn->setChecked(true);
    inactiveBtn->setChecked(false);

    // 按钮位移动画 - 创建弹跳效果
    int offset = 10;  // 按钮上移距离

    // 激活按钮动画
    QPropertyAnimation *activeMove = new QPropertyAnimation(activeBtn, "pos");
    activeMove->setDuration(350);
    activeMove->setStartValue(activeBtn->pos());
    activeMove->setKeyValueAt(0.3, activeBtn->pos() - QPoint(0, offset));
    activeMove->setEndValue(toPhone ? phoneBtnPos : qrcodeBtnPos);
    activeMove->setEasingCurve(QEasingCurve::OutBack);

    // 非激活按钮动画
    QPropertyAnimation *inactiveMove = new QPropertyAnimation(inactiveBtn, "pos");
    inactiveMove->setDuration(350);
    inactiveMove->setStartValue(inactiveBtn->pos());
    inactiveMove->setEndValue(toPhone ? qrcodeBtnPos : phoneBtnPos);
    inactiveMove->setEasingCurve(QEasingCurve::OutQuad);

    group->addAnimation(activeMove);
    group->addAnimation(inactiveMove);

    // 按钮缩放动画
    QPropertyAnimation *activeScale = new QPropertyAnimation(activeBtn, "geometry");
    activeScale->setDuration(350);
    activeScale->setStartValue(activeBtn->geometry());
    activeScale->setKeyValueAt(0.5, activeBtn->geometry().adjusted(-5, -5, 5, 5));
    activeScale->setEndValue(activeBtn->geometry());
    activeScale->setEasingCurve(QEasingCurve::OutBack);

    group->addAnimation(activeScale);
}

void LoginWidget::createPageSwitchAnimation(QParallelAnimationGroup* group, int newIndex) {
    // 当前页面淡出
    QWidget *currentWidget = stackedWidget->currentWidget();
    QGraphicsOpacityEffect *fadeOutEffect = new QGraphicsOpacityEffect(currentWidget);
    currentWidget->setGraphicsEffect(fadeOutEffect);

    QPropertyAnimation *fadeOut = new QPropertyAnimation(fadeOutEffect, "opacity");
    fadeOut->setDuration(300);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->setEasingCurve(QEasingCurve::OutQuad);
    group->addAnimation(fadeOut);

    // 新页面淡入
    QWidget *newWidget = stackedWidget->widget(newIndex);
    QGraphicsOpacityEffect *fadeInEffect = new QGraphicsOpacityEffect(newWidget);
    newWidget->setGraphicsEffect(fadeInEffect);
    fadeInEffect->setOpacity(0.0);

    QPropertyAnimation *fadeIn = new QPropertyAnimation(fadeInEffect, "opacity");
    fadeIn->setDuration(300);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InQuad);
    group->addAnimation(fadeIn);

    // 动画完成后切换页面
    connect(group, &QParallelAnimationGroup::finished, [=]() {
        stackedWidget->setCurrentIndex(newIndex);
        currentWidget->setGraphicsEffect(nullptr);
        newWidget->setGraphicsEffect(nullptr);
    });
}

void LoginWidget::createIndicatorAnimation(QParallelAnimationGroup* group, QWidget* targetBtn) {
    // 指示器移动动画
    int targetX = targetBtn->x() + (targetBtn->width() - indicator->width()) / 2;
    QPropertyAnimation *moveAnim = new QPropertyAnimation(indicator, "geometry");
    moveAnim->setDuration(300);
    moveAnim->setStartValue(indicator->geometry());
    moveAnim->setEndValue(QRect(targetX, indicator->y(),
                                indicator->width(), indicator->height()));
    moveAnim->setEasingCurve(QEasingCurve::OutBack);
    group->addAnimation(moveAnim);
}

void LoginWidget::onPhoneLogin() {
    // 按钮点击动画
    QPropertyAnimation *clickAnim = new QPropertyAnimation(loginBtn, "geometry");
    clickAnim->setDuration(150);
    clickAnim->setStartValue(loginBtn->geometry());
    clickAnim->setKeyValueAt(0.5, loginBtn->geometry().adjusted(0, 5, 0, -5));
    clickAnim->setEndValue(loginBtn->geometry());
    clickAnim->start(QAbstractAnimation::DeleteWhenStopped);

    // 登录逻辑（保持不变）
    QString phone = phoneEdit->text().trimmed();
    QString password = passwordEdit->text();

    if(phone.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "手机号和密码不能为空");
        return;
    }

    manager.connectToHost(HTTPS_REQ_HEAD );
    // ...（API请求逻辑保持不变）
}

void LoginWidget::loadQRCode() {
    // 显示加载状态
    loadingLabel->setText("加载中...");
    qrcodeLabel->clear();

    // 模拟加载动画
    QPropertyAnimation *fadeAnim = new QPropertyAnimation(loadingLabel, "opacity");
    fadeAnim->setDuration(1000);
    fadeAnim->setStartValue(0.3);
    fadeAnim->setEndValue(1.0);
    fadeAnim->setLoopCount(-1);
    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);

    // 模拟API获取二维码
    QTimer::singleShot(1500, [=]() {
        fadeAnim->stop();
        loadingLabel->setText("请使用LearningMachine扫码登录");

        // 此处应替换为真实二维码生成逻辑
        QPixmap pixmap(220, 220);
        pixmap.fill(Qt::white);
        qrcodeLabel->setPixmap(pixmap);
    });
}
