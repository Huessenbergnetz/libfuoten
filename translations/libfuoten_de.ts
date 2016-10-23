<?xml version="1.0" ?><!DOCTYPE TS><TS language="de" sourcelanguage="en" version="2.1">
<context>
    <name/>
    <message id="invalid-api-url">
        <location filename="../src/API/component.cpp" line="+94"/>
        <source>Invalid API URL</source>
        <translation>Ungültige API-URL</translation>
    </message>
    <message id="err-conn-timeout" numerus="yes">
        <location line="+116"/>
        <source>The connection to the server timed out after %n second(s).</source>
        <translation><numerusform>Die Verbindung zum Server hat die Zeitbegrenzung von %n Sekunde überschritten.</numerusform><numerusform>Die Verbindung zum Server hat die Zeitbegrenzung von %n Sekunden überschritten.</numerusform></translation>
    </message>
    <message id="id-err-no-config">
        <location line="+24"/>
        <location filename="../src/Helpers/accountvalidator.cpp" line="+68"/>
        <source>No configuration available.</source>
        <translation>Keine Konfiguration verfügbar.</translation>
    </message>
    <message id="err-username-pass-missing">
        <location line="+7"/>
        <source>You have to specify a username and a password.</source>
        <translation>Sie müssen einen Benutzernamen und ein Passwort angeben.</translation>
    </message>
    <message id="err-no-host">
        <location line="+7"/>
        <source>No host specified</source>
        <translation>Keine Server-Adresse angegeben.</translation>
    </message>
    <message id="err-no-route">
        <location line="+7"/>
        <source>No API route specified.</source>
        <translation>Keine API-Route angegeben.</translation>
    </message>
    <message id="err-no-payloud">
        <location line="+7"/>
        <source>Empty payload when trying to perform a PUT or POST network operation.</source>
        <translation>Leerer Inhalt beim Durchführen einer PUT- oder POST-Operation.</translation>
    </message>
    <message id="err-empty-answer">
        <location line="+26"/>
        <source>The request replied an empty answer, but there was content expected.</source>
        <translation>Die Anfrage hat eine leere Antwort erhalten obwohl Inhalt erwartet wurde.</translation>
    </message>
    <message id="err-no-json-array">
        <location line="+7"/>
        <source>It was expected that the request returns a JSON array, but it returned something else.</source>
        <translation>In der Antwort wurde ein JSON-Array erwarte, aber es wurde etwas anderes geliefert.</translation>
    </message>
    <message id="err-no-json-object">
        <location line="+7"/>
        <source>It was expected that the request returns a JSON object, but it returned something else.</source>
        <translation>In der Antwort wurde ein JSON-Objekt erwartet, aber es wurde etwas anderes geliefert.</translation>
    </message>
    <message id="err-net-reply-1">
        <location filename="../src/error.cpp" line="+68"/>
        <source>The remote server at %1 refused the connection.</source>
        <translation>Der Server unter %1 hat die Verbindung verweigert.</translation>
    </message>
    <message id="err-net-reply-2">
        <location line="+4"/>
        <source>The remote server at %1 closed the connection prematurely, before the entire reply was received and processed.</source>
        <translation>Der Server unter %1 hat die Verbindung vorzeitig beendet, bevor die komplette Antwort erhalten wurde.</translation>
    </message>
    <message id="err-net-reply-3">
        <location line="+4"/>
        <source>The remote host name %1 was not found.</source>
        <translation>Der Server unter %1 wurde nicht gefunden.</translation>
    </message>
    <message id="err-net-reply-4">
        <location line="+4"/>
        <source>The connection to the server at %1 timed out.</source>
        <translation>Die Verbindung zum Server hat die Zeitbegrenzung überschritten.</translation>
    </message>
    <message id="err-net-reply-5">
        <location line="+4"/>
        <source>The operation was canceled before it was finished.</source>
        <translation>Die Operation wurde abgebrochen bevor sie fertig war.</translation>
    </message>
    <message id="err-net-reply-6">
        <location line="+4"/>
        <source>The SSL/TLS handshake failed and the encrypted channel could not be established.</source>
        <translation>Der SSL/TLS-Handschlag ist fehlgeschlagen und es konnte keine verschlüsselte Verbindung aufgebaut werden.</translation>
    </message>
    <message id="err-net-reply-7">
        <location line="+4"/>
        <source>The connection was broken due to disconnection from the network.</source>
        <translation>Die Verbindung wurde durch Abtrennen vom Netzwerk unterbrochen.</translation>
    </message>
    <message id="err-net-reply-8">
        <location line="+4"/>
        <source>The connection was broken due to disconnection from the network or failure to start the network.</source>
        <translation>Die Verbindung wurde unterbrochen weil das Gerät vom Netzwerk getrennt wurde oder das Netzwerk nicht gestartet werden konnte.</translation>
    </message>
    <message id="err-net-reply-9">
        <location line="+4"/>
        <source>The background request is not currently allowed due to platform policy.</source>
        <translation>Die Hintergrundanfrage ist auf Grund von Plattformeinschränkungen zur Zeit nicht erlaubt.</translation>
    </message>
    <message id="err-net-reply-10">
        <location line="+5"/>
        <source>While following redirects, the maximum limit was reached.</source>
        <translation>Es wurde die maximale Anzahl an Umleitungen überschritten.</translation>
    </message>
    <message id="err-net-reply-11">
        <location line="+4"/>
        <source>While following redirects, the network access API detected a redirect from an encrypted protocol (https) to an unencrypted one (http).</source>
        <translation>Beim Folgen einer Umleitung wurde eine Umleitung von einem verschlüsselten Protokoll (https) auf ein unverschlüsseltes (http) entdeckt.</translation>
    </message>
    <message id="err-net-reply-101">
        <location line="+5"/>
        <source>The connection to the proxy server was refused (the proxy server is not accepting requests).</source>
        <translation>Die Verbindung wurde vom Proxy-Server abgelehnt (der Proxy-Server akzeptiert keine Anfragen).</translation>
    </message>
    <message id="err-net-reply-102">
        <location line="+4"/>
        <source>The proxy server closed the connection prematurely, before the entire reply was received and processed.</source>
        <translation>Der Proxy-Server hat die Verbindung vorzeitig beendet, bevor die komplette Antwort empfangen werden konnte.</translation>
    </message>
    <message id="err-net-reply-103">
        <location line="+4"/>
        <source>The proxy host name was not found (invalid proxy hostname).</source>
        <translation>Der Proxy-Server konnte nicht gefunden werden (ungültiger Proxy-Host-Name).</translation>
    </message>
    <message id="err-net-reply-104">
        <location line="+4"/>
        <source>The connection to the proxy timed out or the proxy did not reply in time to the request sent</source>
        <translation>Die Verbindung zum Proxy hat die Zeitbegrenzung überschritten oder der Proxy hat nicht rechtzeitig geantwortet.</translation>
    </message>
    <message id="err-net-reply-105">
        <location line="+4"/>
        <source>The proxy requires authentication in order to honour the request but did not accept any credentials offered (if any).</source>
        <translation>Der Proxy verlangt nach einer Authentifizierung, lehnt die Anmeldeinformationen aber ab.</translation>
    </message>
    <message id="err-net-reply-201">
        <location line="+4"/>
        <source>The access to the remote content was denied.</source>
        <translation>Der Zugriff auf den Server wurde verweigert.</translation>
    </message>
    <message id="err-net-reply-202">
        <location line="+4"/>
        <source>The operation requested on the remote content is not permitted.</source>
        <translation>Die auf die entfernten Inhalte anzuwendende Operation ist nicht erlaubt.</translation>
    </message>
    <message id="err-net-reply-203">
        <location line="+4"/>
        <source>The remote content was not found at the server.</source>
        <translation>Der verlangte Inhalt wurde auf dem Server nicht gefunden.</translation>
    </message>
    <message id="err-net-reply-204">
        <location line="+4"/>
        <source>The remote server requires authentication to serve the content but the credentials provided were not accepted (if any).</source>
        <translation>Der Server verlangt für die Auslieferung der Inhalte nach einer Authentifizierung, die bereitgestellten Anmeldeinformationen wurden allerdings nicht akzeptiert.</translation>
    </message>
    <message id="err-net-reply-205">
        <location line="+4"/>
        <source>The request needed to be sent again, but this failed for example because the upload data could not be read a second time.</source>
        <translation>Die Anfrage musste noch einmal gesendet werden, was aber fehlschlug, bspw. weil die Daten kein zweites mal gelesen werden konnten.</translation>
    </message>
    <message id="err-net-reply-206">
        <location line="+5"/>
        <source>The request could not be completed due to a conflict with the current state of the resource.</source>
        <translation>Die Anfrage konnte auf Grund eines Konflikts mit dem aktuellen Zustand der Ressource nicht vervollständigt werden.</translation>
    </message>
    <message id="err-net-reply-207">
        <location line="+4"/>
        <source>The requested resource is no longer available at the server.</source>
        <translation>Die angefragte Ressource ist auf dem Server nicht mehr verfügbar.</translation>
    </message>
    <message id="err-net-reply-401">
        <location line="+4"/>
        <source>The server encountered an unexpected condition which prevented it from fulfilling the request.</source>
        <translation>Auf dem Server ist ein unerwarteter Fehler aufgetreten, der die Verarbeitung der Anfrage verhinderte.</translation>
    </message>
    <message id="err-net-reply-402">
        <location line="+4"/>
        <source>The server does not support the functionality required to fulfill the request.</source>
        <translation>Der Server unterstützt die Funktion zur Beantwortung der Anfrage nicht.</translation>
    </message>
    <message id="err-net-reply-403">
        <location line="+4"/>
        <source>The server is unable to handle the request at this time.</source>
        <translation>Der Server kann die Anfrage zur Zeit nicht verarbeiten.</translation>
    </message>
    <message id="err-net-reply-301">
        <location line="+5"/>
        <source>The Network Access API cannot honor the request because the protocol is not known.</source>
        <translation>Die Anfrage kann nicht verarbeitet werden, da das Protokoll nicht bekannt ist.</translation>
    </message>
    <message id="err-net-reply-302">
        <location line="+4"/>
        <source>The requested operation is invalid for this protocol.</source>
        <translation>Diese Operation ist mit diesem Protokoll nicht möglich.</translation>
    </message>
    <message id="err-net-reply-99">
        <location line="+4"/>
        <source>An unknown network-related error was detected.</source>
        <translation>Es ist ein unbekannter Netzwerkfehler aufgetreten.</translation>
    </message>
    <message id="err-net-reply-199">
        <location line="+4"/>
        <source>An unknown proxy-related error was detected.</source>
        <translation>Es ist ein unbekannter Proxy-Fehler aufgetreten.</translation>
    </message>
    <message id="err-net-reply-299">
        <location line="+4"/>
        <source>An unknown error related to the remote content was detected.</source>
        <translation>Es wurde ein unbekannter Fehler mit dem entfernten Inhalt festgestellt.</translation>
    </message>
    <message id="err-net-reply-399">
        <location line="+4"/>
        <source>A breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.).</source>
        <translation>Es wurde ein Ausfall im Protokoll festgestellt (Parsing-Fehler, ungültige oder unerwartete Antworten, usw.).</translation>
    </message>
    <message id="err-net-reply-499">
        <location line="+5"/>
        <location line="+5"/>
        <source>An unknown error related to the server response was detected.</source>
        <translation>Es wurde ein unbekannter Fehler im Zusammenhang mit der Server-Antwort festgestellt.</translation>
    </message>
    <message id="id-err-version-low-status-user">
        <location filename="../src/Helpers/accountvalidator.cpp" line="+21"/>
        <source>The version of your News App is lower than 5.2.4. Status and user information can not be queried.</source>
        <translation>Die Version Ihrer News App ist kleiner als 5.2.4. Status und Benutzerinformationen können nicht abgefragt werden.</translation>
    </message>
    <message id="id-err-version-low-user">
        <location line="+21"/>
        <source>The version of your News App is lower than 6.0.5. User information can not be queried.</source>
        <translation>Die Version Ihrer News App ist kleiner als 6.0.5. Benutzerinformationen können nicht abgefragt werden.</translation>
    </message>
    <message id="err-version-not-found">
        <location filename="../src/API/getstatus.cpp" line="+102"/>
        <location filename="../src/API/getversion.cpp" line="+99"/>
        <source>Can not find the version information in the server reply.</source>
        <translation>Die Server-Antwort enthält keine Versionsinformation.</translation>
    </message>
    <message id="err-warnings-not-found">
        <location line="+7"/>
        <source>Can not find the warnings information in the server reply.</source>
        <translation>Die Server-Antwort enthält keine Warnhinweise.</translation>
    </message>
    <message id="err-displayname-not-found">
        <location filename="../src/API/getuser.cpp" line="+107"/>
        <source>Can not find the user&apos;s display name in the server reply.</source>
        <translation>Die Server-Antwort enthält keinen Anzeigenamen des Benutzers.</translation>
    </message>
    <message id="libfuoten-err-failed-open-db">
        <location filename="../src/Storage/sqlitestorage.cpp" line="+63"/>
        <location line="+95"/>
        <source>Failed to open the SQLite database.</source>
        <translation>Die SQLite-Datenbank konnte nicht geöffnet werden.</translation>
    </message>
    <message id="fuoten-error-failed-execute-query">
        <location line="-83"/>
        <location line="+16"/>
        <location line="+13"/>
        <location line="+16"/>
        <location line="+7"/>
        <location line="+162"/>
        <location line="+24"/>
        <location line="+26"/>
        <location line="+74"/>
        <source>Failed to execute database query.</source>
        <translation>Datenbankabfrage konnte nicht durchgeführt werden.</translation>
    </message>
    <message id="fuoten-error-transaction-begin">
        <location line="-332"/>
        <location line="+196"/>
        <source>Failed to begin a database transaction.</source>
        <translation>Datenbanktransaktion konnte nicht begonnen werden.</translation>
    </message>
    <message id="fuoten-error-transaction-commit">
        <location line="-167"/>
        <location line="+239"/>
        <source>Failed to commit a database transaction.</source>
        <translation>Datenbanktransaktion konnte nicht abgeschlossen werden.</translation>
    </message>
    <message id="libfuoten-err-sqlite-db-not-ready">
        <location line="-176"/>
        <source>SQLite database not ready. Can not process requested data.</source>
        <translation>SQLite-Datenbank nicht verfügbar. Daten können nicht verarbeitet werden.</translation>
    </message>
    <message id="fuoten-failed-query-folders">
        <location line="+30"/>
        <source>Failed to query the folders from the local database.</source>
        <translation>Ordner konnten nicht aus der lokalen Datenbank abgefragt werden.</translation>
    </message>
    <message id="fuoten-error-failed-prepare-query">
        <location line="+101"/>
        <location line="+26"/>
        <source>Failed to prepare database query.</source>
        <translation>Datenbankabfrage konnte nicht vorbereitet werden.</translation>
    </message>
</context>
</TS>