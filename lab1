#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <codecvt>

using namespace std;

//coding and decoding
wstring decodeUtf8(const string& s) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}

string encodeUtf8(const wstring& s) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(s);
}

//filtering
static bool isLetter(wchar_t c) {
    return  (c >= 0x0410 && c <= 0x044F) || c == 0x0401 || c == 0x0451;
}

static wchar_t toLower(wchar_t c) {
    if (c >= 0x0410 && c <= 0x042F)     return c + 0x20;
    if (c == 0x0401)                    return 0x0451;
    return c;
}

static wstring stripSpaces(const wstring& t) {
    wstring r;
    for (wchar_t c : t) if (c != L' ') r += c;
    return r;
}

static wstring NormaliseWithSpaces(const wstring& t) {
    wstring r;
    bool special_char = true;
    for (wchar_t c : t) {
        if (isLetter(c)) {
            r += toLower(c);
            special_char = false;
        } else {
            if (!special_char) { r += L' '; special_char = true; }
        }
    }
    if (!r.empty() && r.back() == L' ') r.pop_back();
    return r;
}

static wstring NormaliseWithoutSpaces(const wstring& t) {
    return stripSpaces(NormaliseWithSpaces(t));
}

//stats
using CharFreq = map<wchar_t, long long>;
using BigramFreq = map<pair<wchar_t,wchar_t>, long long>;

struct Stats {
    CharFreq  cf;
    long long nc = 0;

    BigramFreq  bo;
    long long no = 0;

    BigramFreq  bn;
    long long nn = 0;
};

static Stats CalcStats(const wstring& t) {
    Stats s;
    for (wchar_t c : t) {
        s.cf[c]++; s.nc++;
    }

    for (size_t i = 0; i + 1 < t.size(); ++i) {
        s.bo[{t[i], t[i+1]}]++;
        s.no++;
    }
    for (size_t i = 0; i + 1 < t.size(); i += 2) {
        s.bn[{t[i], t[i+1]}]++;
        s.nn++;
    }
    return s;
}

//entropy
static double H1_def(const CharFreq& f, long long n) {
    double h = 0;
    for (auto& [c, k] : f) {
        double p = (double)k / n;
        if (p > 0) h -= p * log2(p);
    }
    return  h / 2;
}

static double H2_def(const BigramFreq& f, long long n) {
    double h = 0;
    for (auto& [b, k] : f) {
        double p = (double)k / n;
        if (p > 0) h -= p * log2(p);
    }
    return  h / 2;
}

static double H1_emp(const CharFreq& f, long long n) {
    const double LN2 = log(2.0);
    double r = lgamma((double)n + 1) / LN2;
    for (auto& [c, k] : f) r -= lgamma((double)k + 1) / LN2;
    return  r / n;
}

static double H2_emp(const BigramFreq& f, long long n) {
    const double LN2 = log(2.0);
    double r = lgamma((double)n + 1) / LN2;
    for (auto& [b, k] : f) r -= lgamma((double)k + 1) / LN2;
    return r / n / 2;
}

// printing
static string symLabel(wchar_t c) {
    if (c == L' ') return "[sp]";
    wstring C; C += c;
    return encodeUtf8(C);
}

static int displayCols(const string& s) {
    int n = 0;
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = (unsigned char)s[i];
        if      (c < 0x80) i += 1;
        else if (c < 0xE0) i += 2;
        else if (c < 0xF0) i += 3;
        else               i += 4;
        ++n;
    }
    return n;
}

static void printW(ostream& out, const string& s, int w, bool right = false) {
    int pad = w - displayCols(s);
    if (pad < 0) pad = 0;
    if (right) { out << string(pad, ' ') << s; }
    else       { out << s << string(pad, ' '); }
}

