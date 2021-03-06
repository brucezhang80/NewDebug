TEMPLATE = app
#TARGET = app
QT += network xml

# Tmp file
#MOC_DIR = tmp/moc
#RCC_DIR = tmp/rcc
#UI_DIR = tmp/ui
#OBJECTS_DIR = tmp/obj

# Config qxmpp
CONFIG += console debug_and_release
INCLUDEPATH += ../lib/QXmppClient/source

CONFIG(debug, debug|release) {
    QXMPP_LIB = QXmppClient_d
    QXMPP_DIR = ../lib/QXmppClient/source/debug
    TARGET = qtalk_d
} else {
    QXMPP_LIB = QXmppClient
    QXMPP_DIR = ../lib/QXmppClient/source/release
    TARGET = qtalk
}

LIBS += -L$$QXMPP_DIR -l$$QXMPP_LIB
PRE_TARGETDEPS += $${QXMPP_DIR}/lib$${QXMPP_LIB}.a

# Input

RESOURCES = application.qrc

TRANSLATIONS = translations/qtalk_zh_CN.ts

SOURCES += main.cpp \
           MainWindow.cpp \
           ChatWindow.cpp \
           XmppMessage.cpp \
           RosterModel.cpp \
           UnreadMessageWindow.cpp \
           UnreadMessageModel.cpp \
           LoginWidget.cpp \
           PreferencesDialog.cpp \
           PrefAccount.cpp \
           Preferences.cpp \
           PrefWidget.cpp \
           PrefGeneral.cpp \
           CloseNoticeDialog.cpp \
           PrefChatWindow.cpp \
           MessageEdit.cpp \
           ContactInfoDialog.cpp \
           TransferManagerWindow.cpp \
           TransferManagerModel.cpp \
           AddContactDialog.cpp \
           InfoEventStackWidget.cpp \
           InfoEventSubscribeRequest.cpp
HEADERS += MainWindow.h \
           ChatWindow.h \
           XmppMessage.h \
           RosterModel.h  \
           UnreadMessageWindow.h \
           UnreadMessageModel.h \
           LoginWidget.h \
           PreferencesDialog.h \
           PrefAccount.h \
           Preferences.h \
           PrefWidget.h \
           PrefGeneral.h \
           CloseNoticeDialog.h \
           PrefChatWindow.h \
           MessageEdit.h \
           ContactInfoDialog.h \
           TransferManagerWindow.h \
           TransferManagerModel.h \
           AddContactDialog.h \
           InfoEventStackWidget.h \
           InfoEventSubscribeRequest.h
FORMS   += MainWindow.ui \
           UnreadMessageWindow.ui \
           LoginWidget.ui \
           ChatWindow.ui \
           PreferencesDialog.ui \
           PrefAccount.ui \
           PrefGeneral.ui \
           CloseNoticeDialog.ui \
           PrefChatWindow.ui \
           ContactInfoDialog.ui \
           TransferManagerWindow.ui \
           AddContactDialog.ui \
           InfoEventStackWidget.ui \
           InfoEventSubscribeRequest.ui

!isEmpty(TRANSLATIONS) {
  isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
 }
  isEmpty(TS_DIR):TS_DIR = translations
  TSQM.name = lrelease ${QMAKE_FILE_IN}
  TSQM.input = TRANSLATIONS
  TSQM.output = $$TS_DIR/${QMAKE_FILE_BASE}.qm
  TSQM.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN}
  TSQM.CONFIG = no_link
  QMAKE_EXTRA_COMPILERS += TSQM
  PRE_TARGETDEPS += compiler_TSQM_make_all
} else:message(No translation files in project)
