# TestStringSorting

by Giovanni Dicanio

This simple C++ benchmark measures sorting times of **string vectors**.

**Compiler**: VS2015 with Update 3  
**CPU**: Intel i7-6700 @ 3.40GHz  
**RAM**: 32GB  
**OS**: Windows 10  


##Benchmark Results##

**Note**: Execution times are expressed in **milliseconds**.

| Configuration | ATL   | STL   | PTR   |
| ------------- |:-----:|:-----:|:-----:|
| 32-bit        | 3110	| 3278  | 2332  |
| 64-bit        | 3272  | 3204  | 2472  |
| 32-bit SSO    | 1218  | 700   | 495   |
| 64-bit SSO    | 1235  | 609   | 512   |


##Conclusions##

- In 32-bit builds, ATL's CStringW performs _slightly_ better.
- In 64-bit builds, STL's wstring performs _slightly_ better.
- With _small strings_, STL's wstring performs _much_ better than ATL, thanks to the Small String Optimization (SSO).
- Raw wchar_t pointers show best performance.

