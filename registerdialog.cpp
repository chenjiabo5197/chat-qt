#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    // 设置lineedit为密码模式，输入不显示密码
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);
    // 给err_tip设置属性，让qt知道err_tip有normal状态
    ui->err_tip->setProperty("state", "normal");
    // 刷新err_tip
    repolish(ui->err_tip);
    // 连接httpmgr类发送给注册模块的http请求服务返回的结果信号
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);

    initHttpHandlers();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_get_code_btn_clicked()
{
    auto email = ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match)
    {
        // 发送验证码
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_verifycode"), json_obj, ReqId::ID_GET_VARIFY_CODE, Modules::REGISTERMOD);
    }
    else
    {
        // 没匹配
        // tr函数将字符串标记为需要翻译的字符串将字符串标记为需要翻译的字符串
        // tr()函数只能在继承自QObject的类中使用，因为它依赖于Qt的元对象系统来工作
        showTip(tr("邮箱格式不正确"), false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId req_id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS)
    {
        showTip(tr("网络请求错误"), false);
        return;
    }
    // 解析json字符串，res(json字符串)转化为QByteArray(可以用字节数组反序列化为类对象)
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    // 解析失败
    if(jsonDoc.isNull())
    {
        showTip(tr("json解析失败"), false);
        return;
    }
    // 解析错误，不能解析为一个对象
    if(!jsonDoc.isObject())
    {
        showTip(tr("json解析失败"), false);
        return;
    }
    // 通过http处理函数的map获取req_id对应的处理函数，传入的参数为json对象
    _handlers[req_id](jsonDoc.object());
    return;
}

void RegisterDialog::initHttpHandlers()
{
    // 注册获取验证码回调的逻辑
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS)
        {
            showTip(tr("参数错误"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已经发送到邮箱，请注意查收"), true);
        qDebug() << "email=" << email;
    });

    // 注册用户回包逻辑
    _handlers.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"), true);
        qDebug()<< "email is " << email ;
    });
}

void RegisterDialog::showTip(QString str, bool b_ok)
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


void RegisterDialog::on_sure_btn_clicked()
{
    if(ui->user_edit->text() == ""){
        showTip(tr("用户名不能为空"), false);
        return;
    }
    if(ui->email_edit->text() == ""){
        showTip(tr("邮箱不能为空"), false);
        return;
    }
    if(ui->pass_edit->text() == ""){
        showTip(tr("密码不能为空"), false);
        return;
    }
    if(ui->confirm_edit->text() == ""){
        showTip(tr("确认密码不能为空"), false);
        return;
    }
    if(ui->confirm_edit->text() != ui->pass_edit->text()){
        showTip(tr("密码和确认密码不匹配"), false);
        return;
    }
    if(ui->verify_edit->text() == ""){
        showTip(tr("验证码不能为空"), false);
        return;
    }

    // 发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = ui->pass_edit->text();
    json_obj["confirm"] = ui->confirm_edit->text();
    json_obj["varifycode"] = ui->verify_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"), json_obj, ReqId::ID_REG_USER, Modules::REGISTERMOD);
}

