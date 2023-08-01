#include <e32base.h>
#include "org_pigler_api_PiglerAPI.h"
#include "fs_methodcall.h"
#include "PiglerJavaAPI.h"
#include "PiglerAPI.h"
#include "javasymbianoslayer.h"
#include "s60commonutils.h"

PiglerFunctionServer::PiglerFunctionServer(JNIEnv& aJni, jobject aPeer):
	java::util::FunctionServer("PiglerFunctionServer")
{
	createServerToNewThread();
	attachToVm(aJni, aPeer);
	mVmAttached = true;
	iServer = reinterpret_cast<java::util::FunctionServer*>(this);
}

JNIEnv* PiglerFunctionServer::getValidJniEnv()
{
	return mJniEnv;
}

jobject PiglerFunctionServer::getPeer()
{
	return mJavaPeerObject;
}

PiglerFunctionServer::~PiglerFunctionServer()
{
	if (mVmAttached) {
		detachFromVm();
	}
	stopServer();
}

java::util::FunctionServer* PiglerFunctionServer::getFunctionServer() const
{
	return iServer;
}

void PiglerFunctionServer::doServerSideInit()
{
	FunctionServer::doServerSideInit();
}

void PiglerFunctionServer::vmDetached()
{
	FunctionServer::vmDetached();
}

CPiglerJavaAPI::CPiglerJavaAPI(PiglerFunctionServer* aFunctionServer) :
		iFunctionServer(aFunctionServer)
{
}

CPiglerJavaAPI::~CPiglerJavaAPI()
{
    delete iApi;
    iApi = NULL;
}

void CPiglerJavaAPI::ConstructL()
{
    iApi = new PiglerAPI();
}

