#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <tchar.h>
#include <Windows.h>
#include <Max.h>
#include <QString>

// 插件窗口描述
#define IDC_WIN_TITEL                   "SouthgisMax in 3dsmax Demo"

// xml
#define XML_DTD                         "<!DOCTYPE xyori>"
#define XML_START_TAG                   "xyori"
#define XML_START_VERSION               "version"
#define XML_START_VERSION_VALUE         "1.0"

#define XML_TAG_CATEGORY                "category"
#define XML_CATEGORY_ATTR_NAME          "name"
#define XML_CATEGORY_ATTR_EXPAND        "expanded"

#define XML_TAG_FILE                    "file"
#define XML_FILE_ATTR_TITLE             "title"
#define XML_ARCHIVE_NAME                "archive"
#define XML_TAG_PROP                    "propertise"

//file
#define XML_FILENAME                    "tree.xml"

// Archive
// 该密码是否需要随机生成?
static const QString ARCHIVE_PASSWORD = "";
#endif
