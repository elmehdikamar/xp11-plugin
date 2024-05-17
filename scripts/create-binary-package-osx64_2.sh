
echo "STKPConnect Fat Plugin Assembler"

# Config
VERSION="0.1"
PLATFORM="osx"
ARC="64"

QT_LIB_PATH="/Users/mehdi.kamar/stkpconnector"
STKPCONNECT_PROJECT_PATH="/Users/mehdi.kamar/xp11-plugin-master"
PLUGIN_NAME="stkpconnector"
PACKAGE_PATH="/Users/mehdi.kamar/xp11-plugin-master/stkpconnect-plugin/$PLUGIN_NAME"

# Show config
echo " "
echo "VERSION = $VERSION";
echo "PLATFORM = $PLATFORM";
echo "ARC = $ARC";
echo "QT_LIB_PATH = $QT_LIB_PATH";
echo "STKPCONNECT_PROJECT_PATH = $STKPCONNECT_PROJECT_PATH";
echo "PACKAGE_PATH = $PACKAGE_PATH";

echo "Copying Qt Frameworks..."
rm -rf $PACKAGE_PATH/*.framework
cp -Rf $QT_LIB_PATH/QtCore.framework $PACKAGE_PATH/
cp -Rf $QT_LIB_PATH/QtNetwork.framework $PACKAGE_PATH/

echo "Change framework ids..."
LINK_PATH="@executable_path/../../../Resources/plugins/$PLUGIN_NAME"
install_name_tool -id $LINK_PATH/QtCore.framework/Versions/5/QtCore $PACKAGE_PATH/QtCore.framework/Versions/5/QtCore
install_name_tool -id $LINK_PATH/QtNetwork.framework/Versions/5/QtNetwork $PACKAGE_PATH/QtNetwork.framework/Versions/5/QtNetwork

# Change framework paths in QtNetwork
echo "Changing framework paths in QtNetwork.framework..."
install_name_tool -change $QT_LIB_PATH/QtCore.framework/Versions/5/QtCore $LINK_PATH/QtCore.framework/Versions/5/QtCore $PACKAGE_PATH/QtNetwork.framework/Versions/5/QtNetwork

echo "Changing framework paths in xpl..."
install_name_tool -change $QT_LIB_PATH/QtCore.framework/Versions/5/QtCore $LINK_PATH/QtCore.framework/Versions/5/QtCore $PACKAGE_PATH/mac.xpl
install_name_tool -change $QT_LIB_PATH/QtNetwork.framework/Versions/5/QtNetwork $LINK_PATH/QtNetwork.framework/Versions/5/QtNetwork $PACKAGE_PATH/mac.xpl
install_name_tool -change @executable_path/../../../Resources/plugins/XPLM.framework/XPLM @executable_path/../../../Resources/plugins/XPLM.framework/XPLM $PACKAGE_PATH/mac.xpl

echo "Stripping debug symbols..."
find $PACKAGE_PATH/QtNetwork.framework -iname *_debug* -exec rm -rf {} \;
find $PACKAGE_PATH/QtCore.framework -iname *_debug* -exec rm -rf {} \;

# Show bindings...
#otool -L $PACKAGE_PATH/$ARC/mac.xpl

echo "Done."

