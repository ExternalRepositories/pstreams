#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstring>

#define	null 0

#include "Process.h"

void Process::init() {
	started = false;envlen = 0;arglen = 0;
	error = -1; status_ = UNDEF;

#if		defined(OS_UNIX)
#elif	defined(OS_WIN32)
    ZeroMemory( &startup, sizeof(startup) );
    startup.cb = sizeof(startup);
    ZeroMemory( &pid, sizeof(pid) );
#endif
}

Process::Process() 
{
	init();
}
Process::Process(const string& command) :
command(command)
{
	init();
}
Process::Process(const string& command, void* in, void* out, void* err) :
command(command) {
	init();
#if		defined(OS_UNIX)
#elif	defined(OS_WIN32)
	startup.hStdError	= err;
	startup.hStdOutput	= out;
	startup.hStdInput	= in;
	startup.dwFlags |= STARTF_USESTDHANDLES;
	args = null; env = null;
#endif
}
Process::Process(const Program& program) :
program(program)
{
	init();
}
Process::Process(const Program& program, void* in, void* out, void* err) :
program(program) {
	init();
#if		defined(OS_UNIX)
#elif	defined(OS_WIN32)
	startup.hStdError	= err;
	startup.hStdOutput	= out;
	startup.hStdInput	= in;
	startup.dwFlags |= STARTF_USESTDHANDLES;
	args = null; env = null;
#endif
}
Process::Exception::Exception(const string& text) :
	runtime_error("Process::Exception: ") {
}
Process::Exception::~Exception() {
}
void Process::setArguments(const Arguments& args){
	for(Arguments::const_iterator i = args.begin(); i != args.end(); i++) {
		arglen += i->size() + 1;
	}
	arguments.insert(arguments.end(), args.begin(), args.end());
}
void Process::addArgument(const string& arg){
	arglen += arg.size() + 1;
	arguments.push_back(arg);
}
void Process::addArgument(const char* arg){
	arglen += strlen(arg) + 1;
	arguments.push_back(arg);
}
void Process::addArgument(const vector<string> args) {
	for(vector<string>::const_iterator i = args.begin(); i != args.end(); i++) {
		arglen += i->size() + 1;
	}
	arguments.insert(arguments.end(), args.begin(), args.end());
}
void Process::addVariable(const string& var, const string& value){
	environment.push_back(var + "=" + value);
	envlen += var.size() + 1 + value.size() + 1;
}
void Process::start() throw(Exception){		// Start the child process. 
	if( CreateProcess(program.getPath().c_str(), // Application name
        args,								// Application arguments
        &psec,								// Process handle not inheritable. 
        &tsec,								// Thread handle not inheritable. 
        TRUE,								// Set handle inheritance to FALSE. 
        CREATE_DEFAULT_ERROR_MODE,          // No creation flags. 
        env,								// Use parent's environment block. 
        NULL,								// Use parent's starting directory. 
        &startup,							// Pointer to STARTUPINFO structure.
        &pid )								// Pointer to PROCESS_INFORMATION structure.
    ) 
    {
		WaitForInputIdle(pid.hProcess, INFINITE);
		started = true;
	} else {
		throw Exception("CreateProcess failed : " + GetLastError());
    }
}
bool Process::spawn() {
#if		defined(OS_UNIX)
#elif	defined(OS_WIN32)
	ZeroMemory( &psec, sizeof(psec) );
	psec.nLength = sizeof(psec);
	psec.bInheritHandle = TRUE;

	ZeroMemory( &tsec, sizeof(tsec) );
	tsec.nLength = sizeof(tsec);
	tsec.bInheritHandle = TRUE;

	if  (command.empty()) {
		if  (!arguments.empty()) {
			args = new char[ 7 + arglen + 1];
			LPSTR	argv = args;
		
			args = static_cast<char*>(memcpy(args, "child ", 6));args+=6; 
			for(Arguments::iterator i = arguments.begin(); i != arguments.end(); i++) {
				std::memcpy(args, i->data(), i->size());args+=i->size();*args++=' ';
			}
			*args = 0;
		}

		if  (!environment.empty()) {

			env = new char[envlen + 1];
			LPSTR	block = env;
			for(Environment::iterator i = environment.begin(); i != environment.end(); i++) {

				std::strcpy(block, i->c_str());block+=i->size() + (size_t)1;
			}
			*block = 0;
		}
	} else {
		args = const_cast<char*>(command.c_str());
	}

	start();
#endif
	return started;
}
Process::ChildState Process::wait(bool nohang, int milli) {
	ChildState		exitcode = UNDEF;
#if		defined(OS_UNIX)
	switch (::waitpid(ppid, &status_, nohang ? WNOHANG : 0)) {
		case 0 :
			// nohang was true and process has not exited
			exitcode = ALIVE;
			break;
		case -1 :
			error = errno;
			break;
		default :
			// process has exited
			ppid_ = 0;
			exitcode = EXITED;
			break;
	}
#elif	defined(OS_WIN32)
	DWORD	waitresult;
	if  (started) {
		// Wait until child process exits.
		do{
                //do some processing here so the app does not appear frozen.
			waitresult = WaitForSingleObject(pid.hProcess, milli);
        } while(waitresult == WAIT_TIMEOUT);
        
		if (waitresult == WAIT_FAILED){
			cout << "WaitForSingleObject() failed" << endl;
                //it failed
                //again we should do something so we don't keep executing
        }

		GetExitCodeProcess(pid.hProcess, &status_);

		cout << "Process::RC=" << status() << endl;
		// Close process and thread handles. 
		CloseHandle( pid.hProcess );
		CloseHandle( pid.hThread );

	}
#endif
		return exitcode;
}
Process::ChildState Process::state() {
#if		defined(OS_UNIX)
#elif	defined(OS_WIN32)
	DWORD	waitresult;
	DWORD	code;
	waitresult = GetExitCodeProcess(pid.hProcess, &code);
	if  (code == STILL_ACTIVE) return ACTIVE;
	else return EXITED;
#endif
}
bool Process::stop() {
#if		defined(OS_UNIX)
	if (::kill(ppid_, signal))
		error = errno;
		return false;
	else {
		// TODO call exited() to check for exit and clean up? leave to user?
		return true;
	}
#else
		return true;//FIXME
#endif
}
Process::getLastError() const {
#if		defined(OS_UNIX)
	return error;
#elif	defined(OS_WIN32)
	return ::GetLastError();
#endif
}

