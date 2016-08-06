/**********************************************************\

  Auto-generated WebApp.h

  This file contains the auto-generated main plugin object
  implementation for the WebApp project

\**********************************************************/
#ifndef H_WebAppPLUGIN
#define H_WebAppPLUGIN

#include "PluginWindow.h"
#include "PluginEvents/MouseEvents.h"
#include "PluginEvents/AttachedEvent.h"
#include "PluginEvents/DrawingEvents.h"
#include "PluginEvents/WindowsEvent.h"

#include "PluginCore.h"
#include "Interface.h"

#include <boost/shared_array.hpp>

class WebAppAPI;

FB_FORWARD_PTR(WebApp)
class WebApp : public FB::PluginCore
{
public:
    static void StaticInitialize();
    static void StaticDeinitialize();

public:
    WebApp();
    virtual ~WebApp();

public:
    void onPluginReady();
    void shutdown();
    virtual FB::JSAPIPtr createJSAPI();
    // If you want your plugin to always be windowless, set this to true
    // If you want your plugin to be optionally windowless based on the
    // value of the "windowless" param tag, remove this method or return
    // FB::PluginCore::isWindowless()
    virtual bool isWindowless() { return false; }

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE(FB::MouseDownEvent, onMouseDown, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseUpEvent, onMouseUp, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseMoveEvent, onMouseMove, FB::PluginWindow)
        EVENTTYPE_CASE(FB::AttachedEvent, onWindowAttached, FB::PluginWindow)
        EVENTTYPE_CASE(FB::DetachedEvent, onWindowDetached, FB::PluginWindow)
		EVENTTYPE_CASE(FB::ResizedEvent, onWindowResized, FB::PluginWindow)
		EVENTTYPE_CASE(FB::StreamEvent, onStreamEvent, FB::BrowserStream)
    END_PLUGIN_EVENT_MAP()

    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    virtual bool onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *);
    virtual bool onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *);
    virtual bool onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *);
    virtual bool onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *);
    virtual bool onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *);
	virtual bool onWindowResized(FB::ResizedEvent *evt, FB::PluginWindow *);
	virtual bool onStreamEvent(FB::StreamEvent *evt, FB::BrowserStream *);
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/
	void onStart(const std::string &url, const std::string &md5, const std::string &args);
	bool onMessage(const std::string &msg, std::string &result);
	void onClose(int i);
	void onNotify(const std::string &type, const FB::variant &args);

private:
	int execute(boost::shared_array<unsigned char> &data, size_t len, const std::string &args);

private:
	FB::PluginWindow *window;
	boost::shared_ptr<WebAppAPI> api;

	class StartupInterface : public Interface
	{
	public:
		StartupInterface(WebApp *webapp);

	private:
		WebApp *webapp;

		static void onClose(Interface *ptr, int i);
		static void onNotify(Interface *ptr, const char *type, const char *args);
		static int onListen(Interface *ptr, void (*fn)(void *, const char *), void *ud);
		static void onRemove(Interface *ptr, int id);
	};

	StartupInterface si;

	bool started;
	HMODULE module;
	StartFunc start;
	CloseFunc close;
	ResizeFunc resize;
	MessageFunc message;
	std::string filename;
	std::string execargs;
	unsigned char *buffer;
	size_t bufflen;
	size_t buffused;
};


#endif

