#include <e32base.h>
#include <e32msgqueue.h>

class MPiglerMsgQueueListener {
	
};

class CPiglerMsgQueue: public CActive, public RMsgQueueBase {
public:
	CPiglerMsgQueue(TDesC aName);
	~CPiglerMsgQueue();
	virtual void RegisterNotifier();
	void StartListening();
protected:
	void (*iCancel)();
	TRequestStatus iRequestStatus;
};

class CPiglerMsgQueueReader: public CPiglerMsgQueue {
public:
	CPiglerMsgQueueReader(TDesC aName, MPiglerMsgQueueListener* aListener);
	~CPiglerMsgQueueReader();
	void DoCancel();
	void RunL();
	void RegisterNotifier();
private:
	int* iBuf[];
	MPiglerMsgQueueListener* iListener;
};

class CPiglerMsgQueueWriter: public CPiglerMsgQueue {
public:
	CPiglerMsgQueueWriter(TDesC aName);
	void DoCancel();
	void RunL();
	void RegisterNotifier();
	void SendData();
	void SendAllData();
};
