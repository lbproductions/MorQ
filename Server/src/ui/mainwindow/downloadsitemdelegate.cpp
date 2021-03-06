#include "downloadsitemdelegate.h"

#include "model/downloadsitemmodel.h"
#include "mainwindow.h"

#include "model/downloadpackage.h"
#include "model/download.h"

#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QBrush>
#include <QApplication>
#include <QPaintEvent>

DownloadsItemDelegate::DownloadsItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void DownloadsItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{  
    painter->save();
    QStyleOptionViewItem op = option;
    initStyleOption(&op, index);

    if (index.column() == DownloadsItemModel::UserInputColumn) {
        QPixmap pixmap = index.data().value<QPixmap>();
        if (pixmap.isNull()) {
            QStyledItemDelegate::paint(painter, option, index);
            painter->restore();
            return;
        }

        QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, NULL);
        painter->drawPixmap(option.rect, pixmap);
        painter->restore();
        return;
    }
    else if (index.column() == DownloadsItemModel::ProgressColumn) {
        QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, NULL);

        int progress = 0;

        QStyleOptionProgressBar progressBarOption;
        progressBarOption.maximum = 100;

        if (!index.parent().isValid()) {
            QSharedPointer<DownloadPackage> package = index.data(Qt::UserRole + 1).value<QSharedPointer<DownloadPackage> >();
            progress = package->progress() * 100;
        }
        else {
            QSharedPointer<Download> download = index.data(Qt::UserRole + 1).value<QSharedPointer<Download> >();
            progress = download->downloadProgress() * 100;

            // Show indetermined progressbar
            if (download->isExtracting())
                progressBarOption.maximum = 0;
        }

        QPoint topLeft = option.rect.topLeft() + QPoint(5, option.rect.height() / 2 - 10);
        QPoint bottomRight = option.rect.bottomRight() - QPoint(5, 0);

        progressBarOption.rect = QRect(topLeft, bottomRight);
        progressBarOption.minimum = 0;
        progressBarOption.progress = progress;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                           &progressBarOption, painter);

        painter->drawText(option.rect, Qt::AlignCenter, QString("%1 %").arg(progress));
        painter->restore();

        return;
    }

    painter->restore();

    QStyledItemDelegate::paint(painter, option, index);
}

QWidget *DownloadsItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() != DownloadsItemModel::UserInputColumn)
        return QStyledItemDelegate::createEditor(parent, option, index);

    QPixmap pixmap = index.data().value<QPixmap>();
    if (pixmap.isNull())
        return QStyledItemDelegate::createEditor(parent, option, index);

    CaptchaWidget *editor = new CaptchaWidget(pixmap, parent);
    connect(editor, &CaptchaWidget::editingFinished, this, &DownloadsItemDelegate::onEditingFinished);
    return editor;
}

void DownloadsItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(model);

    CaptchaWidget *widget = static_cast<CaptchaWidget *>(editor);
    QSharedPointer<DownloadPackage> package = index.data(Qt::UserRole + 1).value<QSharedPointer<DownloadPackage> >();
    package->setCaptchaString(widget->text());
}

void DownloadsItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(index);
}

QSize DownloadsItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(32);
    return size;
}

void DownloadsItemDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    if (index.parent().isValid()) {
        QSharedPointer<Download> download = index.data(Qt::UserRole + 1).value<QSharedPointer<Download> >();
        if (!download->isEnabled()) {
            option->state = option->state & ~QStyle::State_Enabled;
        }
    }
}

void DownloadsItemDelegate::onEditingFinished()
{
    CaptchaWidget *editor = qobject_cast<CaptchaWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor, QAbstractItemDelegate::EditNextItem);
}


CaptchaWidget::CaptchaWidget(const QPixmap &pm, QWidget *parent) :
    QWidget(parent),
    m_pixmap(pm)
{
    QLayout *l = new QVBoxLayout;
    setLayout(l);

    lineEdit = new QLineEdit(this);
    l->addWidget(lineEdit);
    l->setContentsMargins(0,0,0,0);
    setFocusProxy(lineEdit);
    setFocusPolicy(Qt::StrongFocus);

    connect(lineEdit, &QLineEdit::editingFinished, this, &CaptchaWidget::editingFinished);

    m_label = new QLabel();
    m_label->setPixmap(pm);
    m_label->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint);
}

CaptchaWidget::~CaptchaWidget()
{
    delete m_label;
}

QString CaptchaWidget::text() const
{
    return lineEdit->text();
}

void CaptchaWidget::showEvent(QShowEvent* e)
{
    QPoint p = mapToGlobal(QPoint(0,lineEdit->height() + 5));
    m_label->show();
    m_label->move(p);

    QWidget::showEvent(e);
    lineEdit->activateWindow();
    lineEdit->grabKeyboard();
}
