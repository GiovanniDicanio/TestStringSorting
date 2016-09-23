////////////////////////////////////////////////////////////////////////////////
// Test String Performance Sorting a Vector of Strings
// by Giovanni Dicanio
////////////////////////////////////////////////////////////////////////////////

// To test STL's Small String Optimization (SSO), #define TEST_SSO:
//#define TEST_SSO

#include <string.h>     // wcscmp
#include <algorithm>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <vector>
#include <atlstr.h>
#include <windows.h>
using namespace std;

long long counter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li.QuadPart;
}

long long frequency() {
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    return li.QuadPart;
}

void print_time(const long long start, const long long finish, const char * const s) {
    cout << s << ": " << (finish - start) * 1000.0 / frequency() << " ms" << endl;
}


// StringPtr does *not* own the string: it's an *observing* pointer!
class StringPtr {
public:
    StringPtr() : m_ptr(nullptr) {}
    explicit StringPtr(const wchar_t* ptr) : m_ptr(ptr) {}

    const wchar_t* Ptr() const {
        return m_ptr;
    }

    friend bool operator<(const StringPtr& a, const StringPtr& b) {
        return (wcscmp(a.m_ptr, b.m_ptr) < 0);
    }

private:
    const wchar_t* m_ptr;  // Observing pointer
};


void print_test_conditions() {
    cout << "(";
#if defined(_M_X64)
    cout << "64-bit";
#elif defined(_M_IX86)
    cout << "32-bit";
#endif

#ifdef TEST_SSO
    cout << "; testing with small strings)";
#else
    cout << ")";
#endif

    cout << "\n\n";
}

int main() {
    cout << "*** Testing String Sorting Performance ***\n\n";

    print_test_conditions();

    cout << "Building the string vectors for testing...\n\n";
    const auto shuffled = []() -> vector<wstring> {
        const wstring lorem[] = {
            L"Lorem ipsum dolor sit amet, consectetuer adipiscing elit.",
            L"Maecenas porttitor congue massa. Fusce posuere, magna sed",
            L"pulvinar ultricies, purus lectus malesuada libero,",
            L"sit amet commodo magna eros quis urna.",
            L"Nunc viverra imperdiet enim. Fusce est. Vivamus a tellus.",
            L"Pellentesque habitant morbi tristique senectus et netus et",
            L"malesuada fames ac turpis egestas. Proin pharetra nonummy pede.",
            L"Mauris et orci. [*** add more chars to prevent SSO ***]"
        };

        vector<wstring> v;

        for (int i = 0; i < 200000; ++i) {
            for (auto& s : lorem) {
#ifdef TEST_SSO
                UNREFERENCED_PARAMETER(s);
                v.push_back(L"#" + to_wstring(i));
#else
                v.push_back(s + L" (#" + to_wstring(i) + L")");
#endif
            }
        }

        mt19937 prng(1729);

        shuffle(v.begin(), v.end(), prng);

        return v;
    }();

    const auto shuffled_ptrs = [&]() -> vector<const wchar_t *> {
        vector<const wchar_t *> v;

        for (auto& s : shuffled) {
            v.push_back(s.c_str());
        }

        return v;
    }();

    long long start = 0;
    long long finish = 0;

    vector<ATL::CStringW> atl1(shuffled_ptrs.begin(), shuffled_ptrs.end());
    vector<wstring> stl1 = shuffled;
    vector<StringPtr> ptr1(shuffled_ptrs.begin(), shuffled_ptrs.end());

    vector<ATL::CStringW> atl2(shuffled_ptrs.begin(), shuffled_ptrs.end());
    vector<wstring> stl2 = shuffled;
    vector<StringPtr> ptr2(shuffled_ptrs.begin(), shuffled_ptrs.end());

    vector<ATL::CStringW> atl3(shuffled_ptrs.begin(), shuffled_ptrs.end());
    vector<wstring> stl3 = shuffled;
    vector<StringPtr> ptr3(shuffled_ptrs.begin(), shuffled_ptrs.end());

    cout << "Starting sorting.\n\n";

    start = counter();
    sort(atl1.begin(), atl1.end());
    finish = counter();
    print_time(start, finish, "ATL1");

    start = counter();
    sort(stl1.begin(), stl1.end());
    finish = counter();
    print_time(start, finish, "STL1");

    start = counter();
    sort(ptr1.begin(), ptr1.end());
    finish = counter();
    print_time(start, finish, "PTR1");

    cout << "\n";

    start = counter();
    sort(atl2.begin(), atl2.end());
    finish = counter();
    print_time(start, finish, "ATL2");

    start = counter();
    sort(stl2.begin(), stl2.end());
    finish = counter();
    print_time(start, finish, "STL2");

    start = counter();
    sort(ptr2.begin(), ptr2.end());
    finish = counter();
    print_time(start, finish, "PTR2");

    cout << "\n";

    start = counter();
    sort(atl3.begin(), atl3.end());
    finish = counter();
    print_time(start, finish, "ATL3");

    start = counter();
    sort(stl3.begin(), stl3.end());
    finish = counter();
    print_time(start, finish, "STL3");

    start = counter();
    sort(ptr3.begin(), ptr3.end());
    finish = counter();
    print_time(start, finish, "PTR3");
}
