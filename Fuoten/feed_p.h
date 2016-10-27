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

    QUrl url;
    QString title;
    QDateTime added;
    qint64 folderId;
    QString folderName;
    uint unreadCount;
    Feed::FeedOrdering ordering;
    QUrl link;
    bool pinned;
    uint updateErrorCount;
    QString lastUpdateError;
    QUrl faviconLink;
};

}

#endif // FUOTENFEED_P_H
