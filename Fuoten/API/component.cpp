/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "component_p.h"
#include "../error.h"
#include <QJsonParseError>
#include <QUrl>
#include <QReadWriteLock>
#include <QGlobalStatic>

using namespace Fuoten;

/*!
 * \internal
 * \brief Stores global default values
 */
class DefaultValues
{
public:
    mutable QReadWriteLock lock;

    AbstractConfiguration *configuration() const
    {
        return m_defaultConfig;
    }

    void setConfiguration(AbstractConfiguration *config)
    {
        m_defaultConfig = config;
    }

    AbstractStorage *storage() const
    {
        return m_defaultStorage;
    }

    void setStorage(AbstractStorage *storage)
    {
        m_defaultStorage = storage;
    }

    AbstractNamFactory *networkAccessManagerFactory() const
    {
        return m_namFactory;
    }

    void setNetworkAccessManagerFactory(AbstractNamFactory *factory)
    {
        m_namFactory = factory;
    }

    AbstractNotificator *notificator() const
    {
        return m_defaultNotificator;
    }

    void setNotificator(AbstractNotificator *notificator)
    {
        m_defaultNotificator = notificator;
    }

    WipeManager *wipeManager() const
    {
        return m_defaultWipeManager;
    }

    void setWipeManager(WipeManager *wipeManager)
    {
        m_defaultWipeManager = wipeManager;
    }

    QNetworkAccessManager *defaultNam() const
    {
        return m_defaultNam;
    }

    void setDefaultNam(QNetworkAccessManager *nam)
    {
        m_defaultNam = nam;
    }

private:
    AbstractConfiguration *m_defaultConfig = nullptr;
    AbstractStorage *m_defaultStorage = nullptr;
    AbstractNamFactory *m_namFactory = nullptr;
    AbstractNotificator *m_defaultNotificator = nullptr;
    WipeManager *m_defaultWipeManager = nullptr;
    QNetworkAccessManager *m_defaultNam = nullptr;
};
Q_GLOBAL_STATIC(DefaultValues, defVals)


ComponentPrivate::ComponentPrivate()
{

}


ComponentPrivate::~ComponentPrivate()
{

}


void ComponentPrivate::performNetworkOperation(const QNetworkRequest &request)
{
    switch(namOperation) {
    case QNetworkAccessManager::HeadOperation:
        reply = networkAccessManager->head(request);
        qDebug("%s", "Performing HEAD network operation.");
        break;
    case QNetworkAccessManager::PostOperation:
        reply = networkAccessManager->post(request, payload);
        qDebug("%s", "Performing POST network operation.");
        break;
    case QNetworkAccessManager::PutOperation:
        reply = networkAccessManager->put(request, payload);
        qDebug("%s", "Performing PUT network operation.");
        break;
    case QNetworkAccessManager::DeleteOperation:
        reply = networkAccessManager->deleteResource(request);
        qDebug("%s", "Performing DELETE network operation");
        break;
    default:
        reply = networkAccessManager->get(request);
        qDebug("%s", "Performing GET network operation.");
        break;
    }
}


AbstractConfiguration *ComponentPrivate::defaultConfiguration()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractConfiguration *config = defs->configuration();
    defs->lock.unlock();

    return config;
}


void ComponentPrivate::setDefaultConfiguration(AbstractConfiguration *config)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);
    QWriteLocker locker(&defs->lock);
    qDebug("%s", "Setting default configuration.");
    defs->setConfiguration(config);
}


AbstractStorage *ComponentPrivate::defaultStorage()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractStorage *storage = defs->storage();
    defs->lock.unlock();

    return storage;
}


void ComponentPrivate::setDefaultStorage(AbstractStorage *storage)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);
    QWriteLocker locker(&defs->lock);
    qDebug("%s", "Setting default storage.");
    defs->setStorage(storage);
}


AbstractNamFactory *ComponentPrivate::networkAccessManagerFactory()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractNamFactory *nam = defs->networkAccessManagerFactory();
    defs->lock.unlock();

    return nam;
}


void ComponentPrivate::setNetworkAccessManagerFactory(AbstractNamFactory *factory)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);
    QWriteLocker locker(&defs->lock);
    qDebug("%s", "Setting network access manager factory.");
    defs->setNetworkAccessManagerFactory(factory);
}


AbstractNotificator *ComponentPrivate::defaultNotificator()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractNotificator *noti = defs->notificator();
    defs->lock.unlock();

    return noti;
}


