#ifndef PIGLERJAVEEVENT
#define PIGLERJAVEEVENT
#include <mevents.h>
#include "PiglerJavaEventSource.h"

NONSHARABLE_CLASS(CPiglerJavaEvent) :
		public CJavaEvent<CPiglerJavaEventSource>
{
public:
	CPiglerJavaEvent(jobject aObject, jmethodID aMethod, TInt aUid);
private:
	void Dispatch(JNIEnv& aJni);
	jobject iObject;
	jmethodID iMethod;
	TInt iUid;
};
#endif
