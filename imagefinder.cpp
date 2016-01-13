/* ============================================================
* ImageFinder plugin for QupZilla
* Copyright (C) 2016 Vladislav Tronko <innermous@gmail.com>
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
* ============================================================ */
#include "imagefinder.h"

#include <QApplication>
#include <QSettings>
#include <QUrl>

ImageFinder::ImageFinder(const QString &settingsFile, QObject *parent)
    : QObject(parent)
    , m_settingsFile(settingsFile)
    , m_searchEngine(SearchEngine::Google)
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.beginGroup("ImageFinder");

    m_searchEngine = static_cast<SearchEngine>(settings.value("SearchEngine").toInt());

    settings.endGroup();
}

ImageFinder::SearchEngine ImageFinder::searchEngine() const
{
    return m_searchEngine;
}

void ImageFinder::setSearchEngine(ImageFinder::SearchEngine searchEngine)
{
    m_searchEngine = searchEngine;

    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.beginGroup("ImageFinder");
    settings.setValue("SearchEngine", m_searchEngine);
    settings.endGroup();
}

QString ImageFinder::searchEngineName() const
{
    QStringList searchEngines =
    {
        "Google",
        "Yandex",
        "TinEye"
    };

    return searchEngines.at(m_searchEngine);
}

QUrl ImageFinder::getSearchQuery(QUrl imageUrl)
{
    QUrl query;

    switch (m_searchEngine)
    {
    case SearchEngine::Google:
        query = QUrl("https://www.google.com/searchbyimage?site=search&image_url=" +  imageUrl.toString());
        break;

    case SearchEngine::Yandex:
        query = QUrl("https://yandex.com/images/search?&img_url=" + imageUrl.toString() + "&rpt=imageview");
        break;

    case SearchEngine::TinEye:
        query = QUrl("http://www.tineye.com/search?url=" + imageUrl.toString());
        break;

    default: break;
    }

    return query;
}