void ComponentPrivate::setDefaultNotificator(AbstractNotificator *notificator)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);
    QWriteLocker locker(&defs->lock);
    qDebug("%s", "Setting default notificator.");
    defs->setNotificator(notificator);
}


WipeManager *ComponentPrivate::defaultWipeManager()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    WipeManager *wm = defs->wipeManager();
    defs->lock.unlock();

    return wm;
}


void ComponentPrivate::setDefaultWipeManager(WipeManager *wipeManager)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);
    QWriteLocker locker(&defs->lock);
    qDebug("%s", "Setting default wipe manager.");
    defs->setWipeManager(wipeManager);
}


QNetworkAccessManager *ComponentPrivate::defaultNam()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    QReadLocker locker(&defs->lock);
    return defs->defaultNam();
}


void ComponentPrivate::setDefaultNam(QNetworkAccessManager *nam)
{
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);
    QWriteLocker locker(&defs->lock);
    qDebug("%s", "Setting default QNetworkAccessManager.");
    defs->setDefaultNam(nam);
}


Component::Component(QObject *parent) :
    QObject(parent), d_ptr(new ComponentPrivate)
{
    connect(this, &Component::failed, this, [this](Error *e){notify(e);});
}


Component::Component(ComponentPrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{
    connect(this, &Component::failed, this, [this](Error *e){notify(e);});
}


Component::~Component()
{

}


void Component::sendRequest()
{
    Q_D(Component);

    if (!d->configuration) {
        d->configuration = ComponentPrivate::defaultConfiguration();
        if (d->configuration) {
            Q_EMIT configurationChanged(d->configuration);
        }
    }

    Q_ASSERT_X(d->configuration, "send request", "no configuration available");
    Q_ASSERT_X(!d->configuration->getHost().isEmpty(), "send request", "empty host name");
    Q_ASSERT_X(!d->apiRoute.isEmpty(), "send request", "empty API route");

    setError(nullptr);

    d->result.clear();
    d->jsonResult = QJsonDocument();

    if (Q_UNLIKELY(!checkInput())) {
        setInOperation(false);
        return;
    }

    QUrl url;

    if (d->configuration->getUseSSL()) {
        url.setScheme(QStringLiteral("https"));
    } else {
        url.setScheme(QStringLiteral("http"));
    }

    if (d->configuration->getServerPort() != 0) {
        url.setPort(d->configuration->getServerPort());
    }

    url.setHost(d->configuration->getHost());

    QString urlPath = d->configuration->getInstallPath();
    urlPath.append(d->baseRoute);
    urlPath.append(d->apiRoute);

    url.setPath(urlPath);

    if (!d->urlQuery.isEmpty()) {
        url.setQuery(d->urlQuery);
    }

    Q_ASSERT_X(url.isValid(), "send request", "invalid API URL");

    if (!d->networkAccessManager) {
        AbstractNamFactory *namf = Component::networkAccessManagerFactory();
        if (namf) {
            d->networkAccessManager = namf->create(this);
        } else {
            d->networkAccessManager = Component::defaultNam();
            if (!d->networkAccessManager) {
                d->networkAccessManager = new QNetworkAccessManager(this);
            }
        }
        if (d->configuration->getIgnoreSSLErrors()) {
            connect(d->networkAccessManager, &QNetworkAccessManager::sslErrors, this, &Component::_ignoreSSLErrors);
        }
    }

    QNetworkRequest nr(url);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    if (Q_LIKELY(d->requestTimeout > 0)) {
        nr.setTransferTimeout(static_cast<int>(d->requestTimeout) * 1000);
    }
#endif

    nr.setRawHeader(QByteArrayLiteral("User-Agent"), d->configuration->getUserAgent().toUtf8());

    if (!d->requestHeaders.isEmpty()) {
        QHash<QByteArray, QByteArray>::const_iterator i = d->requestHeaders.constBegin();
        while (i != d->requestHeaders.constEnd()) {
            nr.setRawHeader(i.key(), i.value());
            ++i;
        }
    }

    if (!d->payload.isEmpty()) {
        nr.setRawHeader(QByteArrayLiteral("Content-Length"), QByteArray::number(d->payload.length()));
        nr.setRawHeader(QByteArrayLiteral("Content-Type"), d->payloadContentType);
    }

    if (d->expectedJSONType != Empty) {
        nr.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("application/json"));
    }

    if (d->requiresAuth) {
        QByteArray authHeader = QByteArrayLiteral("Basic ");
        QString auth(d->configuration->getUsername());
        auth.append(QLatin1String(":")).append(d->configuration->getPassword());
        authHeader.append(auth.toUtf8().toBase64());
        nr.setRawHeader(QByteArrayLiteral("Authorization"), authHeader);
    }

#ifdef QT_DEBUG
    qDebug("Start performing network operation.");
    qDebug("API URL: %s", qUtf8Printable(url.toString()));
    if (!nr.rawHeaderList().empty()) {
        const QList<QByteArray> hl = nr.rawHeaderList();
        for (const QByteArray &h : hl) {
            if (h != QByteArrayLiteral("Authorization")) {
                qDebug("%s: %s", h.constData(), nr.rawHeader(h).constData());
            }
        }
    }
    if (!d->payload.isEmpty()) {
        qDebug("Payload: %s", d->payload.constData());
    }
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    if (Q_LIKELY(d->requestTimeout > 0)) {
        if (!d->timeoutTimer) {
            d->timeoutTimer = new QTimer(this);
            qDebug("%s", "Created new timeout timer.");
            d->timeoutTimer->setSingleShot(true);
            d->timeoutTimer->setTimerType(Qt::VeryCoarseTimer);
            connect(d->timeoutTimer, &QTimer::timeout, this, &Component::_requestTimedOut);
        }
        d->timeoutTimer->start(static_cast<int>(d->requestTimeout) * 1000);
        qDebug("Started timeout timer with %hu seconds.", d->requestTimeout);
    }
#endif

    d->performNetworkOperation(nr);
    Q_CHECK_PTR(d->reply);
    if (!connect(d->reply, &QNetworkReply::finished, this, &Component::_requestFinished)) {
        qFatal("Failed to connect QNetworkReply to Component::_requestFinished slot.");
    }
}


