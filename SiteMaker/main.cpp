#include <fstream>
#include <string>
#include <iostream>

using namespace std;

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

class markdownInterpreter {
private:
    bool unordered_list_open = false;
    bool ordered_list_open = false;
    bool unordered_sublist_open = false;
    bool ordered_sublist_open = false;
    bool listitem_open = false;
    bool paragraph_open = false;
    bool styleopen[2] = { false, false };
    bool openBlackquote = false;
    bool codeblockopen = false;
    bool ignore = false;
    bool footnoteOpen = false;

    string newline;

public:

    void checkCloseList(int type, bool lionly = false, bool ft = false) {
        if (openBlackquote == true) {
            newline = "</blockquote>\n" + newline;
            openBlackquote = false;
        }
        if (listitem_open == true) {
            newline = "</li>\n" + newline;
            listitem_open = false;
        }

        if (footnoteOpen == true) {
            newline = "</ol>\n" + newline;
            footnoteOpen = false;
        }

        if (unordered_sublist_open == true && type != 2 && !lionly) {
            checkCloseList(0, true);
            newline = "</ul>\n" + newline;
            unordered_sublist_open = false;
        }
        if (ordered_sublist_open == true && type != 2 && !lionly) {
            checkCloseList(0, true);
            newline = "</ol>\n" + newline;
            ordered_sublist_open = false;
        }
        if (unordered_list_open == true && type != 1 && !lionly) {
            newline = "</li></ul>\n" + newline;
            unordered_list_open = false;
            listitem_open = false;
        }
        if (ordered_list_open == true && type != 1 && !lionly) {
            listitem_open = false;
            newline = "</li></ol>\n" + newline;
            ordered_list_open = false;
        }
    }

    string finalCloseList(string finaltext, int type = 3) {
        if (paragraph_open) {
            paragraph_open = false;
            finaltext += "</p>";
        }
        if (unordered_sublist_open == true && type != 2) {
            checkCloseList(0, true);
            finaltext += "</ul>\n";
            unordered_sublist_open = false;
        }
        if (ordered_sublist_open == true && type != 2) {
            checkCloseList(0, true);
            finaltext += "</ol>\n";
            ordered_sublist_open = false;
        }
        if (unordered_list_open == true && type != 1) {
            finaltext += "</ul>\n";
            unordered_list_open = false;
        }
        if (ordered_list_open == true && type != 1) {
            finaltext += "</ol>\n";
            ordered_list_open = false;
        }
        if (footnoteOpen) {
            finaltext += "</ol>\n";
            footnoteOpen = false;
        }
        return finaltext;
    }

    string TextStyle(string line) {
        bool isActive = false;
        string templine = "";
        int stylenum = 0;
        int counter = 0;

        for (char c : line) {
            if (c == '\\') {
                ignore = true;
                counter = 1;
            }

            if (c == '*') {
                if (!ignore) {
                    stylenum++;
                }
                else {
                    if (counter <= 0) {
                        ignore = false;
                    }
                    else {
                        counter--;
                    }
                    templine += c;
                }
            }
            else {
                if (stylenum > 0) {
                    if (stylenum == 1) {
                        if (!styleopen[0]) {
                            templine += "<em>";
                            styleopen[0] = true;
                        }
                        else {
                            templine += "</em>";
                            styleopen[0] = false;
                        }
                    }
                    else if (stylenum == 2) {
                        if (!styleopen[1]) {
                            templine += "<strong>";
                            styleopen[1] = true;
                        }
                        else {
                            templine += "</strong>";
                            styleopen[1] = false;
                        }
                    }
                    else if (stylenum == 3) {
                        if (!styleopen[1] && !styleopen[0]) {
                            templine += "<strong><em>";
                            styleopen[0] = true;
                            styleopen[1] = true;
                        }
                        else if (styleopen[0] && styleopen[1]) {
                            templine += "</em></strong>";
                            styleopen[0] = false;
                            styleopen[1] = false;
                        }
                    }
                    stylenum = 0;
                }

                if (c != '\\') {
                    templine += c;
                }
            }
        }

        return templine;
    }

