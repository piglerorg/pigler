/**
 * Copyright (c) 2023-2024 Arman Jussupgaliyev
 */

#include "org_pigler_api_PiglerAPI.h"
#include "PiglerJavaAPI.h"
#include "JniEnvWrapper.h"
#include "javacommonutils.h"
#include "s60commonutils.h"
#include <akndiscreetpopup.h>
#include <avkon.hrh>
#include <eikenv.h>
#include "CMidToolkit.h"

CPiglerJavaAPI::CPiglerJavaAPI(CPiglerJavaEventSource* aEventSource):
	iEventSource(aEventSource)
{
}

CPiglerJavaAPI::~CPiglerJavaAPI()
{
	delete iApi;
	iApi = NULL;
}

void CPiglerJavaAPI::ConstructL(jobject aPeer)
{
	iApi = new PiglerAPI;
	iApi->SetTapHandler(this);
	iPeer = aPeer;
	JNIEnv* jniEnv = JniEnvWrapper::GetValidJniRef();
	jclass classRef = jniEnv->GetObjectClass(aPeer);
	iMethod = jniEnv->GetMethodID(classRef, "notificationCallback", "(I)V");
}

void CPiglerJavaAPI::HandleTap(TInt aUid)
{
	CPiglerJavaEvent* javaEvent = new (ELeave) CPiglerJavaEvent(iPeer, iMethod, aUid);
	iEventSource->PostEvent(javaEvent, CJavaEventBase::EEventPriority);
}

LOCAL_C void Construct(CPiglerJavaAPI *self, jobject aPeer)
{
	self->ConstructL(aPeer);
}

void CPiglerJavaAPI::NewL(CPiglerJavaEventSource* aEventSource, jobject aPeer, TInt* aHandle)
{
	CPiglerJavaAPI* self = new (ELeave) CPiglerJavaAPI(aEventSource);
	CleanupStack::PushL(self);
	aEventSource->Execute(&Construct, self, aPeer);
	CleanupStack::Pop(self);
	*aHandle = JavaMakeHandle(self);
}

LOCAL_C void Dispose(CPiglerJavaAPI* aApi)
{
	delete aApi;
}

