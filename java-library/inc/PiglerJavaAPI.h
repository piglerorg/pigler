/**
 * Copyright (c) 2023 Arman Jussupgaliyev
 */

#include <e32base.h>
#include "jni.h"
#include "jutils.h"
#include "PiglerAPI.h"
#include "PiglerJavaEventSource.h"
#include "PiglerJavaEvent.h"

class PiglerAPI;

NONSHARABLE_CLASS(CPiglerJavaAPI):
	public IPiglerTapHandler
{
public:
	static void NewL(CPiglerJavaEventSource* aEventSource, jobject aPeer, TInt* aHandle);
	~CPiglerJavaAPI();
	PiglerAPI *iApi;
	virtual void HandleTap(TInt aUid);
	void ConstructL(jobject aPeer);
private:
	CPiglerJavaAPI(CPiglerJavaEventSource* aEventSource);
	CPiglerJavaEventSource* iEventSource;
	jobject iPeer;
	jmethodID iMethod;
};

TBuf<64> jstringToTBuf64(JNIEnv*, jstring);
TBuf<256> jstringToTBuf256(JNIEnv*, jstring);
