#!/bin/bash
set -e

VERSION="1.0-1"
ARCH="amd64"
PKG_DIR="vsysinfo_${VERSION}_${ARCH}"

echo "Cleaning up old package directory..."
rm -rf ${PKG_DIR}
mkdir -p ${PKG_DIR}/DEBIAN
mkdir -p ${PKG_DIR}/usr/bin
mkdir -p ${PKG_DIR}/usr/share/applications
mkdir -p ${PKG_DIR}/usr/share/icons/hicolor/256x256/apps

echo "Copying files to package directory..."
cp bin/vsysinfo ${PKG_DIR}/usr/bin/
cp vsysinfo.desktop ${PKG_DIR}/usr/share/applications/
if [ -f "vsysinfo.png" ]; then
    cp vsysinfo.png ${PKG_DIR}/usr/share/icons/hicolor/256x256/apps/
else
    echo "Warning: vsysinfo.png not found, icon won't be packaged."
fi

echo "Creating control file..."
cat <<EOF > ${PKG_DIR}/DEBIAN/control
Package: vsysinfo
Version: ${VERSION}
Section: utils
Priority: optional
Architecture: ${ARCH}
Depends: libc6, libgtk-3-0
Maintainer: VAXP Developer <dev@vaxp.com>
Description: VAXP Hardware Info
 Advanced system monitoring tool for VAXP-OS presenting CPU, Memory, 
 Motherboard, GPU, Sensors, and Storage in a modern GTK3 GUI.
EOF

echo "Building Debian package..."
dpkg-deb --build ${PKG_DIR}
echo "Done! Package is available at ${PKG_DIR}.deb"
