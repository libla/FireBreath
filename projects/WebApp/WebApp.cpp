/**********************************************************\

  Auto-generated WebApp.cpp

  This file contains the auto-generated main plugin object
  implementation for the WebApp project

\**********************************************************/

#include "WebAppAPI.h"

#include "WebApp.h"

#include <FBControl.h>

///////////////////////////////////////////////////////////////////////////////
/// @fn WebApp::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginInitialize()
///
/// @see FB::FactoryBase::globalPluginInitialize
///////////////////////////////////////////////////////////////////////////////
void WebApp::StaticInitialize()
{
    // Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
}

///////////////////////////////////////////////////////////////////////////////
/// @fn WebApp::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginDeinitialize()
///
/// @see FB::FactoryBase::globalPluginDeinitialize
///////////////////////////////////////////////////////////////////////////////
void WebApp::StaticDeinitialize()
{
    // Place one-time deinitialization stuff here. As of FireBreath 1.4 this should
    // always be called just before the plugin library is unloaded
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  WebApp constructor.  Note that your API is not available
///         at this point, nor the window.  For best results wait to use
///         the JSAPI object until the onPluginReady method is called
///////////////////////////////////////////////////////////////////////////////
WebApp::WebApp() : window(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  WebApp destructor.
///////////////////////////////////////////////////////////////////////////////
WebApp::~WebApp()
{
    // This is optional, but if you reset m_api (the shared_ptr to your JSAPI
    // root object) and tell the host to free the retained JSAPI objects then
    // unless you are holding another shared_ptr reference to your JSAPI object
    // they will be released here.
    releaseRootJSAPI();
    m_host->freeRetainedObjects();
}

void WebApp::onPluginReady()
{
    // When this is called, the BrowserHost is attached, the JSAPI object is
    // created, and we are ready to interact with the page and such.  The
    // PluginWindow may or may not have already fire the AttachedEvent at
    // this point.
}

void WebApp::shutdown()
{
    // This will be called when it is time for the plugin to shut down;
    // any threads or anything else that may hold a shared_ptr to this
    // object should be released here so that this object can be safely
    // destroyed. This is the last point that shared_from_this and weak_ptr
    // references to this object will be valid
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Creates an instance of the JSAPI object that provides your main
///         Javascript interface.
///
/// Note that m_host is your BrowserHost and shared_ptr returns a
/// FB::PluginCorePtr, which can be used to provide a
/// boost::weak_ptr<WebApp> for your JSAPI class.
///
/// Be very careful where you hold a shared_ptr to your plugin class from,
/// as it could prevent your plugin class from getting destroyed properly.
///////////////////////////////////////////////////////////////////////////////
FB::JSAPIPtr WebApp::createJSAPI()
{
    // m_host is the BrowserHost
	api = boost::make_shared<WebAppAPI>(FB::ptr_cast<WebApp>(shared_from_this()), m_host);
    return api;
}

bool WebApp::onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse down at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool WebApp::onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse up at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool WebApp::onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse move at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}
bool WebApp::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *win)
{
    // The window is attached; act appropriately
	window = win;
    return false;
}

bool WebApp::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *win)
{
    // The window is about to be detached; act appropriately
	if (window != win)
	{
		window = NULL;
	}
    return false;
}

bool WebApp::onWindowEventHandle(FB::WindowsEvent *evt, FB::PluginWindow *)
{
	return false;
}

void WebApp::onStart(const std::string &url, const std::string &md5, const std::string &args)
{
	api->fire_notify(0, "", g_dllPath + args);
}