static void printEntropy(ostream& out, const Stats& s) {
    out << "\n  [H1: symbol entropy]\n";
    out << "    Definition : "  << fixed << setprecision(4) << H1_def(s.cf, s.nc) << " bits/symbol\n";
    out << "    Empirical        : " << H1_emp(s.cf, s.nc)       << " bits/symbol\n";

    out << "\n  [H2: entropy per symbol, OVERLAPPING bigrams  |  total: " << s.no << "]\n";
    out << "    Definition : "  << H2_def(s.bo, s.no) << " bits/symbol\n";
    out << "    Empirical        : " << H2_emp(s.bo, s.no) << " bits/symbol\n";

    out << "\n  [H2: entropy per symbol, NON-OVERLAPPING bigrams  |  total: " << s.nn << "]\n";
    out << "    Definition  : " << H2_def(s.bn, s.nn) << " bits/symbol\n";
    out << "    Empirical        : " << H2_emp(s.bn, s.nn) << " bits/symbol\n";

    out << "\n\n";
}

static void SymbolTable(ostream& out, const CharFreq& f, long long n) {
    vector<pair<long long, wchar_t>> v;
    v.reserve(f.size());
    for (auto& [c, ch] : f) v.push_back({ch, c});
    sort(v.rbegin(), v.rend());

    const int W_SYM   = 6;
    const int W_COUNT = 12;
    const int W_PROB  = 10;

    out << "| "; printW(out, "Sym",   W_SYM,   false); out << " | ";
    out << "     Count" << "   | ";
    out << "  Prob (%) |\n";

    for (auto& [c, ch] : v) {
        string sym = symLabel(ch);
        out << "| "; printW(out, sym, W_SYM, false); out << " | ";
        out << right << setw(W_COUNT) << c << " | ";
        out << fixed << setprecision(5) << setw(W_PROB) << 100.0 * c / n << " |\n";
    }
}

static void BigramMatrix(ostream& out, const BigramFreq& f, const vector<wchar_t>& alpha, const string& title) {
    out << "\n  [" << title << "]\n";

    long long mx = 1;
    for (auto& [b, n] : f) mx = max(mx, n);
    int cw = (int)to_string(mx).size() + 1;

    out << "  ";
    for (wchar_t c : alpha) printW(out, symLabel(c), cw, true);
    out << "\n";

    for (wchar_t row : alpha) {
        printW(out, symLabel(row), 4, false);
        for (wchar_t col : alpha) {
            auto it = f.find({row, col});
            if (it == f.end() || it->second == 0)
                out << string(cw - 1, ' ') << ".";
            else
                out << setw(cw) << it->second;
        }
        out << "\n";
    }
}


int main() {
    const string& input = "input.txt";
    ifstream in(input, ios::binary);
    if (!in) { cerr << "Error: cannot open input file\n"; return 1; }
    string bytes{ istreambuf_iterator<char>(in), {} };
    ofstream file("report.txt", ios::binary);

    wstring text = decodeUtf8(bytes);
    wstring withSpaces    = NormaliseWithSpaces(text);
    wstring withoutSpaces = NormaliseWithoutSpaces(text);
    string edited_text = encodeUtf8(withoutSpaces);

    Stats sWS = CalcStats(withSpaces);
    Stats sNS = CalcStats(withoutSpaces);

    file << "Entropy on text with spaces: ";
    printEntropy(file, sWS);
    file << "Entropy on text without spaces: ";
    printEntropy(file, sNS);
    SymbolTable(file, sWS.cf, sWS.nc);
    file << "\n\n";
    SymbolTable(file, sNS.cf, sNS.nc);

    vector<wchar_t> alpha;
    for (auto& [c, n] : sWS.cf) alpha.push_back(c);

    sort(alpha.begin(), alpha.end(), [&](wchar_t a, wchar_t b) {
        return sWS.cf.at(a) > sWS.cf.at(b);
    });

    alpha.resize(30);

    BigramMatrix(file, sWS.bo, alpha, "OVERLAPPING bigrams");
    BigramMatrix(file, sWS.bn, alpha, "NON-OVERLAPPING bigrams");

    return 0;
}
