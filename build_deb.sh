#!/bin/bash
cp TabS debian/usr/bin/tabs
dpkg-deb --build debian/
mv debian.deb tabs.deb
