#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

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