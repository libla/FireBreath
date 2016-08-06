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
	registerMethod("message",
		make_method(this, &WebAppAPI::message));
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

void WebAppAPI::start(const FB::variant &url, const FB::variant &md5, const FB::variant &args)
{
	std::string urlstr = url.cast<std::string>();
	std::string md5str;
	std::string argstr;
	if (args.empty())
	{
		if (!md5.empty())
			argstr = md5.cast<std::string>();
	}
	else
	{
		md5str = md5.cast<std::string>();
		argstr = args.cast<std::string>();
	}
	getPlugin()->onStart(urlstr, md5str, argstr);
}

FB::variant WebAppAPI::message(const std::string &msg)
{
	FB::variant value;
	std::string result;
	if (getPlugin()->onMessage(msg, result))
		value = result;
	return value;
}

// Read-only property version
std::string WebAppAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}
