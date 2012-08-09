/****************************************************************************
 * This file is part of Preferences.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Preferences is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Preferences is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Preferences.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QApplication>
#include <QPainter>
#include <QPixmap>
#include <QTextDocument>
#include <QDesktopWidget>

#include "backgrounditemdelegate.h"
#include "backgroundsmodel.h"

const int kScreenshotSize = 128;
const int kMargin = 6;

BackgroundItemDelegate::BackgroundItemDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
    // Background previews respect screen aspect ratio
    QSize screenSize = qApp->desktop()->size();
    qreal ratio = qreal(screenSize.width() / screenSize.height());
    m_maxSize = QSize(kScreenshotSize, kScreenshotSize / ratio);
}

void BackgroundItemDelegate::paint(QPainter *painter,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    const QString title = index.model()->data(index, Qt::DisplayRole).toString();
    const QString author = index.model()->data(index, BackgroundsModel::AuthorRole).toString();
    const QString resolution = index.model()->data(index, BackgroundsModel::ResolutionRole).toString();
    const QPixmap pix = index.model()->data(index, Qt::DecorationRole).value<QPixmap>();

    // Highlight selected item
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &option, painter);

    // Draw wallpaper thumbnail
    if (pix.isNull())
        painter->fillRect(option.rect, option.palette.brush(QPalette::Base));
    else
        // Draw the actual thumbnail
        painter->drawPixmap(option.rect.center().x() - (m_maxSize.width() / 2),
                            option.rect.y() + kMargin, m_maxSize.width(),
                            m_maxSize.height(), pix);

    // Use a QTextDocument to layout the text
    QTextDocument document;
    QString html = QString("<strong>%1</strong>").arg(title);

    if (!author.isEmpty()) {
        QString authorCaption = tr("by %1").arg(author);
        html += QString("<br />%1").arg(authorCaption);
    }

    if (!resolution.isEmpty())
        html += QString("<br /><em>%1</em>").arg(resolution);

    // Set the text color according to the item state
    QColor color;
    if (option.state & QStyle::State_Selected)
        color = QApplication::palette().brush(QPalette::HighlightedText).color();
    else
        color = QApplication::palette().brush(QPalette::Text).color();
    html = QString("<div style=\"color: %1\" align=\"center\">%2</div>").arg(color.name()).arg(html);

    // Set contents and word-wrap
    document.setHtml(html);
    document.setTextWidth(m_maxSize.width());

    // Calculate positioning
    int x = option.rect.left() + kMargin;
    int y = option.rect.top() + m_maxSize.height() + kMargin * 2;

    // Draw text
    painter->save();
    painter->translate(x, y);
    document.drawContents(painter, QRect(QPoint(0, 0), option.rect.size() -
                                         QSize(0, m_maxSize.height() + kMargin * 2)));
    painter->restore();
}

QSize BackgroundItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    Q_UNUSED(option)

    const QString title = index.model()->data(index, Qt::DisplayRole).toString();
    const QString author = index.model()->data(index, BackgroundsModel::AuthorRole).toString();
    const QString resolution = index.model()->data(index, BackgroundsModel::ResolutionRole).toString();

    // Generate a sample complete entry (with the real title) to calculate sizes
    QTextDocument document;
    QString html = QString("<strong>%1</strong>").arg(title);
    if (!author.isEmpty())
        html += QString("<br />by %1").arg(author);
    if (!resolution.isEmpty())
        html += QString("<br /><em>%1</em>").arg(resolution);
    document.setHtml(html);
    document.setTextWidth(m_maxSize.width());

    QSize s(m_maxSize.width() + kMargin * 2,
            m_maxSize.height() + kMargin * 3 + (int)(document.size().height()));
    return s;
}

void BackgroundItemDelegate::resetMaxHeight()
{
    m_maxSize.setHeight(0);
}

#include "moc_backgrounditemdelegate.cpp"