    void processHeader(string line) {
        int headernum = 0;
        for (char c : line) {
            if (c == '#') { headernum++; }
        }
        if (headernum > 6) {
            cout << "Warning - MarkupInterpreter: Too many # Level 6 is maximum allowed! Assuming you meant Level 6;";
            headernum = 6;
        }

        line.erase(0, headernum + 1);
        newline = "<h" + to_string(headernum) + ">" + line + "</h" + to_string(headernum) + ">";
    }

    void processRemainingMarkdown(string line) {
        newline = "";
        string linkText = "";
        string link = "";
        string title = "";
        string code = "";
        string footnote = "";
        bool isImage = false;
        bool isFootnote = false;
        bool wasTriggered = false;
        bool isCode = false;
        bool isFootnoteDeclaration = false;
        bool isTask = false;
        int i = 0;
        bool linkc[3] = { false, false, false };

        for (char c : line) {
            i++;
            switch (c) {
            case '!':
                wasTriggered = true;
                isImage = true;
                break;
            case '[':
                wasTriggered = true;
                linkc[0] = true;
                break;
            case ']':
                linkc[0] = false;
                if (linkText == "x" || linkText == " " || linkText == "X") {
                    isTask = true;
                    if (linkText = " ") {
                        newline += "<input type=\"checkbox\"> ";
                    }
                    else if (linkText == "X" || linkText == "x") {
                        newline += "<input type=\"checkbox\" checked>";
                    }
                }
                if (isFootnote && line[1] != '^') {
                    newline += "<sup id=\"fnref" + linkText + "\"><a href=\"#fn" + linkText + "\">" + linkText + "</a></sup>";
                }
                break;
            case '(':
                linkc[1] = true;
                break;
            case '^':
                if (linkc[0]) {
                    isFootnote = true;
                }
                break;
            case ')':
                linkc[1] = false;
                if (link.back() == ' ') {
                    link.pop_back();
                }
                if (isImage && wasTriggered && !isFootnote) {
                    newline += "<img src=\"" + link + "\" alt=\"" + linkText + "\" title=\"" + title + "\">";
                }
                else if (wasTriggered && !isFootnote) {
                    newline += "<a href=\"" + link + "\" title=\"" + title + "\">" + linkText + "</a>";
                }
                isImage = false;
                wasTriggered = false;
                break;
            case '"':
                linkc[2] = !linkc[2];
                break;
            case ':':
                if (isFootnote) {
                    isFootnoteDeclaration = true;
                }
                break;
            case '`':
                if (!codeblockopen) {
                    isCode = !isCode;
                    wasTriggered = true;
                    if (!isCode && wasTriggered) {
                        newline += "<code>" + code + "</code>";
                        code.clear();
                    }
                }
                break;
            default:
                if (linkc[0] && !isFootnoteDeclaration) {
                    linkText += c;
                }
                else if (linkc[1] && !linkc[2] && !isFootnoteDeclaration) {
                    link += c;
                }
                else if (linkc[2]) {
                    title += c;
                }
                else if (isCode) {
                    code += c;
                }
                else if (isFootnoteDeclaration) {
                    footnote += c;
                }
                else {
                    newline += c;
                }
                break;
            }
        }

        if (isFootnoteDeclaration) {
            if (!footnoteOpen) {
                newline += "<ol id=\"footnotes\">\n";
                footnoteOpen = true;
            }
            newline += "<li id=\"fn" + linkText + "\">" + footnote + " <a href=\"#fnref" + linkText + "\">↩</a></li>\n";
        }
    }

    void processParagraph(string line) {
        if (line.empty()) {
            if (paragraph_open) {
                paragraph_open = false;
                newline += "</p>";
            }
        }
        else {
            if (!paragraph_open) {
                paragraph_open = true;
                newline = "<p>";
            }
            newline += line;
        }
    }

