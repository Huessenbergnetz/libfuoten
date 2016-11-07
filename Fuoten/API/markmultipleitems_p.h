#ifndef FUOTENMARKMULTIPLEITEMS_P_H
#define FUOTENMARKMULTIPLEITEMS_P_H

#include "markmultipleitems.h"
#include "component_p.h"

namespace Fuoten {


class MarkMultipleItemsPrivate : public ComponentPrivate
{
public:
    MarkMultipleItemsPrivate() :
        ComponentPrivate(),
        unread(false)
    {
        expectedJSONType = Component::Empty;
        namOperation = QNetworkAccessManager::PutOperation;
    }

    MarkMultipleItemsPrivate(const IdList &nItemIds, bool nUnread) :
        ComponentPrivate(),
        itemIds(nItemIds),
        unread(nUnread)
    {
        expectedJSONType = Component::Empty;
        namOperation = QNetworkAccessManager::PutOperation;
    }

    IdList itemIds;
    bool unread;
};

}

#endif // FUOTENMARKMULTIPLEITEMS_P_H