// JNI implementation

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1createEventSource(JNIEnv *aEnv, jobject aThis, jint aEventServerHandle)
{
	TInt eventSourceHandle = CPiglerJavaEventSource::New(*aEnv, aThis, aEventServerHandle);
	return eventSourceHandle;
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1createAPI(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle)
{
	jobject peer = aEnv->NewWeakGlobalRef(aThis);
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	TInt handle = 0;
	TRAPD(err, CPiglerJavaAPI::NewL(eventSource, peer, &handle))
	if(err != KErrNone) {
		aEnv->DeleteWeakGlobalRef((jweak)peer);
		return err;
	}
	return handle;
}

JNIEXPORT void JNICALL Java_org_pigler_api_PiglerAPI__1dispose(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	eventSource->Execute(&Dispose, api);
	//CallMethod(Dispose, api, server);
	eventSource->Dispose(*aEnv);
}

LOCAL_C void SetAppId(PiglerAPI* aApi, TInt aAppId)
{
	aApi->SetAppId(aAppId);
}

LOCAL_C void Init1(PiglerAPI* aApi, TInt* aRes)
{
	*aRes = aApi->Init();
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1initRandom(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle, jint aAppId)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res = KErrNone;
	eventSource->Execute(&SetAppId, api->iApi, aAppId);
	eventSource->Execute(&Init1, api->iApi, &res);
	return res;
}

LOCAL_C void Init2(PiglerAPI* aApi, JNIEnv *aEnv, jstring aAppName, TInt* aRes)
{
	TBuf<64> buf = jstringToTBuf64(aEnv, aAppName);
	*aRes = aApi->Init(buf);
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1init(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle, jint aAppId, jstring aAppName)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res = KErrNone;
	eventSource->Execute(&SetAppId, api->iApi, aAppId);
	eventSource->Execute(&Init2, api->iApi, aEnv, aAppName, &res);
	return res;
}

LOCAL_C void SetNotification(PiglerAPI* aApi, JNIEnv *aEnv, TInt aUid, jstring aText, TInt* aRes)
{
	TBuf<256> buf = jstringToTBuf256(aEnv, aText);
	*aRes = aApi->SetNotification(aUid, buf);
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1setNotification(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle, jint aUid, jstring aText)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res;
	eventSource->Execute(&SetNotification, api->iApi, aEnv, aUid, aText, &res);
	return res;
}

LOCAL_C void Remove(PiglerAPI* aApi, TInt aUid, TInt* aRes)
{
	*aRes = aApi->RemoveNotification(aUid);
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1removeNotification(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle, jint aUid)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res;
	eventSource->Execute(&Remove, api->iApi, aUid, &res);
	return res;
}

LOCAL_C void RemoveAll(PiglerAPI* aApi, TInt* aRes)
{
	*aRes = aApi->RemoveAllNotifications();
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1removeAllNotifications(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res = KErrNone;
	eventSource->Execute(&RemoveAll, api->iApi, &res);
	return res;
}

LOCAL_C void GetLastTappedNotification(PiglerAPI* aApi, TInt* aRes)
{
	*aRes = aApi->GetLastTappedNotification();
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1getLastTappedNotification(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res = KErrNone;
	eventSource->Execute(&GetLastTappedNotification, api->iApi, &res);
	return res;
}

LOCAL_C void SetRemoveOnTap(PiglerAPI* aApi, TInt aUid, TBool aRemove, TInt* aRes)
{
	*aRes = aApi->SetRemoveOnTap(aUid, aRemove);
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1setRemoveNotificationOnTap(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle, jint aUid, jboolean aRemove)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res;
	eventSource->Execute(&SetRemoveOnTap, api->iApi, (TInt) aUid, (TBool) aRemove, &res);
	return res;
}

LOCAL_C void SetIcon(PiglerAPI* aApi, TInt aUid, int *rgb, int size, TInt* aRes)
{
	TPtrC8 bitmap((const TUint8 *) rgb, size * 4);
	*aRes = aApi->SetNotificationIcon(aUid, bitmap);
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1setNotificationIcon(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle, jint aUid, jintArray aRgb)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	
	int size = aEnv->GetArrayLength(aRgb);
	jboolean iscopy;
	int *rgb = aEnv->GetIntArrayElements(aRgb, &iscopy);
	
	TInt res;
	eventSource->Execute(&SetIcon, api->iApi, (TInt) aUid, rgb, size, &res);
	
	aEnv->ReleaseIntArrayElements(aRgb, rgb, 0);
	return res;
}

LOCAL_C void Close(PiglerAPI* aApi)
{
	aApi->Close();
}

JNIEXPORT void JNICALL Java_org_pigler_api_PiglerAPI__1close(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	eventSource->Execute(&Close, api->iApi);
}

LOCAL_C void GetAPIVersion(PiglerAPI* aApi, TInt* aRes)
{
	*aRes = aApi->GetAPIVersion();
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1getAPIVersion(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res;
	eventSource->Execute(&GetAPIVersion, api->iApi, &res);
	return res;
}

LOCAL_C void SetLaunchOnTap(PiglerAPI* aApi, TInt aUid, TBool aLaunch, TInt* aRes)
{
	*aRes = aApi->SetLaunchAppOnTap(aUid, aLaunch);
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1setLaunchAppOnTap(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle, jint aUid, jboolean aLaunch)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res;
	eventSource->Execute(&SetLaunchOnTap, api->iApi, aUid, (TBool) aLaunch, &res);
	return res;
}

LOCAL_C void GetNotificationsCount(PiglerAPI* aApi, TInt* aRes)
{
	*aRes = aApi->GetNotificationsCount();
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1getNotificationsCount(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res;
	eventSource->Execute(&GetNotificationsCount, api->iApi, &res);
	return res;
}

LOCAL_C void GetMaxNotificationsCount(PiglerAPI* aApi, TInt* aRes)
{
	*aRes = aApi->GetMaxNotificationsCount();
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1getMaxNotificationsCount(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res;
	eventSource->Execute(&GetMaxNotificationsCount, api->iApi, &res);
	return res;
}

LOCAL_C void GetGlobalNotificationsCount(PiglerAPI* aApi, TInt* aRes)
{
	*aRes = aApi->GetGlobalNotificationsCount();
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1getGlobalNotificationsCount(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res;
	eventSource->Execute(&GetGlobalNotificationsCount, api->iApi, &res);
	return res;
}

LOCAL_C void GetBitmapSize(PiglerAPI* aApi, TInt* aRes)
{
	*aRes = aApi->GetBitmapSize();
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1getBitmapSize(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res;
	eventSource->Execute(&GetBitmapSize, api->iApi, &res);
	return res;
}

LOCAL_C void StartAnnaServer(PiglerAPI* aApi, TInt* aRes)
{
	*aRes = aApi->StartAnnaServer();
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1startAnnaServer(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aHandle)
{
	CPiglerJavaEventSource* eventSource = JavaUnhand<CPiglerJavaEventSource>(aEventSourceHandle);
	CPiglerJavaAPI* api = JavaUnhand<CPiglerJavaAPI>(aHandle);
	TInt res;
	eventSource->Execute(&StartAnnaServer, api->iApi, &res);
	return res;
}

LOCAL_C void ShowGlobalDiscreetPopup(TInt* aRes,const TDesC* aTitle, const TDesC* aText, TInt aFlags, TInt aUid)
{
	TInt res(0);
	TRAPD(error,
		res = CAknDiscreetPopup::ShowGlobalPopupL(*aTitle, *aText,
			KAknsIIDNone, KNullDesC, 0, 0, aFlags, 0, NULL, TUid::Uid(aUid));
	);
	if (error < KErrNone) {
		*aRes = error;
		return;
	}
	*aRes = res;
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1showGlobalDiscreetPopup
(JNIEnv *aEnv, jobject aThis, jint aEventSourceHandle, jint aToolkitHandle, jstring aTitle, jstring aText, jint aFlags, jint aUid)
{
	CMIDToolkit* toolkit = JavaUnhand<CMIDToolkit>(aToolkitHandle);
	RJString title(*aEnv, aTitle);
	RJString text(*aEnv, aText);
	
	TInt res(0);
	toolkit->Execute(&ShowGlobalDiscreetPopup, &res,
		(const TDesC*)&title,
		(const TDesC*)&text,
		aFlags, aUid
		);
	return res;
}

// Utils

TBuf<64> jstringToTBuf64(JNIEnv* aEnv, jstring aJstring) {
	jboolean iscopy;
	const char* utf8 = aEnv->GetStringUTFChars(aJstring, &iscopy);
	HBufC* hbuf = java::util::S60CommonUtils::wstringToDes(java::util::JavaCommonUtils::utf8ToWstring(utf8).c_str());
	TBuf<64> tbuf;
	tbuf.Copy(*hbuf);
	aEnv->ReleaseStringUTFChars(aJstring, utf8);
	delete hbuf;
	return tbuf;
}

TBuf<256> jstringToTBuf256(JNIEnv* aEnv, jstring aJstring) {
	jboolean iscopy;
	const char* utf8 = aEnv->GetStringUTFChars(aJstring, &iscopy);
	HBufC* hbuf = java::util::S60CommonUtils::wstringToDes(java::util::JavaCommonUtils::utf8ToWstring(utf8).c_str());
	TBuf<256> tbuf;
	tbuf.Copy(*hbuf);
	aEnv->ReleaseStringUTFChars(aJstring, utf8);
	delete hbuf;
	return tbuf;
}