void CPiglerJavaAPI::NewL(PiglerFunctionServer* aFunctionServer, TInt* aHandle)
{
	CPiglerJavaAPI* self = new (ELeave) CPiglerJavaAPI(aFunctionServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CallMethodL(self, &CPiglerJavaAPI::ConstructL, self->iFunctionServer);
	CleanupStack::Pop(self);
	*aHandle = reinterpret_cast<TInt>(self);
}

LOCAL_C void Dispose(CPiglerJavaAPI* aApi)
{
    delete aApi;
}

// JNI implementation

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1createFunctionServer(JNIEnv *aEnv, jobject aThis)
{
	PiglerFunctionServer* functionServer = new PiglerFunctionServer(*aEnv, aThis);
	return reinterpret_cast<jint>(functionServer);
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1createAPI(JNIEnv *aEnv, jobject aThis, jint aServerHandle)
{
    PiglerFunctionServer *server = reinterpret_cast<PiglerFunctionServer *>(aServerHandle);
	TInt handle = 0;
	TRAPD(err, CPiglerJavaAPI::NewL(server, &handle))
	if(err != KErrNone) {
		return err;
	}
	return handle;
}

JNIEXPORT void JNICALL Java_org_pigler_api_PiglerAPI__1dispose(JNIEnv *aEnv, jobject aThis, jint aServerHandle, jint aHandle)
{
    PiglerFunctionServer *server = reinterpret_cast<PiglerFunctionServer *>(aServerHandle);
	CPiglerJavaAPI* api = reinterpret_cast<CPiglerJavaAPI *>(aHandle);
	CallMethod(Dispose, api, server);
	delete server;
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1initRandom(JNIEnv *aEnv, jobject aThis, jint aServerHandle, jint aHandle)
{
    PiglerFunctionServer *server = reinterpret_cast<PiglerFunctionServer *>(aServerHandle);
	CPiglerJavaAPI* api = reinterpret_cast<CPiglerJavaAPI *>(aHandle);
	TInt res = KErrNone;
	CallMethod(res, api->iApi, &PiglerAPI::Init, server);
	return res;
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1init(JNIEnv *aEnv, jobject aThis, jint aServerHandle, jint aHandle, jstring aAppName)
{
    PiglerFunctionServer *server = reinterpret_cast<PiglerFunctionServer *>(aServerHandle);
	CPiglerJavaAPI* api = reinterpret_cast<CPiglerJavaAPI *>(aHandle);
	TInt res = KErrNone;
	TBuf<64> tbuf = jstringToTBuf64(aEnv, aAppName);
	CallMethod(res, api->iApi, &PiglerAPI::Init, tbuf, server);
	return res;
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1setNotification(JNIEnv *aEnv, jobject aThis, jint aServerHandle, jint aHandle, jint aUid, jstring aText)
{
    PiglerFunctionServer *server = reinterpret_cast<PiglerFunctionServer *>(aServerHandle);
	CPiglerJavaAPI* api = reinterpret_cast<CPiglerJavaAPI *>(aHandle);
	TInt res = aUid;
	TBuf<256> tbuf = jstringToTBuf256(aEnv, aText);
	CallMethod(res, api->iApi, &PiglerAPI::SetNotification, res, tbuf, server);
	return res;
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1removeNotification(JNIEnv *aEnv, jobject aThis, jint aServerHandle, jint aHandle, jint aUid)
{
    PiglerFunctionServer *server = reinterpret_cast<PiglerFunctionServer *>(aServerHandle);
	CPiglerJavaAPI* api = reinterpret_cast<CPiglerJavaAPI *>(aHandle);
	TInt res = aUid;
	CallMethod(res, api->iApi, &PiglerAPI::RemoveNotification, res, server);
	return res;
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1removeAllNotifications(JNIEnv *aEnv, jobject aThis, jint aServerHandle, jint aHandle)
{
    PiglerFunctionServer *server = reinterpret_cast<PiglerFunctionServer *>(aServerHandle);
	CPiglerJavaAPI* api = reinterpret_cast<CPiglerJavaAPI *>(aHandle);
	TInt res = KErrNone;
	CallMethod(res, api->iApi, &PiglerAPI::RemoveAllNotifications, server);
	return res;
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1getLastTappedNotification(JNIEnv *aEnv, jobject aThis, jint aServerHandle, jint aHandle)
{
    PiglerFunctionServer *server = reinterpret_cast<PiglerFunctionServer *>(aServerHandle);
	CPiglerJavaAPI* api = reinterpret_cast<CPiglerJavaAPI *>(aHandle);
	TInt res = KErrNone;
	CallMethod(res, api->iApi, &PiglerAPI::GetLastTappedNotification, server);
	return res;
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1setRemoveNotificationOnTap(JNIEnv *aEnv, jobject aThis, jint aServerHandle, jint aHandle, jint aUid, jboolean aRemove)
{
    PiglerFunctionServer *server = reinterpret_cast<PiglerFunctionServer *>(aServerHandle);
	CPiglerJavaAPI* api = reinterpret_cast<CPiglerJavaAPI *>(aHandle);
	TInt res = aUid;
	TBool remove = aRemove;
	CallMethod(res, api->iApi, &PiglerAPI::SetRemoveNotificationOnTap, res, remove, server);
	return res;
}

JNIEXPORT jint JNICALL Java_org_pigler_api_PiglerAPI__1setNotificationIcon(JNIEnv *aEnv, jobject aThis, jint aServerHandle, jint aHandle, jint aUid, jintArray aRgb)
{
    PiglerFunctionServer *server = reinterpret_cast<PiglerFunctionServer *>(aServerHandle);
	CPiglerJavaAPI* api = reinterpret_cast<CPiglerJavaAPI *>(aHandle);
	int size = aEnv->GetArrayLength(aRgb);
	jboolean iscopy;
	int *rgb = aEnv->GetIntArrayElements(aRgb, &iscopy);
	TPtrC8 iconBitmap((const TUint8 *) rgb, size * 4);
	TInt res = aUid;
	CallMethod(res, api->iApi, &PiglerAPI::SetNotificationIcon, res, iconBitmap, server);
	return res;
}

JNIEXPORT void JNICALL Java_org_pigler_api_PiglerAPI__1close(JNIEnv *aEnv, jobject aThis, jint aServerHandle, jint aHandle)
{
    PiglerFunctionServer *server = reinterpret_cast<PiglerFunctionServer *>(aServerHandle);
	CPiglerJavaAPI* api = reinterpret_cast<CPiglerJavaAPI *>(aHandle);
	CallMethod(api->iApi, &PiglerAPI::Close, server);
}

TBuf<64> jstringToTBuf64(JNIEnv* aEnv, jstring aJstring) {
	jboolean iscopy;
	const char* utf8 = aEnv->GetStringUTFChars(aJstring, &iscopy);
	HBufC* hbuf = java::util::S60CommonUtils::wstringToDes(java::util::JavaCommonUtils::utf8ToWstring(utf8).c_str());
	TBuf<64> tbuf;
	tbuf.Copy(*hbuf);
	delete hbuf;
	return tbuf;
}

TBuf<256> jstringToTBuf256(JNIEnv* aEnv, jstring aJstring) {
	jboolean iscopy;
	const char* utf8 = aEnv->GetStringUTFChars(aJstring, &iscopy);
	HBufC* hbuf = java::util::S60CommonUtils::wstringToDes(java::util::JavaCommonUtils::utf8ToWstring(utf8).c_str());
	TBuf<256> tbuf;
	tbuf.Copy(*hbuf);
	delete hbuf;
	return tbuf;
}
