# libfuoten
A [Qt](https://www.qt.io/) based library to provide access to the ownCloud / Nextcloud [News App](https://github.com/nextcloud/news) API.

libfuoten implements the complete [New App REST API v1.2](https://github.com/nextcloud/news/blob/master/docs/externalapi/Legacy.md) and provides interfaces to create Qt applications talking to an ownCloud/Nextcloud server with News App enabled. Beside the API implementation classes there are classes to support local storage and models to represent the stored data. Helper classes make it easier to synchronize local storage with the remote server.

Have a look at the [roadmap](https://github.com/Huessenbergnetz/Fuoten/milestones) for further information about the development plans. Find out more in the [API documentation](https://doc.huessenbergnetz.de/libfuoten/?pk_campaign=Github-Project-Libfuoten&pk_kwd=ReadmeFile) of libfuoten.

## Features
* account validation (request version, status and user)
* folder management (synchronization, adding, deleting, renaming, marking as read)
* folder list model and filtering
* feeds management (synchronization, adding, deleting, renaming, marking as read, moving)
* feeds list model and filtering
* articles management (synchronization, marking as read/unread/starred/unstarred)
* articles list model and filtering
* synchronization class
* SQLite storage class

## Contributing
The source code is available on [Github](https://github.com/Huessenbergnetz/libfuoten), feel free to clone or branch according to the [LGPLv3](https://github.com/Huessenbergnetz/libfuoten/blob/master/LICENSE). Translation is done on [Transifex](https://www.transifex.com/huessenbergnetz/libfuoten).

## Getting libfuoten
* [openSUSE](https://software.opensuse.org/package/libfuoten)

## Building libfuoten
Either clone the git repository or download one of the [release](https://github.com/Huessenbergnetz/libfuoten/releases) tarballs. cmake, zlib, pkg-config and Qt 5.6 or newer are required to build libfuoten as well as the following Qt modules: Qt5Core, Qt5Network and Qt5Sql.

### Install dependencies
Install gcc, g++, cmake, make, zlib, Qt5Core, Qt5Network and Qt5Sql.

#### openSUSE

```
sudo zypper in cmake gcc-++ make pkg-config zlib-devel libqt5-qtbase-common-devel libqt5-qtbase-devel libQt5Core-devel libQt5Network-devel libQt5Sql-devel
```
For creating documentation:
```
sudo zypper in doxygen libqt5-qtdoc-devel libqt5-qttools
```

### Available cmake variables
* We use cmake’s GNUInstallDirs extension to select target directories
* BUILD_SHARED_LIBS - Enable the build of shared libraries (default: on)
* ENABLE_MAINTAINER_FLAGS - Enables some build flags used for development (default: off)
* BUILD_DOCS - Enable the creation of doc targets, needs doxygen (default: off)
* BUILD_DOCS_QUIET - Tell doxygen to be quiet while building the documentation (default: off)
* I18NDIR - Target installation directory for translation files

### Additional make targets
When `BUILD_DOCS` is enabled, additional build targets are available.

#### webdocs
Will create HTML documentation files linked to Qt online documentation if correct tag files are avaiable.

#### htmldocs
Will create HTML documentation files linked to local Qt offline HTML documentation if correct tag files are available.

#### qtdocs
Will create compiled Qt documentation usable inside Qt creator if qhelpgenerator is available.

### Get libfuoten and build it
```
git clone https://github.com/Huessenbergnetz/libfuoten.git
cd libfuoten
mkdir build && cd build
cmake ..
make
sudo make install
```

## License
```
libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
Copyright (C) 2016-2021 Hüssenbergnetz/Matthias Fehring
https://github.com/Huessenbergnetz/libfuoten

This library is free software: you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
```

