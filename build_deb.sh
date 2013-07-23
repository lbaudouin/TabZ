#!/bin/bash
mkdir -p debian/usr/bin/
cp TabS debian/usr/bin/
dpkg-deb --build debian/
mv debian.deb TabS.deb
