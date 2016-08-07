/**********************************************************\ 
 
 Auto-generated Factory.cpp
 
 This file contains the auto-generated factory methods 
 for the WebApp project
 
\**********************************************************/

#include "FactoryBase.h"
#include "FBControl.h"
#include "WebApp.h"
#include <boost/make_shared.hpp>

class PluginFactory : public FB::FactoryBase
{
public:
    ///////////////////////////////////////////////////////////////////////////////
    /// @fn FB::PluginCorePtr createPlugin(const std::string& mimetype)
    ///
    /// @brief  Creates a plugin object matching the provided mimetype
    ///         If mimetype is empty, returns the default plugin
    ///////////////////////////////////////////////////////////////////////////////
    FB::PluginCorePtr createPlugin(const std::string& mimetype)
    {
        return boost::make_shared<WebApp>();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginInitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginInitialize()
    {
        WebApp::StaticInitialize();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginDeinitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginDeinitialize()
    {
        WebApp::StaticDeinitialize();
    }

	void getLoggingMethods(FB::Log::LogMethodList& outMethods)
	{
#ifndef NDEBUG
		int pos = g_dllPath.find_last_of("\\");
		std::string rootpath = g_dllPath.substr(0, pos + 1);
		SetCurrentDirectoryW(FB::utf8_to_wstring(rootpath).c_str());
		if (!PathFileExistsA("log"))
			CreateDirectoryA("log", NULL);
		outMethods.push_back(std::make_pair(FB::Log::LogMethod_File, std::string("log/common.log")));
		outMethods.push_back(std::make_pair(FB::Log::LogMethod_Console, std::string()));
#endif
	}

	FB::Log::LogLevel getLogLevel()
	{
#ifndef NDEBUG
		return FB::Log::LogLevel_Debug;
#else
		return FB::Log::LogLevel_Info;
#endif
	}
};

///////////////////////////////////////////////////////////////////////////////
/// @fn getFactoryInstance()
///
/// @brief  Returns the factory instance for this plugin module
///////////////////////////////////////////////////////////////////////////////
FB::FactoryBasePtr getFactoryInstance()
{
    static boost::shared_ptr<PluginFactory> factory = boost::make_shared<PluginFactory>();
    return factory;
}

