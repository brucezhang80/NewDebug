/****************************************************************************
**
****************************************************************************/

#ifndef QTFILECOPIER_H
#define QTFILECOPIER_H

#include <QtCore/QObject>

#if defined(Q_WS_WIN)
#  if !defined(QT_QTCOPYDIALOG_EXPORT) && !defined(QT_QTCOPYDIALOG_IMPORT)
#    define QT_QTCOPYDIALOG_EXPORT
#  elif defined(QT_QTCOPYDIALOG_IMPORT)
#    if defined(QT_QTCOPYDIALOG_EXPORT)
#      undef QT_QTCOPYDIALOG_EXPORT
#    endif
#    define QT_QTCOPYDIALOG_EXPORT __declspec(dllimport)
#  elif defined(QT_QTCOPYDIALOG_EXPORT)
#    undef QT_QTCOPYDIALOG_EXPORT
#    define QT_QTCOPYDIALOG_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTCOPYDIALOG_EXPORT
#endif

class QtFileCopierPrivate;

class QT_QTCOPYDIALOG_EXPORT QtFileCopier : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int progressInterval READ progressInterval WRITE setProgressInterval)
    Q_PROPERTY(bool autoReset READ autoReset WRITE setAutoReset)
public:

    QtFileCopier(QObject *parent = 0);
    ~QtFileCopier();

    enum State {
        Idle,
        Completed,
        Busy,
        WaitingForInteraction
    };

    enum CopyFlag {
        NonInteractive = 0x01,
        Force = 0x02,
        MakeLinks = 0x04,
        FollowLinks = 0x08 // if not set links are copied
    };

    enum Error {
        NoError,
        SourceNotExists,
        DestinationExists,
        SourceDirectoryOmitted,
        SourceFileOmitted,
        PathToDestinationNotExists,
        CannotCreateDestinationDirectory,
        CannotOpenSourceFile,
        CannotOpenDestinationFile,
        CannotRemoveDestinationFile,
        CannotCreateSymLink,
        CannotReadSourceFile,
        CannotWriteDestinationFile,
        CannotRemoveSource,
        Canceled
    };

    Q_DECLARE_FLAGS(CopyFlags, CopyFlag)

    int copy(const QString &sourceFile, const QString &destinationPath,
                CopyFlags flags = 0);
    QList<int> copyFiles(const QStringList &sourceFiles, const QString &destinationDir,
                CopyFlags flags = 0);

    // 2013/5/29 MacaiÌí¼Ó
    QList<int> copyFileDestinations(const QString &sourceFile, const QStringList &destinationDirs,
                CopyFlags flags = 0);

    QList<int> copyDirectory(const QString &sourceDir, const QString &destinationDir,
                CopyFlags flags = 0);

    int move(const QString &sourceFile, const QString &destinationPath,
                CopyFlags flags = 0);
    QList<int> moveFiles(const QStringList &sourceFiles, const QString &destinationDir,
                CopyFlags flags = 0);
    QList<int> moveDirectory(const QString &sourceDir, const QString &destinationDir,
                CopyFlags flags = 0);

    QList<int> pendingRequests() const;
    QString sourceFilePath(int id) const;
    QString destinationFilePath(int id) const;
    bool isDir(int id) const;
    QList<int> entryList(int id) const;
    int currentId() const;

    State state() const;

    void setAutoReset(bool on);
    bool autoReset() const;
    int progressInterval() const;
    void setProgressInterval(int ms);

public Q_SLOTS:

    void cancelAll();
    void cancel(int id);

    void skip();
    void skipAll();
    void retry();

    void overwrite();
    void overwriteAll();

    void reset();
    void resetSkip();
    void resetOverwrite();

Q_SIGNALS:
    void error(int id, QtFileCopier::Error error, bool stopped);

    void stateChanged(QtFileCopier::State state);

    void done(bool error);
    void started(int id);
    void dataTransferProgress(int id, qint64 progress);
    void finished(int id, bool error);
    void canceled();

private:

    QtFileCopierPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtFileCopier)
    Q_DISABLE_COPY(QtFileCopier)

    Q_PRIVATE_SLOT(d_func(), void copyStarted(int))
    Q_PRIVATE_SLOT(d_func(), void copyFinished(int, bool))
    Q_PRIVATE_SLOT(d_func(), void copyCanceled())
    Q_PRIVATE_SLOT(d_func(), void copyError(int, QtFileCopier::Error, bool))
    Q_PRIVATE_SLOT(d_func(), void progressRequest())
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QtFileCopier::CopyFlags)

#endif