    void processUnList(string line) {
        if (!unordered_list_open) {
            newline = "<ul>\n";
            unordered_list_open = true;
        }
        else {
            checkCloseList(0, true);
            checkCloseList(1);
        }
        line.erase(0, 2);
        newline += "<li>" + line;
        listitem_open = true;
    }

    void processUnSublist(string line) {
        if (!unordered_sublist_open) {
            newline += "<ul>\n";
            unordered_sublist_open = true;
        }
        line.erase(0, 4);
        newline += "<li>" + line + "</li>";
    }

    void processOList(string line) {
        checkCloseList(0, true);
        if (!ordered_list_open) {
            newline = "<ol>\n";
            ordered_list_open = true;
        }
        else {
            checkCloseList(0, true);
            checkCloseList(1);
        }
        line.erase(0, 2);
        newline += "<li>" + line;
        listitem_open = true;
    }

    void processOSublist(string line) {
        if (!ordered_sublist_open) {
            newline += "<ol>\n";
            ordered_sublist_open = true;
        }
        line.erase(0, 5);
        newline += "<li>" + line + "</li>";
    }

    void processBlockquote(string line) {
        int BCount = 0;
        for (char c : line) {
            if (c == '>') BCount++;
            else break;
        }
        line.erase(0, BCount + 1);
        if (BCount == 1) {
            if (openBlackquote) {
                checkCloseList(0, true);
            }
            openBlackquote = true;
            newline = "<blockquote>" + line;
        }
        else if (BCount == 2) {
            newline = "<blockquote>" + line + "</blockquote>";
        }
    }

    void processHorizontalRule() {
        newline = "<hr>";
    }

    void processCodeBlock(string line) {
        codeblockopen = !codeblockopen;
        if (codeblockopen) {
            newline = "<pre><code>";
        }
        else {
            newline = "</code></pre>";
        }
    }

    string processline(string line) {
        newline = "";
        if (line.empty()) {
            processParagraph(line);
            return newline + "\n";
        }

        char infocus = line[0];
        bool processed = false;

        if (isdigit(infocus) && line[1] == '.') {
            checkCloseList(0, true)
            processOList(line);
            processed = true;
        }
        else {
            switch (infocus) {
            case '#':
                processHeader(line);
                checkCloseList(3);
                processed = true;
                break;
            case '*':
            case '+':
            case '-':
                if (line.size() > 1 && line[1] == ' ') {
                    processUnList(line);
                    processed = true;
                }
                else if (line.substr(0, 3) == "---" || line.substr(0, 3) == "***") {
                    processHorizontalRule();
                    processed = true;
                }
                break;
            case ' ':
                if (line.size() > 3 && (line[2] == '-' || line[2] == '+' || line[2] == '*')) {
                    processUnSublist(line);
                    processed = true;
                }
                else if (line.size() > 4 && isdigit(line[3]) && line[4] == '.') {
                    processOSublist(line);
                    processed = true;
                }
                break;
            case '[':
                processRemainingMarkdown(line);
                processed = true;
                break;
            case '>':
                processBlockquote(line);
                processed = true;
                break;
            case '~':
            case '`':
                if (line.size() > 1 && (line[1] == '`' || line[1] == '~')) {
                    processCodeBlock(line);
                    processed = true;
                }
                break;
            case '_':
                if (line.substr(0, 3) == "___") {
                    processHorizontalRule();
                    processed = true;
                }
                break;
            default:
                break;
            }
        }

        if (!processed) {
            processParagraph(line);
            checkCloseList(3);
        }

        newline = TextStyle(newline);
        processRemainingMarkdown(newline);
        return newline + "\n";
    }
};

int main() {
    string filepath = "C:/test/test.txt";
    string line;
    fstream mfile(filepath);
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