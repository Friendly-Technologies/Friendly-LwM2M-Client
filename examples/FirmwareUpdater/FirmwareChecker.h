#ifndef FIRMWARE_CHECKER_H_
#define FIRMWARE_CHECKER_H_

#include <iostream>
#include <fstream>

using namespace std;

class FirmwareChecker {

public:

    static FwUpdRes getFwDownloadRes() {
        if (!checkPackageType()) return R_UNSUPPORTED_PKG_TYPE;
        if (!checkConnection()) return R_CONN_LOST;
        if (!checkIntegrity()) return R_INTEGRITY_CHECK_FAIL;
        
        if (getFileSize() > max_file_size_b) {
            string res = readMetadata(7);
            if (res == "/memory_location:=flash") return R_NOT_ENOUGH_FLASH;
            else if (res == "/memory_location:=RAM") return R_OUT_OF_RAM;
        }
        return R_INITIAL;
    }

    static string getPkgName() {
        cout << "FirmwareChecker: getPkgName()" << endl;
        return readMetadata(1);
    }

    static string getPkgVersion() {
        cout << "FirmwareChecker: getPkgVersion()" << endl;
        return readMetadata(2);
    }

    static bool getUpdateResult() {
        string res = readMetadata(6);
        cout << "FirmwareChecker: update res: " << res << endl;
        return res == "/pass_updating:=true";
    }

private:

    static bool checkPackageType() {
        string res = readMetadata(3);
        cout << "FirmwareChecker: package type: " << res << endl;
        return res == "/package_type:=1";
    }


    static bool checkConnection() {
        string res = readMetadata(4);
        cout << "FirmwareChecker: connection: " << res << endl;
        return res == "/pass_connection:=true";

    }

    static bool checkIntegrity() {
        string res = readMetadata(5);
        cout << "FirmwareChecker: integrity: " << res << endl;
        return res == "/pass_integrity:=true";
    }


    static string readMetadata(uint8_t line_num) {
        ifstream is;
        string str;
    
        cout << "FirmwareChecker: readMetadata()" << endl;

        is.open(file_name, ios::binary);
        if (is.is_open()) {
            // if file can't be opened it means it's not exists. Will return "default"
            for (uint8_t i = 0; i < line_num; i++) getline(is, str);
        }
        is.close();

        return str;
    }

    static int getFileSize() {
        streampos fsize = 0;
        ifstream file(file_name, ios::binary);

        cout << "FirmwareChecker: getFileSize()" << endl;

        fsize = file.tellg();
        file.seekg(0, ios::end );
        fsize = file.tellg() - fsize;
        file.close();

        return fsize;
    }

public:
    inline static const string file_name = "test_fw.fw";
    inline static const int max_file_size_b = 2000000;
};


#endif // FIRMWARE_CHECKER_H_
