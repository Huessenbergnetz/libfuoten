#ifndef FUOTENFEED_P_H
#define FUOTENFEED_P_H

#include "feed.h"
#include "baseitem_p.h"

namespace Fuoten {

class FeedPrivate : public BaseItemPrivate
{
public:
    FeedPrivate() : BaseItemPrivate() {}

    FeedPrivate(qint64 _id, qint64 _folderId, const QString &_title, const QUrl &_url, const QUrl &_link, const QDateTime &_added, uint _unreadCount, Feed::FeedOrdering _ordering, bool _pinned, uint _updateErrorCount, const QString &_lastUpdateError, const QUrl &_faviconLink, const QString &_folderName) :
        BaseItemPrivate(_id),
        folderId(_folderId),
        title(_title),
        folderName(_folderName),
        lastUpdateError(_lastUpdateError),
        url(_url),
        link(_link),
        faviconLink(_faviconLink),
        added(_added),
        unreadCount(_unreadCount),
        updateErrorCount(_updateErrorCount),
        ordering(_ordering),
        pinned(_pinned)
    {}

    qint64 folderId = 0;
    QString title;
    QString folderName;
    QString lastUpdateError;
    QUrl url;
    QUrl link;
    QUrl faviconLink;
    QDateTime added;
    uint unreadCount = 0;
    uint updateErrorCount = 0;
    Feed::FeedOrdering ordering = Feed::NoSpecial;
    bool pinned = false;

private:
    Q_DISABLE_COPY(FeedPrivate)
};

}

#endif // FUOTENFEED_P_H
