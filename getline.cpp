#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "正しい引数を指定してください。" << std::endl;
        std::cout << "使用法: ./program <検索文字列> <入力ファイル名> <出力ファイル名>" << std::endl;
        return 1;
    }

    std::string searchWord = argv[1];  // 検索する文字列
    std::string fileName = argv[2];   // 入力ファイル名
    std::string outputFileName = argv[3];  // 出力ファイル名

    std::ifstream file(fileName);
    std::ofstream output(outputFileName);

    if (!file) {
        std::cout << "ファイルを開くことができませんでした。" << std::endl;
        return 1;
    }

    std::string line;
    int lineCount = 0;

    while (std::getline(file, line)) {
        lineCount++;

        // "ACCL sampling rate" が含まれている行はスキップ
        if (line.find("ACCL sampling rate") != std::string::npos) {
            continue;
        }

        // 行に検索する文字列が含まれている場合、その行から数値部分を抽出する
        if (line.find(searchWord) != std::string::npos) {
            std::size_t startPos = line.find("ACCL") + 5;
            std::size_t endPos = line.find_last_of(",");
            std::string numbers = line.substr(startPos, endPos - startPos);

            std::string replacedString = "m/s\xB2,";
            size_t pos;
            while ((pos = numbers.find(replacedString)) != std::string::npos) {
                numbers.replace(pos, replacedString.length(), ",");
            }

            // 最後の数値の後ろの単位を削除
            replacedString = "m/s\xB2";
            while ((pos = numbers.find(replacedString)) != std::string::npos) {
                numbers.replace(pos, replacedString.length(), "");
            }

//            std::cout << numbers << std::endl;
            output << numbers << std::endl;  // 数値をCSV形式で出力
        }
        // 例えば、100行ごとに処理中であることを表示
        if (lineCount % 1000 == 0) {
            std::cout << lineCount << "lines processed." << std::endl;
        }
    }

    file.close();
    output.close();

    return 0;
}
