#include "logindialog.h"
#include "ui_logindialog.h"
#include <QPainter>
#include <QPainterPath>
#include "httpmgr.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    connect(ui->register_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);

    ui->forget_label->SetState("normal", "hover", "", "selected", "selected_hover", "");

    connect(ui->forget_label, &ClickedLable::clicked, this, &LoginDialog::slot_forget_pwd);
    initHead();
    ui->err_tip->clear();
    initHttoHandlers();
    // 连接httpmgr类发送给登录模块的http请求服务返回的结果信号
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_login_mod_finish, this, &LoginDialog::slot_login_mod_finish);
}

LoginDialog::~LoginDialog()
{
    delete ui;
    qDebug() << "~LoginDialog";
}

void LoginDialog::initHttoHandlers()
{
    // 注册获取验证码回调的逻辑
    _handlers.insert(ReqId::ID_CHAT_LOGIN, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS)
        {
            showTip(tr("参数错误"), false);
            enableBtn(true);
            return;
        }

        auto email = jsonObj["email"].toString();

        // 发送信号通知tcpMgr发送长链接
        ServerInfo si;
        si.Uid = jsonObj["uid"].toInt();
        si.Host = jsonObj["host"].toString();
        si.Port = jsonObj["port"].toString();
        si.Token = jsonObj["token"].toString();

        _uid = si.Uid;
        _token = si.Token;

        qDebug() << "email=" << email << "uid=" << si.Uid << "host=" << si.Host << "port=" << si.Port << "token=" << si.Token;
        emit sig_connect_tcp(si);
    });
}

void LoginDialog::initHead()
{
    // 加载图片
    QPixmap originalPixmap(":/res/head_1.jpg");
    // 设置图片自动缩放
    qDebug() << originalPixmap.size() << ui->head_label->size();
    // scaled用于将QPixmap对象缩放到指定的尺寸
    // Qt::KeepAspectRatio：表示在缩放时保持图片的原始纵横比
    // Qt::SmoothTransformation：表示在缩放时使用平滑变换算法，以获得更高质量的缩放效果
    originalPixmap = originalPixmap.scaled(ui->head_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 创建一个和原始图片大小相同的QPixmap，用于绘制圆角图片
    QPixmap roundedPixmap(originalPixmap.size());
    // 用透明色填充
    roundedPixmap.fill(Qt::transparent);
    // QPainter 类是用于在 QWidget、QPixmap、QImage 或 QPrinter 等绘制设备上进行绘制的工具。创建一个 QPainter 对象，并将其与要绘制的 QPixmap 关联起来
    QPainter painter(&roundedPixmap);
    // 设置抗锯齿，使圆角更平滑
    painter.setRenderHint(QPainter::Antialiasing);
    // 启用 SmoothPixmapTransform 渲染提示，减少或消除像素化的现象
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    // 使用QPainterPath设置圆角
    QPainterPath path;
    // 创建一个圆角矩形，并将其添加到了 QPainterPath 对象 path 中。这个圆角矩形的参数解释如下：
    // 第一个和第二个参数（0, 0）指定了圆角矩形的左上角在父坐标系中的位置。
    // 第三个和第四个参数（originalPixmap.width(), originalPixmap.height()）指定了圆角矩形的宽度和高度。
    // 第五个和第六个参数（10, 10）指定了圆角矩形每个角的半径。
    path.addRoundedRect(0, 0, originalPixmap.width(), originalPixmap.height(), 10, 10);
    // setClipPath 方法用于设置当前的裁剪路径,裁剪路径定义了一个区域，只有在这个区域内的绘制操作才会被保留
    painter.setClipPath(path);

    // 将原始图片绘制到roundedPixmap上
    // 将 originalPixmap（一个 QPixmap 对象）绘制到与 QPainter 对象关联的绘制设备的 (0, 0) 位置
    painter.drawPixmap(0, 0, originalPixmap);
    // 设置绘制好的圆角图片到QLable上
    ui->head_label->setPixmap(roundedPixmap);

}

bool LoginDialog::checkEmailVaild()
{
    auto email = ui->email_edit->text();
    if(email.isEmpty())
    {
        qDebug() << "email empty";
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool LoginDialog::checkPwdValid()
{
    auto pass = ui->pass_edit->text();
    if(pass.length() < 6 || pass.length() > 15)
    {
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 密码应该是6-15位的字符，包含字母数据和特殊字符
    QRegularExpression regex(R"(^[a-zA-Z0-9!@#$%^&*]{6,15}$)");
    bool match = regex.match(pass).hasMatch();
    if(!match)
    {
        // 输入字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

void LoginDialog::showTip(QString str, bool b_ok)
{
    if(b_ok)
    {
        ui->err_tip->setProperty("state", "normal");
    }
    else
    {
        ui->err_tip->setProperty("state", "err");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

void LoginDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void LoginDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty())
    {
        ui->err_tip->clear();
        return;
    }
    showTip(_tip_errs.first(), false);
}

bool LoginDialog::enableBtn(bool enabled)
{
    ui->login_btn->setEnabled(enabled);
    ui->register_btn->setEnabled(enabled);
    return true;
}

void LoginDialog::slot_forget_pwd()
{
    qDebug() << "slot forget pwd";
    emit switchReset();
}

void LoginDialog::on_login_btn_clicked()
{
    qDebug() << "login btn clicked";
    if(!checkEmailVaild()){
        return;
    }
    if(!checkPwdValid()){
        return;
    }
    enableBtn(false);
    // 发送http请求登录
    QJsonObject json_obj;
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = xorString(ui->pass_edit->text());
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"), json_obj, ReqId::ID_CHAT_LOGIN, Modules::LOGINMOD);
}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS)
    {
        showTip(tr("网络请求错误"), false);
        return;
    }
    // 解析json字符串，res转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    // json解析错误
    if(jsonDoc.isNull())
    {
        showTip(tr("json解析错误"), false);
        return;
    }
    if(!jsonDoc.isObject())
    {
        showTip(tr("json解析失败"), false);
        return;
    }
    // 调用对应的逻辑
    _handlers[id](jsonDoc.object());
    return;
}

