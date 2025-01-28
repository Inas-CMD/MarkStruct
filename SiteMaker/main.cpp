#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "TableConstructor.h"
#include "ErrorCorrector.h"
#include "MarkdownInterpreter.h"

using namespace std;


int main() {
    string filepath = "C:/test/test.txt";
    string line;
    ifstream mfile(filepath);
    markdownInterpreter i;
    ErrorCorrector ecc;
    string tempfile = "";

    if (!mfile.is_open()) {
        cout << "Error: Could not open file " << filepath << endl;
        return 1;
    }

    while (getline(mfile, line)) {
        tempfile += i.processline(line);
    }
    tempfile = i.finalCloseList(tempfile);
    ecc.submitCode(tempfile);
    tempfile = ecc.checkCode();
    cout << tempfile;
    system("pause");
    return 0;
}