/**
 * Copyright (c) 2023 Arman Jussupgaliyev
 */

#include "PiglerJavaEvent.h"

CPiglerJavaEvent::CPiglerJavaEvent(jobject aObject, jmethodID aMethod, TInt aUid) :
		iObject(aObject),
		iMethod(aMethod),
		iUid(aUid)
{
}

void CPiglerJavaEvent::Dispatch(JNIEnv& aJniEnv)
{
	aJniEnv.CallVoidMethod(iObject, iMethod, iUid);
}
