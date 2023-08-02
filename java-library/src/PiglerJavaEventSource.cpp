#include "PiglerJavaEventSource.h"

CPiglerJavaEventSource::CPiglerJavaEventSource()
{
}

void CPiglerJavaEventSource::ConstructL(JNIEnv& aJni, jobject aPeer, TJavaEventServer aServer)
{
    CJavaEventSourceBase::ConstructL(aJni, aPeer, aServer);
}

CPiglerJavaEventSource::~CPiglerJavaEventSource()
{
}

void CPiglerJavaEventSource::FinalizeSvr()
{
}
