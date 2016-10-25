/****************************************************************************

 - Codename: CHER-Ob (Yale Computer Graphics Group)

 - Writers:  Weiqi Shi (weiqi.shi@yale.edu)

 - License:  GNU General Public License Usage
   Alternatively, this file may be used under the terms of the GNU General
   Public License version 3.0 as published by the Free Software Foundation
   and appearing in the file LICENSE.GPL included in the packaging of this
   file. Please review the following information to ensure the GNU General
   Public License version 3.0 requirements will be met:
   http://www.gnu.org/copyleft/gpl.html.

 - Warranty: This software is distributed WITHOUT ANY WARRANTY; without even
   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
   PURPOSE.

 - Acknowledgments: Some portions of this file are based on the example codes
   of ITK/VTK library from Kitware, QT API from Nokia. I would like to thank
   anonymous help from various software engineering communities.

   This file defines the different classes of bookmark that the user may work
   with (corresponding to the different file types that may be displayed).

*****************************************************************************/

#ifndef HTMLDELEGATE_H
#define HTMLDELEGATE_H

#include <QStyledItemDelegate>
#include <QtScript>
#include <QPainter>
#include <QWidget>
#include <QTextDocument>
#include <QRect>
#include <QObject>
#include <QAbstractTextDocumentLayout>

/**
 * This class is designed to render using html delegate. It is used in report generation, highlighting search result etc..
 */
class HtmlDelegate : public QStyledItemDelegate
{
protected:
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif