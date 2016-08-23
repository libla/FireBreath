#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for WebApp
#
#\**********************************************************/

set(PLUGIN_NAME "WebApp")
set(PLUGIN_PREFIX "App")
set(COMPANY_NAME "MiniGame")

# ActiveX constants:
set(FBTYPELIB_NAME WebAppLib)
set(FBTYPELIB_DESC "WebApp 1.0 Type Library")
set(IFBControl_DESC "WebApp Control Interface")
set(FBControl_DESC "WebApp Control Class")
set(IFBComJavascriptObject_DESC "WebApp IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "WebApp ComJavascriptObject Class")
set(IFBComEventSource_DESC "WebApp IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID d701c925-1f57-5b07-b75b-be9d3264629b)
set(IFBControl_GUID 8c08e7bb-c621-51e4-b9af-bfbbf25994bb)
set(FBControl_GUID 8084423c-ccd9-5501-b22c-ab2b9ec32ce3)
set(IFBComJavascriptObject_GUID 3b3830b7-e0c2-5759-b379-7c572fb37142)
set(FBComJavascriptObject_GUID 42f81235-c628-56ee-8219-75ce95047219)
set(IFBComEventSource_GUID aaa5bf49-2730-548b-85c3-b7b656677acd)
if ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 27fd9b74-b7cf-5255-b55e-f9de4893035e)
else ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 625b7fab-263e-5ac2-8320-9ab515a143d6)
endif ( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "MiniGame.WebApp")
if ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "mini1.cn/webapp")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(MOZILLA_PLUGINID "mini1.cn/webapp_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )

# strings
set(FBSTRING_CompanyName "MiniGame")
set(FBSTRING_PluginDescription "Web Application Player")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "© MiniGame. All Rights Reserved.")
set(FBSTRING_PluginFileName "NP${PLUGIN_NAME}")
if (APPLE)
    # On apple, NP is not needed
    set(FBSTRING_PluginFileName "${PLUGIN_NAME}")
endif()
set(FBSTRING_ProductName "WebApp")
set(FBSTRING_FileExtents "")
if ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "WebApp")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "WebApp_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType "application/x-webapp-player")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

#set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 1)
set(FBMAC_USE_COCOA 1)
set(FBMAC_USE_COREGRAPHICS 1)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)

add_firebreath_library(log4cplus)
add_firebreath_library(curl)
add_firebreath_library(zlib)
add_firebreath_library(minizip)
add_firebreath_library(mbedcrypto)