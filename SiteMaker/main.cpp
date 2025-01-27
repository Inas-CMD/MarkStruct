#include <fstream>
#include <string>
#include <iostream>

using namespace std;

class ErrorCorrector {
private:
    string code = "";
public:
    void removeExtraLi(string& str) {
        string patternOl = "</ol>\n</li>";
        string patternUl = "</li></ul>\n</li>";

        size_t posOl = 0;
        while ((posOl = str.find(patternOl, posOl)) != string::npos) {
            str.erase(posOl + 6, 5);
            posOl += 6;
        }

        size_t posUl = 0;
        while ((posUl = str.find(patternUl, posUl)) != string::npos) {
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
    string footnoteText;
    string footnoteLabel;

public:
    void checkCloseList(int type, bool lionly = false) {
        if (openBlackquote) {
            newline = "</blockquote>\n" + newline;
            openBlackquote = false;
        }
        if (listitem_open) {
            newline = "</li>\n" + newline;
            listitem_open = false;
        }

        if (footnoteOpen) {
            newline = "</ol>\n" + newline;
            footnoteOpen = false;
        }

        if (unordered_sublist_open && type != 2 && !lionly) {
            checkCloseList(0, true);
            newline = "</ul>\n" + newline;
            unordered_sublist_open = false;
        }
        if (ordered_sublist_open && type != 2 && !lionly) {
            checkCloseList(0, true);
            newline = "</ol>\n" + newline;
            ordered_sublist_open = false;
        }
        if (unordered_list_open && type != 1 && !lionly) {
            newline = "</li></ul>\n" + newline;
            unordered_list_open = false;
            listitem_open = false;
        }
        if (ordered_list_open && type != 1 && !lionly) {
            listitem_open = false;
            newline = "</li></ol>\n" + newline;
            ordered_list_open = false;
        }
    }

    string finalCloseList(string finaltext) {
        if (paragraph_open) {
            paragraph_open = false;
            finaltext += "</p>";
        }
        if (unordered_sublist_open) {
            finaltext += "</ul>\n";
            unordered_sublist_open = false;
        }
        if (ordered_sublist_open) {
            finaltext += "</ol>\n";
            ordered_sublist_open = false;
        }
        if (unordered_list_open) {
            finaltext += "</ul>\n";
            unordered_list_open = false;
        }
        if (ordered_list_open) {
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
        int stylenum = 0;
        string templine;
        bool escape = false;

        for (size_t i = 0; i < line.size(); i++) {
            if (escape) {
                templine += line[i];
                escape = false;
                continue;
            }
            if (line[i] == '\\') {
                escape = true;
                continue;
            }
            if (line[i] == '*') {
                stylenum++;
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
                        if (!styleopen[0] && !styleopen[1]) {
                            templine += "<strong><em>";
                            styleopen[0] = styleopen[1] = true;
                        }
                        else {
                            templine += "</em></strong>";
                            styleopen[0] = styleopen[1] = false;
                        }
                    }
                    stylenum = 0;
                }
                templine += line[i];
            }
        }
        return templine;
    }

    void processHeader(string line) {
        int headernum = 0;
        while (line[headernum] == '#') headernum++;
        line = line.substr(headernum);
        while (line[0] == ' ') line.erase(0, 1);
        newline = "<h" + to_string(headernum) + ">" + line + "</h" + to_string(headernum) + ">";
    }

    void processRemainingMarkdown(string line) {
        newline = "";
        bool isImage = false;
        bool isFootnoteRef = false;
        bool isFootnoteDecl = false;
        string linkText, link, title, codeContent;
        int bracketDepth = 0;

        for (size_t i = 0; i < line.size(); i++) {
            char c = line[i];
            if (c == '!' && i + 1 < line.size() && line[i + 1] == '[') {
                isImage = true;
                i++;
                continue;
            }

            if (c == '[') {
                if (i + 1 < line.size() && line[i + 1] == '^') {
                    isFootnoteRef = true;
                    i++;
                    linkText.clear();
                    while (i + 1 < line.size() && line[i + 1] != ']') {
                        linkText += line[++i];
                    }
                    i++;
                    if (i + 1 < line.size() && line[i + 1] == ':') {
                        isFootnoteDecl = true;
                        isFootnoteRef = false;
                        i++;
                        footnoteLabel = linkText;
                        footnoteText.clear();
                        while (i + 1 < line.size()) footnoteText += line[++i];
                        break;
                    }
                    else {
                        newline += "<sup id=\"fnref" + linkText + "\"><a href=\"#fn" + linkText + "\">" + linkText + "</a></sup>";
                    }
                }
                continue;
            }

            newline += c;
        }

        if (isFootnoteDecl) {
            if (!footnoteOpen) {
                newline += "<ol id=\"footnotes\">";
                footnoteOpen = true;
            }
            newline += "<li id=\"fn" + footnoteLabel + "\">" + footnoteText + "</li>";
        }
    }

    void processParagraph(string line) {
        if (line.empty()) {
            if (paragraph_open) {
                paragraph_open = false;
                newline = "</p>";
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
        line = line.substr(2);
        newline += "<li>" + line;
        listitem_open = true;
    }

    void processOList(string line) {
        if (!ordered_list_open) {
            newline = "<ol>\n";
            ordered_list_open = true;
        }
        line = line.substr(line.find('.') + 2);
        newline += "<li>" + line;
        listitem_open = true;
    }

    string processline(string line) {
        newline = "";
        if (line.empty()) {
            checkCloseList(3);
            return "\n";
        }

        if (line[0] == '#') {
            processHeader(line);
        }
        else if (isdigit(line[0])) {
            processOList(line);
        }
        else if (line[0] == '*' || line[0] == '+' || line[0] == '-') {
            if (line.substr(0, 3) == "***") {
                newline = "<hr>";
            }
            else {
                processUnList(line);
            }
        }
        else if (line[0] == '>') {
            newline = "<blockquote>" + line.substr(1) + "</blockquote>";
        }
        else if (line.substr(0, 3) == "```") {
            newline = codeblockopen ? "</code></pre>" : "<pre><code>";
            codeblockopen = !codeblockopen;
        }
        else {
            processParagraph(line);
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
    markdownInterpreter interpreter;
    ErrorCorrector corrector;
    string output;

    if (!mfile.is_open()) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    while (getline(mfile, line)) {
        output += interpreter.processline(line);
    }
    output = interpreter.finalCloseList(output);
    corrector.submitCode(output);
    output = corrector.checkCode();

    ofstream out("output.html");
    out << output;
    out.close();

    cout << "Conversion complete. Output saved to output.html" << endl;
    return 0;
}