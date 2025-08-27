#ifndef USERINFO_H
#define USERINFO_H

#include <QString>

class UserInfo
{
public:
    int gender;
    QString name, passwd, icon;
private:
    static UserInfo userInfo;
    explicit UserInfo();
    UserInfo(const UserInfo&) = delete;
};

#endif // USERINFO_H
