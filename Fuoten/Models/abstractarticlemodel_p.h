#ifndef FUOTENABSTRACTARTICLEMODEL_P_H
#define FUOTENABSTRACTARTICLEMODEL_P_H

#include "abstractarticlemodel.h"
#include "basemodel_p.h"
#include "../article.h"

namespace Fuoten {

class AbstractArticleModelPrivate : public BaseModelPrivate
{
public:
    AbstractArticleModelPrivate() :
        BaseModelPrivate(),
        parentIdType(FuotenEnums::All)
    {}

    ~AbstractArticleModelPrivate() {
        while (!articles.isEmpty()) {
            Article *a = articles.takeFirst();
            if (!a->inOperation()) {
                delete a;
            }
        }
    }


    int rowByID(qint64 id) {
        if (articles.isEmpty()) {
            return -1;
        }

        int idx = -1;

        for (int i = 0; i < articles.count(); ++i) {
            if (articles.at(i)->id() == id) {
                idx = i;
                break;
            }
        }

        return idx;
    }

    FuotenEnums::Type parentIdType;
    QList<Article*> articles;

private:
    Q_DISABLE_COPY(AbstractArticleModelPrivate)
};

}

#endif // FUOTENABSTRACTARTICLEMODEL_P_H
