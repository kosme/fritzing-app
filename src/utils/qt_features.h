// qt_features.h
#pragma once

#include <QtGlobal> // for QT_VERSION and QT_VERSION_CHECK
#include <QString>

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
    #define HAS_QT_STRINGLITERALS 1
#else   
    #define HAS_QT_STRINGLITERALS 0
    // inline QString operator"" _s(const char* str, std::size_t len) {
    //     return QString::fromUtf8(str, static_cast<int>(len));
    // }
    // UTF-16 string literal (const char16_t*)
    inline QString operator"" _s(const char16_t* str, std::size_t len) {
        return QString::fromUtf16(str, static_cast<int>(len));
    }
#endif