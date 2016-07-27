/**********************************************************\

  Auto-generated WebAppAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "WebAppAPI.h"

WebAppAPI::WebAppAPI(const WebAppPtr& plugin, const FB::BrowserHostPtr& host) :
	m_plugin(plugin), m_host(host)
{
	registerMethod("start",
		make_method(this, &WebAppAPI::start));
	// Read-only property
	registerProperty("version",
		make_property(this, &WebAppAPI::get_version));
}

WebAppAPI::~WebAppAPI()
{
}

///////////////////////////////////////////////////////////////////////////////
/// @fn WebAppPtr WebAppAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
WebAppPtr WebAppAPI::getPlugin()
{
    WebAppPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

void WebAppAPI::start(const std::string &url, const std::string &args)
{
	getPlugin()->onStart(url, args);
}

// Read-only property version
std::string WebAppAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}
