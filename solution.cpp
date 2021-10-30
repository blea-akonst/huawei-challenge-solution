#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

bool check(string s1, string s2)
{
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] != '1' && s2[i] == '1') return true;
        if (s1[i] == '1' && s2[i] != '1') return false;
    }
    return false;
}

struct rule {
    int left, right, index;
    string name, l, r;

    friend bool operator < (const rule& rule1, const rule& rule2) {
        if (rule1.left != -1) {
            return rule1.left < rule2.left;
        }
        return rule1.l < rule2.l;
    }

    friend ostream& operator<<(ostream &out, const rule &_rule) {
        if (_rule.left != -1) {
            out << '(' << _rule.left << ',' << _rule.right << ')';
        } else {
            out << _rule.name;
        }
        out << '[' << _rule.index << ']';

        return out;
    }

};

void printRule(const vector<vector<rule>> &v) {
    for (int i = 0; i < v[0].size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            cout << v[j][i] << ' ';
        }
        cout << "\n";
    }
}

rule convert(const string &s) {
    rule res;
    res.left = -1;

    int pos = s.find('-');
    if (pos > 0) {
        res.left = stoi(s.substr(0, pos));
        res.right = stoi(s.substr(pos + 1));
    } else {
        res.name = s;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == '*') {
                res.l += '0';
                res.r += '1';
            } else {
                res.l += s[i];
                res.r += s[i];
            }
        }
    }
    return res;
}

int main() {
    char buf_in[2048];

    ifstream ifile("input.txt", ios::binary);
    ofstream ofile("output.txt", ios::binary);

    ifile.rdbuf()->pubsetbuf(buf_in, sizeof buf_in);

    string s, w;

    getline(ifile, s);
    int n = stoi(s), dim = 0;
    getline(ifile, s);
    vector<vector<rule>> _rule(5, vector<rule>(n));
    s = s + " ";

    for (int i = 0; i < s.size(); i++) {
        if (s[i] == ' ') {
            _rule[dim][0] = convert(w);
            _rule[dim][0].index = 0;
            dim++;
            w = "";
        } else {
            w += s[i];
        }
    }
    _rule.resize(dim);
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < dim; j++) {
            ifile >> s;
            _rule[j][i] = convert(s);
            _rule[j][i].index = i;
        }
    }
    vector<vector<rule>> cop = _rule, r_rule = _rule;
    for (int i = 0; i < r_rule.size(); i++) {
        for (int j = 0; j < r_rule[i].size(); j++) {
            if (r_rule[i][j].left != -1) {
                swap(r_rule[i][j].left, r_rule[i][j].right);
            } else

                swap(r_rule[i][j].l, r_rule[i][j].r);
        }
    }

    for (int i = 0; i < dim; i++) {
        sort(_rule[i].begin(), _rule[i].end());
        sort(r_rule[i].begin(), r_rule[i].end());
    }
    //printRule(r_rule);


    int m, l, r, mid, x, ans, q;
    vector<string> S(dim);
    ifile >> m;

    while(m--) {
        std::vector<bool> f(n);

        for (int i = 0; i < dim; i++) {
            ifile >> S[i];

            s = S[i];
            l = 0;
            r = n;
            if (_rule[i][0].left != -1) x = stoi(s);
            //else l = n;
            while (l + 1 < r) {
                mid = (l + r) / 2;
                if (_rule[i][mid].left == -1) {
                    if (_rule[i][mid].l > s) {
                        r = mid;
                    } else l = mid;
                } else {


                    if (_rule[i][mid].left > x) {
                        r = mid;
                    } else l = mid;
                }
            }

            for (int j = l + 1; j < n; j++) {
                f[_rule[i][j].index] = true;
                //cout << _rule[i][j].index << ' ';
            }

            l = -1;
            r = n - 1;

            while (l + 1 < r) {
                mid = (l + r) / 2;
                if (r_rule[i][mid].left == -1) {
                    if (r_rule[i][mid].l < s) {
                        l = mid;
                    } else r = mid;
                } else {
                    if (r_rule[i][mid].left < x) {
                        l = mid;
                    } else r = mid;
                }
            }

            for (int j = 0; j < r; j++) {
                f[r_rule[i][j].index] = true;
                //cout << _rule[i][j].index << ' ';
            }


        }


        bool c = true;
        for (int i = 0; i < n; i++) {
            if (!f[i]) {
                ans = 0;

                for (int j = 0; j < dim; j++) {
                    //cout << S[j] << ' ' << j << ' ' << i << "\n";
                    if (cop[j][i].left != -1) {
                        x = stoi(S[j]);
                        //cout << cop[j][i].left << ' ' << x << ' ' << cop[j][i].right << "\n";
                        if (cop[j][i].left <= x && x <= cop[j][i].right) ans++;
                    } else {
                        q = 0;
                        //cout << cop[j][i].name << ' ' << S[j] << "\n";
                        for (int k = 0; k < S[j].size(); k++) {
                            if (cop[j][i].name[k] == '1' && S[j][k] == '0' ||
                                cop[j][i].name[k] == '0' && S[j][k] == '1') q++;
                            if (q) break;
                        }
                        if (!q) ans++;
                    }
                }
                //cout << m << ' ' << i << ' ' << ans <<' ';
                if (ans == dim) { c = false;ofile << i << "\n";break;}


            }
        }
        if (c) ofile << -1 << "\n";


    }
    ifile.close();
    ofile.close();

    return 0;
}
