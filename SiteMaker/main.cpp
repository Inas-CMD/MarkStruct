#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Styler {
private:
    string lightTheme = R"(
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  background: linear-gradient(135deg, #ffffff 0%, #f0f0f0 100%);
  color: #333;
  font-family: 'Arial', sans-serif;
  transition: background 0.3s ease;
}

a {
  color: #1e90ff;
  text-decoration: none;
}

a:hover {
  text-decoration: underline;
}

button {
  background: #ffffff;
  border: 1px solid #cccccc;
  color: #333;
  font-family: 'Arial', sans-serif;
}

input {
  background: #ffffff;
  border: 1px solid #cccccc;
  color: #333;
}
)";

    string darkTheme = R"(
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  background: #121212;
  color: #e0e0e0;
  font-family: 'Arial', sans-serif;
  transition: background 0.3s ease;
}

a {
  color: #bb86fc;
  text-decoration: none;
}

a:hover {
  text-decoration: underline;
}

button {
  background: #333333;
  border: 1px solid #444444;
  color: #e0e0e0;
  font-family: 'Arial', sans-serif;
}

input {
  background: #333333;
  border: 1px solid #444444;
  color: #e0e0e0;
}
)";

    // Seaside Theme
    string seasideTheme = R"(
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  background: url('https://scontent-vie1-1.xx.fbcdn.net/v/t39.30808-6/311667397_454111373484989_9219682886027814393_n.jpg?_nc_cat=102&ccb=1-7&_nc_sid=cc71e4&_nc_eui2=AeFzGFcgDAPeXwFM9EAKjIjxqw2f2F3dZV-rDZ_YXd1lX0c4n5iCNRN_siywLJ4eyiX2RfaIp_zHqhKIOuzujIN5&_nc_ohc=ty1whWJLuTMQ7kNvgErqvp0&_nc_zt=23&_nc_ht=scontent-vie1-1.xx&_nc_gid=AkNOw2iFc9fugXY1UcMQmML&oh=00_AYB2526NVStxMy85ori0dH-FVT-lzKgF2jIxFxcMH74EHw&oe=679EA55F') no-repeat center center fixed;
  background-size: cover;
  color: #ffffff;
  font-family: 'Arial', sans-serif;
  transition: background 0.5s ease-in-out;
}

a {
  color: #00bcd4;
  text-decoration: none;
}

a:hover {
  text-decoration: underline;
}

button {
  background: rgba(0, 188, 212, 0.7);
  border: 1px solid #00bcd4;
  color: #ffffff;
  font-family: 'Arial', sans-serif;
}

input {
  background: rgba(0, 188, 212, 0.7);
  border: 1px solid #00bcd4;
  color: #ffffff;
}
)";

    string greenCoderTheme = R"(
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  background: #2e8b57;
  color: #f4f4f4;
  font-family: 'Courier New', Courier, monospace;
  transition: background 0.3s ease;
}

a {
  color: #ff6347;
  text-decoration: none;
}

a:hover {
  text-decoration: underline;
}

button {
  background: #3b9a4f;
  border: 1px solid #336b3b;
  color: #f4f4f4;
  font-family: 'Courier New', Courier, monospace;
}

input {
  background: #3b9a4f;
  border: 1px solid #336b3b;
  color: #f4f4f4;
}
)";

    // Purple Theme
    string purpleTheme = R"(
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  background: linear-gradient(135deg, #6a0dad, #9b30b6);
  color: #fff;
  font-family: 'Arial', sans-serif;
  animation: pulse 3s infinite;
}

a {
  color: #f0f0f0;
  text-decoration: none;
}

a:hover {
  text-decoration: underline;
}

button {
  background: #6a0dad;
  border: 1px solid #9b30b6;
  color: #fff;
  font-family: 'Arial', sans-serif;
}

input {
  background: #6a0dad;
  border: 1px solid #9b30b6;
  color: #fff;
}

@keyframes pulse {
  0% {
    background-color: #6a0dad;
  }
  50% {
    background-color: #9b30b6;
  }
  100% {
    background-color: #6a0dad;
  }
}
)";

