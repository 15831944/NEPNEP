#include "..\util\network\SocketIO.h"

class socketClass : public SocketSpace::SocketIOClass
{
public:
	socketClass();
	~socketClass();
	void onProcess(void *) override;
	static socketClass singleton;
	static socketClass* Instance()  { return &singleton; };
};
#ifdef SocketIO
#undef SocketIO
#endif
#define sockInst() socketClass::Instance()