#!/bin/bash

CURRENTDIR=$PWD
TMPDIR=/tmp
PRJNAME=libfuoten
SIGNKEY=6607CA3F41B25F45

if [ "$1" == "" ]; then
	echo "You have to specify a version number"
	exit 1
fi

VERSION=$1

if [ "$2" == "" ]; then
	echo "You have to specify an output directory"
	exit 1
fi

OUTDIR=$2

TARNAME="${PRJNAME}-${VERSION}"

DIR="${TMPDIR}/${TARNAME}"

rm -rf $DIR

./scripts/releasel10n.sh

mkdir -p $DIR

cp -a -r -t $DIR *

rm "$DIR/scripts/doxyWebFooter.html"
rm "$DIR/scripts/makeWebDocs.sh"

pushd "${TMPDIR}" > /dev/null

rm -f "${OUTDIR}/${TARNAME}.tar.xz"
rm -f "${OUTDIR}/${TARNAME}.tar.gz"

tar -cJf "${OUTDIR}/${TARNAME}.tar.xz" ${TARNAME}
tar -czf "${OUTDIR}/${TARNAME}.tar.gz" ${TARNAME}

rm -rf ${TARNAME}

popd > /dev/null

gpg --armor --detach-sign --yes --default-key $SIGNKEY "${OUTDIR}/${TARNAME}.tar.xz"
gpg --armor --detach-sign --yes --default-key $SIGNKEY "${OUTDIR}/${TARNAME}.tar.gz"

exit 0