void Component::_requestFinished()
{
    Q_D(Component);

    qDebug("%s", "Finished network operation.");
#ifdef QT_DEBUG
    qDebug("API URL: %s", qUtf8Printable(d->reply->url().toString()));
    qDebug("Reply size: %lli", d->reply->size());
    const QList<QByteArray> hl = d->reply->rawHeaderList();
    for (const QByteArray &h : hl) {
        qDebug("%s: %s", h.constData(), d->reply->rawHeader(h).constData());
    }
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    if (Q_LIKELY(d->timeoutTimer && d->timeoutTimer->isActive())) {
        qDebug("Stopping timeout timer with %i seconds left.", d->timeoutTimer->remainingTime()/1000);
        d->timeoutTimer->stop();
    }
#endif

    if (Q_LIKELY(d->reply->error() == QNetworkReply::NoError)) {


        qDebug("%s", "Reading network reply data.");

        d->result = d->reply->readAll();

        if (checkOutput()) {
            qDebug("%s", "Calling successCallback().");
            successCallback();
        } else {
            setInOperation(false);
        }

    } else {
        qDebug("%s", "Extracting error data from network reply.");
        extractError(d->reply);
        setInOperation(false);
    }

    d->reply->deleteLater();
    d->reply = nullptr;
}


void Component::extractError(QNetworkReply *reply)
{
    Q_ASSERT_X(reply, "extract error", "invalid QNetworkReply");

    Q_D(Component);

    const int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (d->checkForWipe && wipeManager() && (httpStatusCode == 401 || httpStatusCode == 403)) {
        wipeManager()->checkForWipe();
    }

    setError(new Error(reply, this));

    Q_EMIT failed(error());
}


#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
void Component::_requestTimedOut()
{
    Q_D(Component);

    //% "The connection to the server timed out after %n second(s)."
    setError(new Error(Error::RequestError, Error::Critical, qtTrId("err-conn-timeout", requestTimeout()), d->reply->request().url().toString(), this));

    setInOperation(false);

    QNetworkReply *nr = d->reply;
    d->reply = nullptr;
    delete nr;
    Q_EMIT failed(error());
}
#endif


void Component::_ignoreSSLErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    Q_UNUSED(errors);
    if (reply) {
        reply->ignoreSslErrors();
    }
}


