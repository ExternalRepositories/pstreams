#ifndef	_Process_h_
#define _Process_h_

#include "autosense.h"

#if		defined(OS_UNIX)
#elif	defined(OS_WIN32)
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
#include "Program.h"

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
	Program				program;

#if		defined(OS_UNIX)
	pid_t				ppid;
	int					status_;
#elif	defined(OS_WIN32)
	PROCESS_INFORMATION pid;
	STARTUPINFO         startup;
	SECURITY_ATTRIBUTES	psec;	//process level
	SECURITY_ATTRIBUTES	tsec;	//thread level
	DWORD				status_;
	char*				args;
	char*				env;
	int					envlen;
	int					arglen;
#endif

private:
	void init();

public:
	enum ChildState {
		UNDEF		= -1,
		ACTIVE		= 0,
		EXITED		= 1,
	};
	
	class Exception: public runtime_error {
	public:
		Exception(const string& text);
		~Exception();
	};
	Process();
	Process(const Program& program);
	Process(const string& command);
#if		defined(OS_UNIX)
#elif	defined(OS_WIN32)
	Process(const Program& program, void* in, void* out, void* err);
	Process(const string& command, void* in, void* out, void* err);
#endif

	void setArguments(const Arguments& arguments);
	void addArgument(const string& arg);
	void addArgument(const char* arg);
	void addArgument(const vector<string> arguments);
	void addVariable(const string& var, const string& value);

	bool spawn();
	bool active() const { return started; };
	ChildState state();
	bool stop();
    ChildState wait(bool nohang = true, int milli = INFINITE);
	
	int	status() { return status_; }
	int getLastError() const;

private:
#if		defined(OS_UNIX)
#elif	defined(OS_WIN32)
	void start();
#endif


};
#endif