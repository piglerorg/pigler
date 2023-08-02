#ifndef PIGLERJAVAEVENTSOURCE
#define PIGLERJAVAEVENTSOURCE
#include <mevents.h>

NONSHARABLE_CLASS(CPiglerJavaEventSource):
	public CJavaEventSource<CPiglerJavaEventSource>
{
public:
	CPiglerJavaEventSource();

	void ConstructL(JNIEnv& aJni, jobject aPeer, TJavaEventServer aServer);
	virtual ~CPiglerJavaEventSource();
private:
	void FinalizeSvr();
};
#endif
