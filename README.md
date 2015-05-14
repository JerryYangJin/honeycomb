Honeycomb [![npm version](https://badge.fury.io/js/honeycomb.svg)](http://badge.fury.io/js/honeycomb)
=========

A hybrid data platform

Status
------
This project is still at early stage, DO NOT use for production.

Integration
-----------
| name | version |
| ---- | ------- |
| io.js | 2.0.0 |
| libgit2 | 0.22.2 |
| mysql | 5.6.22 |
| hadoop | 2.5.0 |

TODO
----
- [ ] Hadoop integration
  - [ ] libhdfs
  - [ ] pipe, stream
  - [ ] map reduce
- [x] ES6 native promise
- [x] mutext to protect global viarable
- [ ] git mysql backend
  - [ ] config backend
    - [ ] able to config at different levels
    - [ ] load git_cvar_cached for repository object

Issue
-----
- [ ] is.js 2.0.0 native addon compile fail on Linux and Mac
- [x] JNI_CreateJavaVM always return -3 (JNI_EVERSION)
- [ ] org/apache/hadoop/fs/FileSystem not found
- [ ] Run-Time Check Failure #2 - Stack around the variable 'xxx' was corrupted
  - [x] Windows 7 32-bit, Release mode fixed
  - [ ] Windows 7 32-bit, Debug mode, the odb_read still throws this error
- [x] mysql_stmt_fetch access violation

License
-------
The MIT License (MIT)

Copyright (c) 2015 Jerry Yang Jin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
