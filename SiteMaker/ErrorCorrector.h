#include <string>
#include <iostream>


class ErrorCorrector {
private:
    string code = "";
public:
    void removeExtraLi(std::string& str) {
        std::string patternOl = "</ol>\n</li>";
        std::string patternUl = "</li></ul>\n</li>";

        size_t posOl = 0;
        while ((posOl = str.find(patternOl, posOl)) != std::string::npos) {
            str.erase(posOl + 6, 5);
            posOl += 6;
        }

        size_t posUl = 0;
        while ((posUl = str.find(patternUl, posUl)) != std::string::npos) {
            str.erase(posUl + 11, 5);
            posUl += 11;
        }
    }
    void submitCode(string programcode) {
        code = programcode;
    }
    string checkCode() {
        this->removeExtraLi(code);
        return code;
    }
};