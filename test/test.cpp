#include <iostream>
#include <string>

#include "../SqliteConnection.h"

void print(const std::string& info) {
    std::cout << "-------------" << info << "-------------" << std::endl;
}

int main() {
    db::SqliteConnection db;
    db::Setting opt;
    opt.dbName = "test.db";
    db::ErrorInfo error;
    if (!db.load(error, opt)) {
        std::cout << "load error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("加载不存在的库");
    opt.dbName = "/home/joersion/work/test";
    if (!db.load(error, opt)) {
        std::cout << "load error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    opt.dbName = "/home/joersion/work/test/test.db";
    if (!db.load(error, opt)) {
        std::cout << "load error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("数据插入");
    if (!db.execute(error, "insert into users (name,email,age,created_at) values(?,?,?,CURRENT_TIMESTAMP)", "wjx", "123456", "1")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("查看插入是否成功");
    if (!db.execute(
            error,
            [](const db::Result& result) {
                for (int i = 0; i < result.rowCount(); ++i) {
                    std::cout << result.data(i, "id") << "|" << result.data(i, "name") << "|" << result.data(i, "email") << "|"
                              << result.data(i, "created_at") << std::endl;
                }
            },
            "select id,name,email,age,created_at from users")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("数据更新");
    if (!db.execute(error, "update users set name=? where id>?", "joersion", "4")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("查看更新是否成功");
    if (!db.execute(
            error,
            [](const db::Result& result) {
                for (int i = 0; i < result.rowCount(); ++i) {
                    std::cout << result.data(i, "id") << "|" << result.data(i, "name") << "|" << result.data(i, "email") << "|"
                              << result.data(i, "created_at") << std::endl;
                }
            },
            "select id,name,email,age,created_at from users")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("数据删除");
    if (!db.execute(error, "delete from users where id>4")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    print("查看删除是否成功");
    if (!db.execute(
            error,
            [](const db::Result& result) {
                for (int i = 0; i < result.rowCount(); ++i) {
                    std::cout << result.data(i, "id") << "|" << result.data(i, "name") << "|" << result.data(i, "email") << "|"
                              << result.data(i, "created_at") << std::endl;
                }
            },
            "select id,name,email,age,created_at from users")) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    return 0;
}