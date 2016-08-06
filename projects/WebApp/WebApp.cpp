/**********************************************************\

  Auto-generated WebApp.cpp

  This file contains the auto-generated main plugin object
  implementation for the WebApp project

\**********************************************************/

#include "WebAppAPI.h"

#include "WebApp.h"

#include <FBControl.h>
#include <BrowserStreamRequest.h>
#include <PluginWindowWin.h>
#include <unzip.h>
#include <md5.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

static std::string uuid()
{
	GUID guid;
	CoCreateGuid(&guid);
	char buffer[36];
	sprintf(buffer, "%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x", (unsigned int)guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	return std::string(buffer);
}

static unsigned int FNV(const char *path)
{
	unsigned int num = 2166136261U;
	while (true)
	{
		unsigned char byte = *path++;
		if (byte == 0)
			break;
		if (byte == '\\')
			byte = '/';
		else
			byte = tolower(byte);
		num ^= byte;
		num *= 16777619U;
	}
	return num;
}

namespace
{
	enum Result
	{
		Succeed,
		FormatFail,
		VerifyFail,
		NotSupport,
		RuntimeError,
		UnknownError,
	};
}

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
WebApp::WebApp() : window(NULL), si(this), started(false), module(NULL), buffer(NULL), bufflen(0)
{
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  WebApp destructor.
///////////////////////////////////////////////////////////////////////////////
WebApp::~WebApp()
{
	delete []buffer;
	if (module)
	{
		FreeLibrary(module);
		module = NULL;
	}
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
	FBLOG_DEBUG("webapp", "attach");
    // The window is attached; act appropriately
	window = win;
    return true;
}

bool WebApp::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *win)
{
	FBLOG_DEBUG("webapp", "detach");
    // The window is about to be detached; act appropriately
	if (window == win)
	{
		window = NULL;
		if (module != NULL && close != NULL)
			close();
	}
    return true;
}

bool WebApp::onWindowResized(FB::ResizedEvent *evt, FB::PluginWindow *)
{
	if (module != NULL && resize != NULL)
		resize();
	return true;
}

bool WebApp::onStreamEvent(FB::StreamEvent *evt, FB::BrowserStream *)
{
	if (evt->validType<FB::StreamDataArrivedEvent>())
	{
		FB::StreamDataArrivedEvent *dataevt = evt->get<FB::StreamDataArrivedEvent>();
		size_t len = dataevt->getLength();
		if (buffused + len > bufflen)
		{
			while (bufflen < buffused + len)
				bufflen = (bufflen + 1) * 2;
			unsigned char *tmp = new unsigned char[bufflen];
			if (buffused != 0)
				memcpy(tmp, buffer, buffused);
			delete[] buffer;
			buffer = tmp;
		}
		memcpy(buffer + buffused, dataevt->getData(), len);
		buffused += len;
	}
	else if (evt->validType<FB::StreamOpenedEvent>())
	{
		FBLOG_DEBUG("webapp", ("download " + evt->stream->getUrl()).c_str());
	}
	else if (evt->validType<FB::StreamFailedOpenEvent>())
	{
		api->fire_error("Unable to download the file from '" + evt->stream->getUrl() + "'");
	}
	else if (evt->validType<FB::StreamCompletedEvent>())
	{
		bool result = evt->get<FB::StreamCompletedEvent>()->success;
		if (result)
		{
			// 保存文件
			if (!filename.empty())
			{
				std::string path = "cache\\" + uuid();
				FILE *file = fopen(path.c_str(), "wb");
				do 
				{
					if (file == NULL)
					{
						api->fire_error("Fail to write to file '" + path + "'");
						break;
					}
					bool result = fwrite(buffer, 1, buffused, file) == buffused;
					fclose(file);
					if (!result)
					{
						remove(path.c_str());
						api->fire_error("Fail to write to file '" + path + "'");
						break;
					}
					rename(path.c_str(), ("webapp\\" + filename).c_str());
				} while (false);
			}

			do
			{
				// 校验文件
				if (!filemd5.empty())
				{
					unsigned char verify[16];
					Md5Calc(verify, (char *)buffer, buffused);
					if (memcmp(verify, &(filemd5[0]), 16) != 0)
					{
						api->fire_error("'" + evt->stream->getUrl() + "' md5sum fail");
						break;
					}
				}

				// 执行webapp
				boost::shared_array<unsigned char> data(buffer);
				buffer = NULL;
				bufflen = 0;
				switch (execute(data, buffused, false, args))
				{
				case Succeed:
					FBLOG_DEBUG("webapp", "succeed");
					break;
				case FormatFail:
					api->fire_error("'" + evt->stream->getUrl() + "' is not a valid web application");
					break;
				case VerifyFail:
					api->fire_error("'" + evt->stream->getUrl() + "' verify fail");
					break;
				case NotSupport:
					api->fire_error("'" + evt->stream->getUrl() + "' is not support your platform");
					break;
				case RuntimeError:
					api->fire_error("'" + evt->stream->getUrl() + "' failed during startup/running");
					break;
				default:
					api->fire_error("'" + evt->stream->getUrl() + "' get a unknown error");
				}
			} while (false);
		}
		else
		{
			api->fire_error("Failed to download '" + evt->stream->getUrl() + "'");
		}
	}
	return true;
}

void WebApp::onStart(const std::string &url, const std::string &md5, const std::string &args)
{
	if (started)
		throw FB::script_error("Could not start again before close");

	started = true;
	// 获取dll所在目录
	std::string rootpath;
	{
		int pos = g_dllPath.find_last_of("\\");
		rootpath = g_dllPath.substr(0, pos + 1);
	}
	SetCurrentDirectoryW(FB::utf8_to_wstring(rootpath).c_str());
	// 创建必须的文件夹
	if (!PathFileExistsA("webapp"))
		CreateDirectoryA("webapp", NULL);
	if (!PathFileExistsA("cache"))
		CreateDirectoryA("cache", NULL);
	FBLOG_DEBUG("webapp", "start");

	// 获取文件name，如果md5有值，则为md5所代表的值，否则为url的"域名-最后的文件名"形式
	std::string name;
	if (md5.empty())
	{
		std::string domain;
		int domainstart = url.find_first_of(":");
		if (domainstart != url.npos)
		{
			domainstart = url.find_first_not_of("/", domainstart + 1);
			int domainend = url.find_first_of("/", domainstart);
			domain = url.substr(domainstart, domainend);
		}
		int pos = url.find_last_of("/");
		if (pos != url.npos)
			name = domain + "-" + url.substr(pos + 1);
		else
			name = domain;
	}
	else
	{
		name = md5;
	}
	if (name.empty())
		throw FB::script_error("Bad argument #1");

	// 读取本地缓存文件
	std::string path = rootpath + name;
	FILE *file;
	{
		std::wstring pathw = FB::utf8_to_wstring(path);
		file = _wfopen(pathw.c_str(), L"rb");
	}
	if (file != NULL)
	{
		fseek(file, 0, SEEK_END);
		size_t len = ftell(file);
		fseek(file, 0, SEEK_SET);
		boost::shared_array<unsigned char> data(new unsigned char[len]);
		len = fread(data.get(), 1, len, file);
		fclose(file);
		// 本地缓存文件不需要再进行校验
		if (execute(data, len, true, args) == Succeed)
			return;
	}
	filename = name;
	filemd5.clear();
	if (!md5.empty())
	{
		// md5字符串转换成字节流
		std::vector<unsigned char> summd5;
		summd5.resize(16);
		size_t index = 0;
		while (md5[index] == ' ' || md5[index] == '\t')
			++index;
		if (md5.length() < index + 32)
			throw FB::script_error("Bad argument #2");
		for (int i = 0; i < 16; ++i)
		{
			unsigned char sum = 0;
			for (int j = 0; j < 2; ++j)
			{
				unsigned char c = md5[i * 2 + j + index];
				if (c >= '0' && c <= '9')
					sum = sum * 16 + c - '0';
				else if (c >= 'a' && c <= 'f')
					sum = sum * 16 + c - 'a' + 10;
				else if (c >= 'A' && c <= 'F')
					sum = sum * 16 + c - 'A' + 10;
				else
					throw FB::script_error("Bad argument #2");
			}
			summd5[i] = sum;
		}
		filemd5.swap(summd5);
	}
	this->args = args;
	buffused = 0;
	getHost()->createStream(url, shared_from_this(), false);
}

bool WebApp::onMessage(const std::string &msg, std::string &result)
{
	if (module == NULL || message == NULL)
		return false;
	const char *ret = message(msg.c_str());
	result = ret == NULL ? "" : ret;
	return true;
}

void WebApp::onClose(int i)
{
	started = false;
	if (module)
	{
		FreeLibrary(module);
		module = NULL;
	}
	api->fire_close(i);
}

void WebApp::onNotify(const std::string &type, const FB::variant &args)
{
	api->fire_notify(type, args);
}

namespace
{
	struct MemZip
	{
	public:
		boost::shared_array<unsigned char> data;
		size_t length;
		size_t offset;
		int error;
	};
}

static voidpf zopen_file(voidpf opaque, const char* filename, int mode)
{
	MemZip *zip = (MemZip *)opaque;
	zip->error = 0;
	return opaque;
}

static uLong zread_file(voidpf opaque, voidpf stream, void* buf, uLong size)
{
	MemZip *zip = (MemZip *)opaque;
	zip->error = 0;
	size_t l = (size_t)size;
	if (l + zip->offset > zip->length)
		l = zip->length - zip->offset;
	memcpy(buf, zip->data.get() + zip->offset, l);
	zip->offset += l;
	return (uLong)l;
}

static uLong zwrite_file(voidpf opaque, voidpf stream, const void* buf, uLong size)
{
	MemZip *zip = (MemZip *)opaque;
	zip->error = 0;
	size_t l = (size_t)size;
	if (l + zip->offset > zip->length)
		l = zip->length - zip->offset;
	memcpy(zip->data.get() + zip->offset, buf, l);
	zip->offset += l;
	return (uLong)l;
}

static long ztell_file(voidpf opaque, voidpf stream)
{
	MemZip *zip = (MemZip *)opaque;
	zip->error = 0;
	return (long)zip->offset;
}

static long zseek_file(voidpf opaque, voidpf stream, uLong offset, int origin)
{
	MemZip *zip = (MemZip *)opaque;
	zip->error = 0;
	size_t origin_offset;
	switch (origin)
	{
	case ZLIB_FILEFUNC_SEEK_CUR :
		origin_offset = zip->offset;
		break;
	case ZLIB_FILEFUNC_SEEK_END :
		origin_offset = zip->length;
		break;
	case ZLIB_FILEFUNC_SEEK_SET :
		origin_offset = 0;
		break;
	default:
		return -1;
	}
	size_t newoffset = origin_offset + (size_t)offset;
	if (zip->length < newoffset)
	{
		zip->error = -1;
		return -1;
	}
	zip->offset = newoffset;
	return 0;
}

static int zclose_file(voidpf opaque, voidpf stream)
{
	MemZip *zip = (MemZip *)opaque;
	zip->error = 0;
	return 0;
}

static int zerror_file(voidpf opaque, voidpf stream)
{
	MemZip *zip = (MemZip *)opaque;
	return zip->error;
}

namespace
{
	struct FileData 
	{
		size_t uncompressed;
		size_t compressed;
		unz_file_pos pos;
	};
}

int WebApp::execute(boost::shared_array<unsigned char> &data, size_t len, bool old, const std::string &args)
{
	FBLOG_DEBUG("webapp", "execute");
	MemZip zip;
	zip.data = data;
	zip.length = len;
	zip.offset = 0;
	zlib_filefunc_def fn = {
		zopen_file,
		zread_file,
		zwrite_file,
		ztell_file,
		zseek_file,
		zclose_file,
		zerror_file,
		&zip,
	};
	std::map<unsigned int, FileData> filedatas;
	unzFile file = unzOpen2("", &fn);
	if (unzOpen2 == NULL)
		return FormatFail;
	// 遍历ZIP文件
	unz_global_info global_info;
	if (UNZ_OK == unzGetGlobalInfo(file, &global_info))
	{
		char *name = NULL;
		size_t length = 0;
		do 
		{
			unz_file_info info;
			unzGetCurrentFileInfo(file, &info, NULL, 0, NULL, 0, NULL, 0);
			if (length < info.size_filename + 1)
			{
				free(name);
				length = info.size_filename + 1;
				name = (char *)malloc(length);
			}
			unzGetCurrentFileInfo(file, NULL, name, info.size_filename, NULL, 0, NULL, 0);
			if (info.size_filename > 0 && name[info.size_filename - 1] != '/')
			{
				name[info.size_filename] = 0;
				FileData &filedata = filedatas[FNV(name)];
				filedata.compressed = info.compressed_size;
				filedata.uncompressed = info.uncompressed_size;
				unzGetFilePos(file, &(filedata.pos));
			}
		} while (UNZ_OK == unzGoToNextFile(file));
		free(name);
	}
	int result = UnknownError;
	std::map<unsigned int, FileData>::iterator i;
	do
	{
		// 首先解压manifest.json(webapp包固定需要有此文件)
		i = filedatas.find(FNV("manifest.json"));
		if (i == filedatas.end())
		{
			result = FormatFail;
			break;
		}
		unzGoToFilePos(file, &(i->second.pos));
		unzOpenCurrentFile(file);
		boost::shared_array<char> jsonbuf(new char[i->second.uncompressed + 1]);
		unzReadCurrentFile(file, jsonbuf.get(), i->second.uncompressed);
		jsonbuf[i->second.uncompressed] = 0;
		rapidjson::Document doc;
		// 解析JSON，mark字段表示该webapp包的属性，也用于作为root文件夹；win32/win64表示对应各自平台的运行时库
		doc.ParseInsitu(jsonbuf.get());
		if (doc.HasParseError())
		{
			result = FormatFail;
			break;
		}
		rapidjson::Value value;
		value = doc["mark"];
		if (!value.IsString())
		{
			result = NotSupport;
			break;
		}
		std::string mark(value.GetString(), value.GetStringLength());
		if (sizeof(int *) == 4)
			value = doc["win32"];
		else
			value = doc["win64"];
		if (!value.IsString())
		{
			result = NotSupport;
			break;
		}
		// 解压运行时库指示的文件
		std::string name(value.GetString(), value.GetStringLength());
		i = filedatas.find(FNV(name.c_str()));
		if (i == filedatas.end())
		{
			result = NotSupport;
			break;
		}
		unzGoToFilePos(file, &(i->second.pos));
		unzOpenCurrentFile(file);
		size_t len = i->second.uncompressed;
		boost::shared_array<unsigned char> buffer(new unsigned char[len]);
		unzReadCurrentFile(file, buffer.get(), len);
		// 保存到随机名字(以解决重复加载同一dll，全局/静态变量共用造成的冲突)
		std::string uuidname = "cache\\" + uuid() + ".dll";
		FILE *dllfile = fopen(uuidname.c_str(), "wb");
		if (dllfile == NULL)
		{
			result = RuntimeError;
			break;
		}
		bool writed = fwrite(buffer.get(), 1, len, dllfile) == len;
		fclose(dllfile);
		if (!writed)
		{
			result = RuntimeError;
			break;
		}
		if (!old)
		{
			// 不是老数据，那么需要校验
		}
		// 载入DLL
		module = LoadLibraryA(uuidname.c_str());
		if (module != NULL)
		{
			FBLOG_DEBUG("webapp", ("loadlib " + uuidname).c_str());
			start = (StartFunc)GetProcAddress(module, "Start");
			close = (CloseFunc)GetProcAddress(module, "Close");
			resize = (ResizeFunc)GetProcAddress(module, "Resize");
			message = (MessageFunc)GetProcAddress(module, "Message");
			if (start == NULL)
			{
				result = FormatFail;
				break;
			}
			// 切换到webapp的root文件夹
			mark = "cache\\" + mark;
			if (!PathFileExistsA(mark.c_str()))
				CreateDirectoryA(mark.c_str(), NULL);
			SetCurrentDirectoryA(mark.c_str());
			// 开始执行
			FB::PluginWindowWin *winwindow = dynamic_cast<FB::PluginWindowWin *>(window);
			start(winwindow == NULL ? NULL : (void *)winwindow->getHWND(), &si, args.c_str());
		}
		result = Succeed;
	} while (false);
	unzClose(file);
	return result;
}

WebApp::StartupInterface::StartupInterface(WebApp *webapp)
{
	this->webapp = webapp;
	this->close = onClose;
	this->notify = onNotify;
}

void WebApp::StartupInterface::onClose(Interface *ptr, int i)
{
	StartupInterface *sp = (StartupInterface *)ptr;
	sp->webapp->onClose(i);
}

void WebApp::StartupInterface::onNotify(Interface *ptr, const char *type, const char *args)
{
	if (type != NULL)
	{
		StartupInterface *sp = (StartupInterface *)ptr;
		FB::variant args_;
		if (args != NULL)
			args_ = std::string(args);
		sp->webapp->onNotify(type, args_);
	}
}
