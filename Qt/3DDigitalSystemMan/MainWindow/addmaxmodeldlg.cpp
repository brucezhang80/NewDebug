#include "addmaxmodeldlg.h"
#include "ui_addmaxmodeldlg.h"
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QUrl>
#include <QFileInfo>
#include <QMessageBox>
#include "modeltreeview.h"

AddMaxModelDlg::AddMaxModelDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMaxModelDlg)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);//希望该窗口在退出时自动能够delete掉自己

    m_thumbFilename = QString(":/images/nopic.jpg");
    m_maxFilename = QString("");
    ui->label_modelThumb->setPixmap(QPixmap(m_thumbFilename));
    ui->label_modelThumb->setScaledContents(true);//为设置QLabel自动缩放，既：显示图像大小自动调整为Qlabel大小。
    ui->label_modelThumb->setAcceptDrops(true);
    ui->label_modelThumb->installEventFilter(this);
    ui->comboBox_subModeType->setHidden(1);
    ui->comboBox_modelType->setItemDelegate(new ComboBoxDelegate(this));

    connect(ui->pushButton_selModelFile, SIGNAL(clicked()), this, SLOT(OnSelMaxModelFileSlot()));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(OnButtonBoxSlot(QAbstractButton*)));

    //sent a signal to this parent window(mainwindow)
    connect( this, SIGNAL(addMaxModelSignal(const QString&, const QString&, const QString&, const QString&)),
             this->parentWidget()->window(), SLOT(OnAddMaxModelSlot(const QString&, const QString&, const QString&, const QString&)) );
}

AddMaxModelDlg::~AddMaxModelDlg()
{
    delete ui;
}

void AddMaxModelDlg::OnSetComboBoxModelTypeDataSlot(const QString &text, const bool &isParent)
{
    if(isParent)
    {
        //ui->comboBox_modelType->addSeparator();
        ui->comboBox_modelType->addParentItem(text);

    }
    else
    {
        ui->comboBox_modelType->addChildItem(text);
    }
}

void AddMaxModelDlg::OnSelMaxModelFileSlot()
{
    QString filename = QFileDialog::getOpenFileName(
       this,
       "Open 3ds Max File",
       QDir::currentPath(),
       "3ds Max(*.max *.MAX);;All files(*.*)");
    if (!filename.isNull())
    {
        ui->lineEdit_modelFile->setText(filename);
        m_maxFilename = filename;
    }
}

// ok and cancel button box event
void AddMaxModelDlg::OnButtonBoxSlot(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        QFile file(m_maxFilename);
        if( !m_maxFilename.isNull() &&  file.exists() )
        {
            if(m_thumbFilename.isNull())
            {
                m_thumbFilename = ":/images/nopic.jpg";
            }
            QString temp = ui->comboBox_modelType->currentText();
            const QString modelType = temp.remove(QChar(' '), Qt::CaseSensitive);
            const QString maxFilename = m_maxFilename;
            const QString thumbFilename = m_thumbFilename;
            const QString& itemText = ui->lineEdit_modelName->text();
            if(itemText.isEmpty())
            {
                QMessageBox::information(this, tr("提示"), tr("请输入\"模型名称\""), QMessageBox::Ok);
                return;
            }
            emit addMaxModelSignal(modelType, maxFilename, thumbFilename, itemText);
        }

    }

    QDialog::close();
    QDialog::destroy(true,true);
}

bool AddMaxModelDlg::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->label_modelThumb)
    {
        if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if( mouseEvent->button() == Qt::LeftButton)
            {
                SetModelThumbnail();
                return true;
            }
            return false;
        }
        else
            return false;
    }

    // pass the event on to the parent class
    return AddMaxModelDlg::eventFilter(obj, event);
}

//set label thumbnail for max model file.
void AddMaxModelDlg::SetModelThumbnail()
{
    QString filename = QFileDialog::getOpenFileName(
       this,
       "Open Picture",
       QDir::currentPath(),
       "Thumbnail files (*.jpg *.png *.bmp *.gif);;All files(*.*)");
    if (!filename.isEmpty())
    {
        ui->label_modelThumb->setPixmap(QPixmap(filename));
        m_thumbFilename = filename;
    }
}

void AddMaxModelDlg::dragEnterEvent(QDragEnterEvent *event)
{
    QRect labelRect = QRect( ui->label_modelThumb->pos() + ui->splitter_1->pos(), ui->label_modelThumb->size() );
    if( !labelRect.contains(event->pos()) )
    {
        event->ignore();
        return;
    }
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        QString localFile;
        QRegExp rx("\\.(jpg|png|bmp|gif)$", Qt::CaseInsensitive);
        foreach(QUrl url, event->mimeData()->urls())
        {
            localFile = url.toLocalFile();
            if( rx.indexIn(localFile) >= 0 )
            {
                event->acceptProposedAction();
                return;
            }
            else
                event->ignore();
        }
    }
}

void AddMaxModelDlg::dropEvent(QDropEvent *event)
{
    if( event->mimeData()->hasUrls() )
    {
        QRegExp rx("\\.(jpg|png|bmp|gif)$", Qt::CaseInsensitive);
        QString localFile;
        foreach(QUrl url, event->mimeData()->urls())
        {
            localFile = url.toLocalFile();
            if( rx.indexIn(localFile) >= 0 )
            {
                event->accept();
                event->setDropAction(Qt::CopyAction);
                //set label picture
                QFileInfo fileInfo(localFile);
                QString fullFilename = fileInfo.absoluteFilePath();
                ui->label_modelThumb->setPixmap( QPixmap(fullFilename) );
                m_thumbFilename = fullFilename;
            }
            else
                qDebug() << "not picture file.";
        }
    }
}

