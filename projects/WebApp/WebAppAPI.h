/**********************************************************\

  Auto-generated WebAppAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "WebApp.h"

#ifndef H_WebAppAPI
#define H_WebAppAPI

class WebAppAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn WebAppAPI::WebAppAPI(const WebAppPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    WebAppAPI(const WebAppPtr& plugin, const FB::BrowserHostPtr &host);

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn WebAppAPI::~WebAppAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~WebAppAPI();

    WebAppPtr getPlugin();
    
    // Event helpers
    FB_JSAPI_EVENT(notify, 3, (const int, const std::string &, const std::string &));

private:
	void start(const FB::variant &url, const FB::variant &md5, const FB::variant &args);
	// Read-only property ${PROPERTY.ident}
	std::string get_version();

private:
    WebAppWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    std::string m_testString;
};

#endif // H_WebAppAPI

