#ifndef FUOTENFEED_P_H
#define FUOTENFEED_P_H

#include "feed.h"
#include "baseitem_p.h"

namespace Fuoten {

class FeedPrivate : public BaseItemPrivate
{
public:
    FeedPrivate() :
        BaseItemPrivate(),
        folderId(0),
        unreadCount(0),
        ordering(Feed::NoSpecial),
        pinned(false),
        updateErrorCount(0)
    {}

    FeedPrivate(qint64 _id, qint64 _folderId, const QString &_title, const QUrl &_url, const QUrl &_link, const QDateTime &_added, uint _unreadCount, Feed::FeedOrdering _ordering, bool _pinned, uint _updateErrorCount, const QString &_lastUpdateError, const QUrl &_faviconLink, const QString &_folderName) :
        BaseItemPrivate(_id),
        folderId(_folderId),
        title(_title),
        url(_url),
        link(_link),
        added(_added),
        unreadCount(_unreadCount),
        ordering(_ordering),
        pinned(_pinned),
        updateErrorCount(_updateErrorCount),
        lastUpdateError(_lastUpdateError),
        faviconLink(_faviconLink),
        folderName(_folderName)
    {}

    qint64 folderId;
    QString title;
    QUrl url;
    QUrl link;
    QDateTime added;
    uint unreadCount;
    Feed::FeedOrdering ordering;
    bool pinned;
    uint updateErrorCount;
    QString lastUpdateError;
    QUrl faviconLink;
    QString folderName;

private:
    Q_DISABLE_COPY(FeedPrivate)
};

}

#endif // FUOTENFEED_P_H
