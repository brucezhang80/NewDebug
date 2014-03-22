#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QStringList>

// 插件窗口描述
#define IDC_WIN_TITEL                   tr("SouthgisMax in 3dsmax Demo")

// xml
#define XML_DTD                         "<!DOCTYPE xyori>"
#define XML_START_TAG                   "xyori"
#define XML_START_VERSION               "version"
#define XML_START_VERSION_VALUE         "1.0"

#define XML_TAG_CATEGORY                "category"
#define XML_CATEGORY_ATTR_NAME          "name"
#define XML_CATEGORY_ATTR_ICON          "icon"

#define XML_TAG_FILE                    "file"
#define XML_FILE_TITLE                  "title"
#define XML_FILE_ARCHIVE                "archive"
#define XML_FILE_ICON                   "img"
#define XML_TAG_PROP                    "propertise"

//file
#define XML_DEFAULT_FILE               ":/toolbox.xml"
#define XML_FILENAME                    "data.xml"

// Toolbox page icons
static const QStringList PageIcons  = (QStringList() << ":/images/Rename.png"
                                       << ":/images/Material.png" << ":/images/ObjectEdit.png"
                                       << ":/images/ImportExport.png" << ":/images/SceneCheck.png"
                                       << ":/images/Other.png");
static const QString BtnIcon =         ":/images/Faverit.png";

// archive type
#define ARCHIVE_TYPE_ZIP                "*.zip"
#define ARCHIVE_TYPE_YRI                "*.yri"
#define ARCHIVE_TYPE_STH                "*.sth"
#define ARCHIVE_TYPE_NONE               ""

// data directory
#define DATA_RENAME                     "rename"
#define DATA_MATERIAL                   "material"
#define DATA_OBJEDIT                    "objectedit"
#define DATA_IMOREXPORT                 "importexport"
#define DATA_SCENECHECK                 "scenecheck"
#define DATA_OTHER                      "other"

// Archive
// 该密码是否需要随机生成?
static const QString ARCHIVE_PASSWORD = "";
#endif
