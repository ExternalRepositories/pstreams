#ifndef	_Pipe_h_
#define _Pipe_h_

#include <stdexcept>

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
	#pragma warning(disable: 4290)
	typedef	void* HANDLE;
	typedef long  LONG_PTR;

	#ifdef INVALID_HANDLE_VALUE
		#undef INVALID_HANDLE_VALUE
		#define INVALID_HANDLE_VALUE  ((HANDLE)((LONG_PTR)-1))
	#endif
#endif

using namespace std;

class Pipe {
public:
#if		defined(REDI_OS_UNIX)
	typedef int		desc;
#elif	defined(REDI_OS_WIN32)
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

	class Exception: public exception {
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

	const desc& in() const { return readend; }
	const desc& out() const { return writend; }

	bool valid(PipeSide s) const;
};

#endif
