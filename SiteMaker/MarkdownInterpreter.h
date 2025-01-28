#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "TableConstructor.h"
#include "ErrorCorrector.h"


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
    bool TableinProgress = false;
    bool hasTableConstructed = false;
    string newline;
    TableConstructor tc;

    vector<pair<string, string>> footnotes;

public:
    void checkCloseList(int type, bool lionly = false) {
        if (openBlackquote == true) {
            newline = "</blockquote>\n" + newline;
            openBlackquote = false;
        }
        if (listitem_open == true) {
            newline = "</li>\n" + newline;
            listitem_open = false;
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
            newline = "</li></ul>\n" + newline;
            unordered_list_open = false;
            listitem_open = false;
        }
        if (ordered_list_open == true && type != 1) {
            listitem_open = false;
            newline = "</li></ol>\n" + newline;
            ordered_list_open = false;
        }


        if (!footnotes.empty()) {
            finaltext += "\n<ol>\n";
            for (const auto& fn : footnotes) {
                finaltext += "  <li id=\"fn" + fn.first + "\">" + fn.second + " <a href=\"#fnref" + fn.first + "\">↩</a></li>\n";
            }
            finaltext += "</ol>\n";
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
                    templine = templine + c;
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
        bool isImage = false;
        bool isFootnote = false;
        bool wasTriggered = false;
        bool isCode = false;
        int i = 0;
        bool linkc[3] = { false, false, false };
        bool previousWasOpenBracket = false;
        string currentFootnoteId;

        for (i = 0; i < line.length(); i++) {
            char c = line[i];
            switch (c) {
            case '!':
                wasTriggered = true;
                isImage = true;
                break;
            case '[':
                wasTriggered = true;
                linkc[0] = true;
                previousWasOpenBracket = true;
                break;
            case ']':
                linkc[0] = false;
                if (isFootnote) {
                    newline += "<sup id=\"fnref" + currentFootnoteId + "\"><a href=\"#fn" + currentFootnoteId + "\">" + currentFootnoteId + "</a></sup>";
                    isFootnote = false;
                }
                break;
            case '(':
                linkc[1] = true;
                break;
            case '^':
                if (previousWasOpenBracket) {
                    isFootnote = true;
                    currentFootnoteId = "";
                    while (++i < line.length()) {
                        char nextChar = line[i];
                        if (nextChar == ']') {
                            i--;
                            break;
                        }
                        currentFootnoteId += nextChar;
                    }
                }
                break;
            case ')':
                linkc[1] = false;
                if (!link.empty() && link.back() == ' ') {
                    link.pop_back();
                }
                if (isImage && wasTriggered) {
                    newline += "<img src=\"" + link + "\" alt=\"" + linkText + "\" title=\"" + title + "\">";
                }
                else if (wasTriggered) {
                    newline += "<a href=\"" + link + "\" title=\"" + title + "\">" + linkText + "</a>";
                }
                break;
            case '"':
                linkc[2] = !linkc[2];
                break;
            case '`':
                if (!codeblockopen) {
                    isCode = !isCode;
                    if (!isCode) {
                        newline += "<code>" + code + "</code>";
                        code.clear();
                    }
                }
                break;
            default:
                if (linkc[0] && !isFootnote) linkText += c;
                else if (linkc[1] && !linkc[2]) link += c;
                else if (linkc[2]) title += c;
                else if (isCode) code += c;
                else newline += c;
            }
            previousWasOpenBracket = (c == '[');
        }
    }

    void processParagraph(string line) {
        if (line.empty()) {
            if (paragraph_open) {
                paragraph_open = false;
                newline = newline + "</p>";
            }
        }
        else {
            if (paragraph_open == false) {
                paragraph_open = true;
                newline = "<p>";
            }
            newline = newline + line;
        }
    }

    void processUnList(string line) {
        if (unordered_list_open == false) {
            newline = "<ul>\n";
            unordered_list_open = true;
        }
        else {
            checkCloseList(0, true);
            checkCloseList(1);
        }
        line.erase(0, 2);
        newline = newline + "<li>" + line;
        listitem_open = true;
    }

    void processUnSublist(string line) {
        if (unordered_sublist_open == false) {
            newline = newline + "<ul>\n";
            unordered_sublist_open = true;
        }
        line.erase(0, 4);
        newline = newline + "<li>" + line + "</li>";
    }

    void processOList(string line) {
        checkCloseList(0, true);
        if (ordered_list_open == false) {
            newline = "<ol>\n";
            ordered_list_open = true;
        }
        else {
            checkCloseList(0, true);
            checkCloseList(1);
        }
        line.erase(0, 2);
        newline = newline + "<li>" + line;
        listitem_open = true;
    }

    void processOSublist(string line) {
        if (ordered_sublist_open == false) {
            newline = newline + "<ol>\n";
            ordered_sublist_open = true;
        }
        line.erase(0, 5);
        newline = newline + "<li>" + line + "</li>";
    }

    void processBlockquote(string line) {
        int BCount = 0;
        bool brakeloop = false;
        for (char c : line) {
            switch (c) {
            case '>':
                BCount++;
                break;
            default:
                brakeloop = true;
            }
            if (brakeloop) {
                break;
            }
        }
        switch (BCount) {
        case 0:
            break;
        case 1:
            if (openBlackquote == true) {
                checkCloseList(0, true);
            }
            openBlackquote = true;
            line.erase(0, 2);
            newline = "<blockquote>" + line;
            break;

        case 2:
            line.erase(0, 3);
            newline = "<blockquote>" + line + "</blockquote>";
            break;
        default:
            break;
        }
    }

    void processHorizontalRule() {
        newline = "<hr>";
    }

    void processCodeBlock(string line) {
        if (codeblockopen == false) {
            codeblockopen = true;
            newline = "<pre><code>";
        }
        else if (codeblockopen == true) {
            codeblockopen = false;
            newline = "</code></pre>";
        }
    }

    string processline(string line) {
        if (line.find("[^") == 0 && line.find("]:") != string::npos) {
            size_t colonPos = line.find("]:");
            string id = line.substr(2, colonPos - 2);
            string content = line.substr(colonPos + 3);
            footnotes.push_back(make_pair(id, content));
            return "";
        }

        newline = "";
        char infocus = line[0];

        if (isdigit(infocus) && line[1] == '.') {
            this->processOList(line);
        }
        else {
            switch (infocus) {
            case '#':
                TableinProgress = false;
                this->processHeader(line);
                this->checkCloseList(3);
                break;

            case '*':
                TableinProgress = false;
                if (line[1] == '*' && line[2] == '*') {
                    processHorizontalRule();
                    break;
                }
            case '+':
            case '-':
                TableinProgress = false;
                if (line[1] == ' ') {
                    this->processUnList(line);
                }
                else if (line[1] == '-' && line[2] == '-') {
                    this->processHorizontalRule();
                }
                break;

            case ' ':
                TableinProgress = false;
                if (unordered_list_open == true) {
                    if (line[2] == '-' || line[2] == '+' || line[2] == '*') {
                        this->processUnSublist(line);
                    }
                }
                else if (isdigit(line[3]) && line[4] == '.') {
                    this->processOSublist(line);
                }
                break;

            case '[':
                TableinProgress = false;
                this->processRemainingMarkdown(line);
                break;

            case '>':
                TableinProgress = false;
                this->processBlockquote(line);
                break;

            case '~':
            case '`':
                TableinProgress = false;
                if (line[1] == '`' || line[1] == '~') {
                    this->processCodeBlock(line);
                }
                break;

            case '_':
                if (line[1] == '_' && line[2] == '_')
                    this->processHorizontalRule();
                break;
            case '\\':
                break;

            case '|':
                hasTableConstructed = true;
                TableinProgress = tc.fillTable(line);
                break;

            default:
                TableinProgress = false;
                this->processParagraph(line);
                this->checkCloseList(3);
                break;
            }
        }
        if (TableinProgress == false) {
            if (hasTableConstructed == true) {
                newline += tc.ConstructTable();
                hasTableConstructed = false;
            }
            newline = this->TextStyle(newline);
            this->processRemainingMarkdown(newline);
            newline = newline + "\n";
        }
        return newline;
    }
};
