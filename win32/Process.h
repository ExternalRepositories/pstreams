#ifndef	_Process_h_
#define _Process_h_

#if defined(_WIN32) || defined(WIN32)
	#define REDI_OS_WIN32
	#ifndef WIN32
		#define WIN32
	#endif
#else
	#define REDI_OS_UNIX
#endif

#if		defined(REDI_OS_UNIX)
#elif	defined(REDI_OS_WIN32)
	#ifdef	DWORD
	#undef	DWORD
	#endif

	#define NOATOM
	#define NOGDI
	#define NOGDICAPMASKS
	#define NOMETAFILE
	#define NOMINMAX
	#define NOMSG
	#define NOOPENFILE
	#define NORASTEROPS
	#define NOSCROLL
	#define NOSOUND
	#define NOSYSMETRICS
	#define NOTEXTMETRIC
	#define NOWH
	#define NOCOMM
	#define NOKANJI
	#define NOCRYPT
	#define NOMCX

	#include <windows.h>
#endif

#include <list>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;
typedef	list<string>		Arguments;
typedef	list<string>		Environment;

class  Process {
private:
	bool				started;
	Arguments			arguments;
	Environment			environment;
	int					error;
	string				command;
	string				program;

#if		defined(REDI_OS_UNIX)
	pid_t				ppid;
	int					status_;
#elif	defined(REDI_OS_WIN32)
	PROCESS_INFORMATION pid;
	STARTUPINFO         startup;
	SECURITY_ATTRIBUTES	psec;	//process level
	SECURITY_ATTRIBUTES	tsec;	//thread level
	DWORD				status_;
	char*				args;
	char*				env;
	size_t				envlen;
	size_t				arglen;
#endif

private:
	void init();

public:
	enum ChildState {
		UNDEF		= -1,
		ACTIVE		= 0,
		EXITED		= 1,
	};
	
	class Exception: public exception {
	public:
		Exception(const string& text);
		~Exception();
	};
	Process();
	Process(const string& command);
	Process(const string& program, Arguments args);
#if		defined(REDI_OS_UNIX)
#elif	defined(REDI_OS_WIN32)
	Process(const string& command, void* in, void* out, void* err);
	Process(const string& program, Arguments args, void* in, void* out, void* err);
#endif

	void setArguments(const Arguments& arguments);
	void addArgument(const string& arg);
	void addArgument(const char* arg);
	void addArgument(const vector<string> arguments);
	void addVariable(const string& var, const string& value);

	bool spawn();
	bool active() const { return started; };
	bool stop();
	ChildState state();
    ChildState wait(bool nohang = true, int milli = INFINITE);
	
	int	status() { return status_; }
	int getLastError() const;

private:
#if		defined(REDI_OS_UNIX)
#elif	defined(REDI_OS_WIN32)
	void start() throw();
#endif


};
#endif