public:
    string ThemeSelector(int themenum) {
        switch (themenum)
        {
        case 1:
            return lightTheme;
        case 2:
            return darkTheme;
        case 3:
            return seasideTheme;
        case 4:
            return greenCoderTheme;
        case 5:
            return purpleTheme;
        default:
            break;
        }
    }

};

class HTMLBuilder {
private:
    Styler st;

public:
    string ConstructSite(string code, string script, int style) {
        string html = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Site</title>
    <style>)" + st.ThemeSelector(style) + R"(</style>
            < / head >
            < body>"
)" + code + "<script>" + script + "</script>\n</head>\n<body>" + code + "</body>\n</html>";
        return html;
    }
        void MakeFile(const std::string& filename, const std::string& htmlCode) {
            std::ofstream htmlFile(filename);

            if (!htmlFile) {
                std::cerr << "Error: Could not create file: " << filename << std::endl;
                return;
            }

            htmlFile << htmlCode;


            htmlFile.close();

            std::cout << "HTML file created successfully: " << filename << std::endl;
        }
};

class TableConstructor {
private:
    vector<vector<string>> Thead;
    vector<vector<string>> Tbody;
    bool headHasEnded = false;
    bool TableConstructionInProgress = false;

    string trim(const string& s) {
        auto start = s.begin();
        while (start != s.end() && isspace(*start)) {
            start++;
        }
        auto end = s.end();
        while (end != start && isspace(*(end - 1))) {
            end--;
        }
        return string(start, end);
    }

    bool isAllDashes(const string& str) {
        for (char c : str) {
            if (c != '-') {
                return false;
            }
        }
        return !str.empty();
    }

public:
    bool fillTable(string line) {
        vector<string> tempvec;
        size_t start = 0;
        size_t end = line.find('|');

        while (end != string::npos) {
            string cell = line.substr(start, end - start);
            tempvec.push_back(trim(cell));
            start = end + 1;
            end = line.find('|', start);
        }
        tempvec.push_back(trim(line.substr(start)));

        if (!tempvec.empty() && tempvec.front().empty()) {
            tempvec.erase(tempvec.begin());
        }
        if (!tempvec.empty() && tempvec.back().empty()) {
            tempvec.pop_back();
        }

        bool isSeparator = true;
        for (string& cell : tempvec) {
            if (!isAllDashes(cell)) {
                isSeparator = false;
                break;
            }
        }

        if (isSeparator) {
            if (!headHasEnded) {
                headHasEnded = true;
            }
            return TableConstructionInProgress;
        }

        if (headHasEnded) {
            Tbody.push_back(tempvec);
        }
        else {
            Thead.push_back(tempvec);
        }

        TableConstructionInProgress = true;
        return TableConstructionInProgress;
    }

    void endTableFill() {
        TableConstructionInProgress = false;
    }

    string ConstructTable() {
        string html = "<table>\n";
        if (!Thead.empty()) {
            html += "  <thead>\n";
            for (auto& row : Thead) {
                html += "    <tr>\n";
                for (auto& cell : row) {
                    html += "      <th>" + cell + "</th>\n";
                }
                html += "    </tr>\n";
            }
            html += "  </thead>\n";
        }
        if (!Tbody.empty()) {
            html += "  <tbody>\n";
            for (auto& row : Tbody) {
                html += "    <tr>\n";
                for (auto& cell : row) {
                    html += "      <td>" + cell + "</td>\n";
                }
                html += "    </tr>\n";
            }
            html += "  </tbody>\n";
        }
        html += "</table>\n";
        return html;
    }
};

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

int main() {
    string filepath = "C:/test/test.txt";
    string line;
    ifstream mfile(filepath);
    markdownInterpreter i;
    ErrorCorrector ecc;
    HTMLBuilder hb;
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
    tempfile = hb.ConstructSite(tempfile, "", 1);
    hb.MakeFile("index.html", tempfile);
    
    system("pause");
    return 0;
}