bool Component::checkInput()
{
    Q_D(Component);

    if (Q_UNLIKELY(d->requiresAuth && (d->configuration->getUsername().isEmpty() || d->configuration->getPassword().isEmpty()))) {
        //% "You have to specify a username and a password."
        setError(new Error(Error::AuthorizationError, Error::Critical, qtTrId("err-username-pass-missing"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    if ((d->namOperation == QNetworkAccessManager::PostOperation || d->namOperation == QNetworkAccessManager::PutOperation) && d->payload.isEmpty()) {
        //% "Empty payload when trying to perform a PUT or POST network operation."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("err-no-payloud"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    return true;
}


bool Component::checkOutput()
{
    Q_D(Component);

    if (d->expectedJSONType != Empty) {
        QJsonParseError jsonError;
        d->jsonResult = QJsonDocument::fromJson(d->result, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            setError(new Error(jsonError, this));
            Q_EMIT failed(error());
            return false;
        }
    }

    if (Q_UNLIKELY((d->expectedJSONType != Empty) && (d->jsonResult.isNull() || d->jsonResult.isEmpty()))) {
        //% "The request replied an empty answer, but there was content expected."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-empty-answer"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    if (Q_UNLIKELY((d->expectedJSONType == Array) && !d->jsonResult.isArray())) {
        //% "It was expected that the request returns a JSON array, but it returned something else."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-no-json-array"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    if (Q_UNLIKELY((d->expectedJSONType == Object && !d->jsonResult.isObject()))) {
        //% "It was expected that the request returns a JSON object, but it returned something else."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-no-json-object"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    return true;
}


bool Component::inOperation() const { Q_D(const Component); return d->inOperation; }

void Component::setInOperation(bool nInOperation)
{
    Q_D(Component);
    if (nInOperation != d->inOperation) {
        d->inOperation = nInOperation;
        qDebug("Changed inOperation to %s.", d->inOperation ? "true" : "false");
        Q_EMIT inOperationChanged(inOperation());
    }
}


quint16 Component::requestTimeout() const { Q_D(const Component); return d->requestTimeout; }

void Component::setRequestTimeout(quint16 seconds)
{
    Q_D(Component);
    if (seconds != d->requestTimeout) {
        d->requestTimeout = seconds;
        qDebug("Changed requestTimeout to %hu seconds.", d->requestTimeout);
        Q_EMIT requestTimeoutChanged(requestTimeout());
    }
}


Error *Component::error() const { Q_D(const Component); return d->error; }

void Component::setError(Error *nError)
{
    Q_D(Component);
    if (nError != d->error) {
        Error *oldError = d->error;
        d->error = nError;
        if (oldError) {
            delete oldError;
        }
        qDebug("Changed error to %p.", d->error);
        Q_EMIT errorChanged(error());
    }
}


AbstractConfiguration *Component::configuration() const
{
    Q_D(const Component);
    AbstractConfiguration *_config = d->configuration;
    if (!_config) {
        _config = ComponentPrivate::defaultConfiguration();
    }
    Q_ASSERT(_config);
    return _config;
}


void Component::setConfiguration(AbstractConfiguration *nAbstractConfiguration)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "configuration");
        return;
    }

    Q_D(Component);
    if (nAbstractConfiguration != d->configuration) {
        d->configuration = nAbstractConfiguration;
        qDebug("Changed configuration to %p.", d->configuration);
        Q_EMIT configurationChanged(configuration());
    }
}


AbstractStorage *Component::storage() const
{
    Q_D(const Component);
    AbstractStorage *_storage = d->storage;
    if (!_storage) {
        _storage = ComponentPrivate::defaultStorage();
    }
    return _storage;
}

void Component::setStorage(AbstractStorage *localStorage)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "storage");
        return;
    }

    Q_D(Component);
    if (localStorage != d->storage) {
        d->storage = localStorage;
        qDebug("Changed storage to %p.", d->storage);
        Q_EMIT storageChanged(storage());
    }
}


bool Component::isUseStorageEnabled() const { Q_D(const Component); return d->useStorage; }

void Component::setUseStorage(bool useStorage)
{
    Q_D(Component);
    if (useStorage != d->useStorage) {
        d->useStorage = useStorage;
        qDebug("Changed useStorage to %s.", d->useStorage ? "true" : "false");
        Q_EMIT useStorageChanged(d->useStorage);
    }
}


AbstractNotificator *Component::notificator() const
{
    Q_D(const Component);
    AbstractNotificator *_notificator = d->notificator;
    if (!_notificator) {
        _notificator = ComponentPrivate::defaultNotificator();
    }
    return _notificator;
}

void Component::setNotificator(AbstractNotificator *notificator)
{
    Q_D(Component);
    if (notificator != d->notificator) {
        d->notificator = notificator;
        qDebug("Changed notificator to %p.", d->notificator);
        Q_EMIT notificatorChanged(d->notificator);
    }
}

WipeManager *Component::wipeManager() const
{
    Q_D(const Component);
    WipeManager *_wipeManager = d->wipeManager;
    if (!_wipeManager) {
        _wipeManager = ComponentPrivate::defaultWipeManager();
    }
    return _wipeManager;
}

