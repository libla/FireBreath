extern "C"
{
	struct Interface
	{
		void (*close)(Interface *ptr, int i);
		void (*notify)(Interface *ptr, const char *type, const char *msg);
	};

	typedef int (*StartFunc)(const void *data, int len, void *wnd, Interface *ptr, const char *args);
	typedef void (*CloseFunc)();
	typedef void (*ResizeFunc)();
	typedef const char * (*MessageFunc)(const char *msg);
};