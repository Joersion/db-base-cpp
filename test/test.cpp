#include <iostream>
#include <string>

#include "../SqliteConnection.h"

int main() {
    db::SqliteConnection db;
    db::Setting opt;
    opt.dbName = "test.db";
    db::ErrorInfo error;
    if (!db.load(error, opt)) {
        std::cout << "load error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    opt.dbName = "/home/joersion/work/test";
    if (!db.load(error, opt)) {
        std::cout << "load error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    opt.dbName = "/home/joersion/work/test/test.db";
    if (!db.load(error, opt)) {
        std::cout << "load error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }
    if (!db.execute(error, "select id,name,email,age,created_at from users;", [](const db::Result& result) {
            for (int i = 0; i < result.rowCount(); ++i) {
                std::cout << result.data(i, "id") << "|" << result.data(i, "name") << "|" << result.data(i, "email") << std::endl;
            }
        })) {
        std::cout << "execute error,code:" << error.code << ",massage:" << error.massage << std::endl;
    }

    return 0;
}