void Component::setWipeManager(WipeManager *wipeManager)
{
    Q_D(Component);
    if (wipeManager != d->wipeManager) {
        d->wipeManager = wipeManager;
        qDebug("%s", "Changed wipeManager.");
        Q_EMIT wipeManagerChanged(d->wipeManager);
    }
}

void Component::setDefaultConfiguration(AbstractConfiguration *config)
{
    ComponentPrivate::setDefaultConfiguration(config);
}


AbstractConfiguration *Component::defaultConfiguration()
{
    return ComponentPrivate::defaultConfiguration();
}


void Component::setDefaultStorage(AbstractStorage *storage)
{
    ComponentPrivate::setDefaultStorage(storage);
}


AbstractStorage *Component::defaultStorage()
{
    return ComponentPrivate::defaultStorage();
}


void Component::setNetworkAccessManagerFactory(AbstractNamFactory *factory)
{
    ComponentPrivate::setNetworkAccessManagerFactory(factory);
}


AbstractNamFactory *Component::networkAccessManagerFactory()
{
    return ComponentPrivate::networkAccessManagerFactory();
}


void Component::setDefaultNotificator(AbstractNotificator *notificator)
{
    ComponentPrivate::setDefaultNotificator(notificator);
}


WipeManager *Component::defaultWipeManager()
{
    return ComponentPrivate::defaultWipeManager();
}


void Component::setDefaultWipeManager(WipeManager *wipeManager)
{
    ComponentPrivate::setDefaultWipeManager(wipeManager);
}


AbstractNotificator *Component::defaultNotificator()
{
    return ComponentPrivate::defaultNotificator();
}


void Component::setDefaultNam(QNetworkAccessManager *nam)
{
    ComponentPrivate::setDefaultNam(nam);
}


QNetworkAccessManager *Component::defaultNam()
{
    return ComponentPrivate::defaultNam();
}


void Component::setExpectedJSONType(ExpectedJSONType type)
{
    Q_D(Component);
    d->expectedJSONType = type;
}


void Component::setApiRoute(const QString &route)
{
    Q_D(Component);
    d->apiRoute = route;
}


void Component::setApiRoute(const QStringList &routeParts)
{
    Q_D(Component);
    d->apiRoute = QStringLiteral("/").append(routeParts.join(QLatin1Char('/')));
}


QJsonDocument Component::jsonResult() const
{
    Q_D(const Component);
    return d->jsonResult;
}


void Component::setNetworkOperation(QNetworkAccessManager::Operation operation)
{
    Q_D(Component);
    d->namOperation = operation;
}


QHash<QByteArray, QByteArray> Component::requestHeaders() const
{
    Q_D(const Component);
    return d->requestHeaders;
}


void Component::setRequestHeaders(const QHash<QByteArray, QByteArray> &headers)
{
    Q_D(Component);
    d->requestHeaders = headers;
}


void Component::addRequestHeader(const QByteArray &headerName, const QByteArray &headerValue)
{
    if (headerName.isEmpty() || headerValue.isEmpty()) {
        return;
    }

    Q_D(Component);
    d->requestHeaders.insert(headerName, headerValue);
}


void Component::addRequestHeaders(const QHash<QByteArray, QByteArray> &headers)
{
    if (headers.isEmpty()) {
        return;
    }

    Q_D(Component);

    QHash<QByteArray, QByteArray>::const_iterator i = headers.constBegin();
    while (i != headers.constEnd()) {
        d->requestHeaders.insert(i.key(), i.value());
        ++i;
    }
}


void Component::setPayload(const QByteArray &payload)
{
    Q_D(Component);
    d->payload = payload;
}


void Component::setPayload(const QJsonObject &payload)
{
    Q_D(Component);
    d->payload = QJsonDocument(payload).toJson(QJsonDocument::Compact);
}


void Component::setUrlQuery(const QUrlQuery &query)
{
    Q_D(Component);
    d->urlQuery = query;
}


void Component::setRequiresAuth(bool reqAuth)
{
    Q_D(Component);
    d->requiresAuth = reqAuth;
}


void Component::notify(const Fuoten::Error *e) const
{
    auto n = notificator();
    if (n) {
        n->notify(e);
    }
}


void Component::notify(AbstractNotificator::Type type, QtMsgType severity, const QVariant &data) const
{
    auto n = notificator();
    if (n) {
        n->notify(type, severity, data);
    }
}

#include "moc_component.cpp"
