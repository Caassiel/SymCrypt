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
    if (c == 0x0401 || c == 0x0451)     return 0x0435;
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





int main()
{
    const string& input = "input.txt";
    ifstream in(input, ios::binary);
    if (!in) { cerr << "Error: cannot open input file\n"; return 1; }
    string bytes{ istreambuf_iterator<char>(in), {} };
    ofstream file("output.txt", ios::binary);

    wstring text = decodeUtf8(bytes);
    wstring withoutSpaces = NormaliseWithoutSpaces(text);
    string edited_text = encodeUtf8(withoutSpaces);

    file << edited_text << "\n";

    return 0;
}
