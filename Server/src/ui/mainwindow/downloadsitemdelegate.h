#ifndef DOWNLOADSITEMDELEGATE_H
#define DOWNLOADSITEMDELEGATE_H

#include <QStyledItemDelegate>

class QLabel;

class DownloadsItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DownloadsItemDelegate(QObject *parent = 0);
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const;

private slots:
    void onEditingFinished();
};

class CaptchaWidget : public QWidget
{
    Q_OBJECT
public:
    CaptchaWidget(const QPixmap &pm, QWidget *parent);
    ~CaptchaWidget();
    QString text() const;
    void showEvent(QShowEvent*);

signals:
    void editingFinished();

private:
    QLineEdit *lineEdit;
    QPixmap m_pixmap;
    QLabel *m_label;
};

#endif // DOWNLOADSITEMDELEGATE_H
