/****************************************************************************
 * This file is part of System Preferences.
 *
 * Copyright (c) 2011-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef WALLPAPERFINDER_H
#define WALLPAPERFINDER_H

#include <QThread>

class WallpaperFinder : public QThread
{
    Q_OBJECT
public:
    explicit WallpaperFinder(const QString &path, QObject *parent = 0);
    ~WallpaperFinder();

    QString path() const {
        return m_path;
    }

signals:
    void backgroundFound(const QString &wallpaperDir,
                         const QString &desktopEntry, const QString &previewImage);

protected:
    virtual void run();

private:
    QString m_path;
};

#endif // WALLPAPERFINDER_H
