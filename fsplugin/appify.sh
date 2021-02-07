#!/bin/sh
set -e

rm -Rf "$1"/../macOSTemp
mkdir -p "$1"/../macOSTemp/Contents/MacOS
mv "$1"/* "$1"/../macOSTemp/Contents/MacOS/
mv "$1"/../macOSTemp/Contents/MacOS/Version.txt "$1"/

P="$1"/../macOSTemp/Contents/Info.plist

echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" >> $P
echo "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" >> $P
echo "<plist version=\"1.0\">" >> $P
echo "<dict>" >> $P
echo "<key>CFBundleDevelopmentRegion</key>" >> $P
echo "<string>English</string>" >> $P
echo "<key>CFBundleDisplayName</key>" >> $P
echo "<string>$2</string>" >> $P
echo "<key>CFBundleExecutable</key>" >> $P
echo "<string>$3</string>" >> $P
echo "<key>CFBundleIdentifier</key>" >> $P
echo "<string>$4</string>" >> $P
echo "<key>CFBundleInfoDictionaryVersion</key>" >> $P
echo "<string>6.0</string>" >> $P
echo "<key>CFBundleName</key>" >> $P
echo "<string>$2</string>" >> $P
echo "<key>CFBundlePackageType</key>" >> $P
echo "<string>APPL</string>" >> $P
echo "<key>CFBundleShortVersionString</key>" >> $P
echo "<string>1.0.0</string>" >> $P
echo "<key>CFBundleSignature</key>" >> $P
echo "<string>????</string>" >> $P
echo "<key>CFBundleVersion</key>" >> $P
echo "<string>1.0.0</string>" >> $P
echo "<key>LSMinimumSystemVersion</key>" >> $P
echo "<string>10.6.0</string>" >> $P
echo "<key>NSAppleScriptEnabled</key>" >> $P
echo "<false/>" >> $P
echo "<key>NSMainNibFile</key>" >> $P
echo "<string></string>" >> $P
echo "<key>NSPrincipalClass</key>" >> $P
echo "<string>NSApplication</string>" >> $P
echo "<key>LSUIPresentationMode</key>" >> $P
echo "<integer>4</integer>" >> $P
echo "</dict>" >> $P
echo "</plist>" >> $P

mv "$1"/../macOSTemp "$1"/$2.app
