#ifndef	_Pipe_h_
#define _Pipe_h_
#include "autosense.h"


#if		defined(OS_UNIX)
#elif	defined(OS_WIN32)
#pragma warning(disable: 4290)
typedef	void* HANDLE;
typedef long  LONG_PTR;

#ifdef INVALID_HANDLE_VALUE
#undef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE  ((HANDLE)((LONG_PTR)-1))
#endif
#endif

#include <stdexcept>

using namespace std;

#ifdef	DONTWORKYET
class HalfPipe {
protected:
#if		defined(OS_UNIX)
	typedef int		desc;
#elif	defined(OS_WIN32)
	typedef HANDLE	desc;
#endif
protected:
	desc		end;
protected:
	HalfPipe(desc d = INVALID_HANDLE_VALUE) : end(d) {}
public:
	virtual ~HalfPipe() {};
};

class PipeRead : public HalfPipe {
protected:
	PipeRead(desc d) : HalfPipe (d) {}
};

class PipeWrite: public HalfPipe {
protected:
	PipeWrite(desc d) : HalfPipe (d) {}
};

class Pipe : public PipeRead, public PipeWrite {
public:
#if		defined(OS_UNIX)
	typedef int		desc;
#elif	defined(OS_WIN32)
	typedef HANDLE	desc;
#endif
private:
	Pope(desc r, desc w);
public:
	Pipe();
	~Pipe() {};

	void open();
	PipeRead	toPipeRead();
	PipeWrite	toPipeWrite();
};
#endif



class Pipe {
public:
#if		defined(OS_UNIX)
	typedef int		desc;
#elif	defined(OS_WIN32)
	typedef HANDLE	desc;
#endif
private:
	desc		readend;
	desc		writend;

public:
	enum PipeSide {
		ReadSide,
		WriteSide,
	};

	class Exception: public runtime_error {
	public:
		Exception(const string& text);
		~Exception();
	};

	Pipe(desc r = INVALID_HANDLE_VALUE, desc w = INVALID_HANDLE_VALUE);
	~Pipe();


	int	write(const void* buffer, int size);
	int	read(void* buffer, int size);

	void open() throw (Exception);
	Pipe toReadPipe() ;
	Pipe toWritePipe() ;

	void close() throw (Exception);
	void closeReadEndPoint() throw (Exception);
	void closeWriteEndPoint() throw (Exception);

	desc& in() { return readend; }
	desc& out(){ return writend; }

	bool valid(PipeSide s) const;
};

#endif
