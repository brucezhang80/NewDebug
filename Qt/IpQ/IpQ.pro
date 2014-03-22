#-------------------------------------------------
#
# Project created by QtCreator 2013-06-04T14:31:12
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IpQ
TEMPLATE = app
TRANSLATIONS = ./lang/zh_CN.ts

SOURCES += main.cpp\
        main_window.cpp \
    systemtray.cpp \
    usertreewidget.cpp \
    aboutdialog.cpp \
    message_base.cpp \
    owner.cpp \
    message_server.cpp \
    message.cpp \
    recv_message.cpp \
    message_thread.cpp \
    macai.cpp \
    send_message.cpp \
    user_manager.cpp \
    preferences.cpp \
    system.cpp \
    useritemwidget.cpp \
    window_manager.cpp \
    message_window.cpp \
    message_box.cpp \
    application.cpp \
    group_manager.cpp \
    transfer_file_window.cpp \
    send_file_treewidget.cpp \
    recv_file_treewidget.cpp \
    send_file_thread.cpp \
    recv_file_thread.cpp \
    file_tcpserver.cpp \
    recv_file_transfer.cpp \
    recv_file_client.cpp \
    send_file_client.cpp \
    qtfilecopier.cpp \
    send_file_transfer.cpp \
    send_users_treewidget.cpp \
    send_history_tableview.cpp \
    addrecieverdlg.cpp \
    send_file_propertise.cpp \
    system_settings.cpp \
    configuration_page.cpp \
    folderconfig_page.cpp \
    helperconfig_page.cpp \
    messageconfig_page.cpp

HEADERS  += main_window.h \
    systemtray.h \
    usertreewidget.h \
    aboutdialog.h \
    message_base.h \
    owner.h \
    ipq.h \
    constants.h \
    message_server.h \
    message.h \
    recv_message.h \
    message_thread.h \
    macai.h \
    send_message.h \
    user_manager.h \
    preferences.h \
    system.h \
    useritemwidget.h \
    window_manager.h \
    message_window.h \
    message_box.h \
    application.h \
    group_manager.h \
    transfer_file_window.h \
    send_file_treewidget.h \
    recv_file_treewidget.h \
    send_file_thread.h \
    recv_file_thread.h \
    file_tcpserver.h \
    recv_file_transfer.h \
    recv_file_client.h \
    send_file_client.h \
    qtfilecopier.h \
    send_file_transfer.h \
    send_users_treewidget.h \
    send_history_tableview.h \
    addrecieverdlg.h \
    send_file_propertise.h \
    system_settings.h \
    configuration_page.h \
    folderconfig_page.h \
    helperconfig_page.h \
    messageconfig_page.h

RESOURCES += \
    res.qrc

OTHER_FILES += \
    themes/default.qss

FORMS += \
    AddRecieverDlg.ui \
    sendfilepropertise.ui \
    systemsettings.ui \
    configurationpage.ui \
    folderconfigpage.ui \
    helperconfigpage.ui \
    messageconfigpage.ui
LIBS += -lws2_32 Netapi32.lib advapi32.lib
