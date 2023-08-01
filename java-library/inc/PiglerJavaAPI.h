#include <e32base.h>
#include "jni.h"
#include "monitor.h"
#include "functionserver.h"
#include "legacyeventserverwrapper.h"

class PiglerAPI;

class PiglerFunctionServer: public java::util::FunctionServer, public LegacyEventServerWrapper {
public:
	PiglerFunctionServer(JNIEnv& aJni, jobject aPeer);
	static TInt NewL(JNIEnv& aJni, jobject aPeer);
	void ConstructL(JNIEnv& aJni, jobject aPeer);
    JNIEnv* getValidJniEnv();
	jobject getPeer();
	~PiglerFunctionServer();
	java::util::FunctionServer* getFunctionServer() const;

private:
	void doServerSideInit();
	void vmDetached();
private:
	bool mVmAttached;
	java::util::FunctionServer* iServer;
};

class CPiglerJavaAPI {
public:
	static void NewL(PiglerFunctionServer* aFunctionServer, TInt* aHandle);
	~CPiglerJavaAPI();
	PiglerAPI *iApi;
	PiglerFunctionServer *iFunctionServer;
private:
	CPiglerJavaAPI(PiglerFunctionServer* aFunctionServer);
	void ConstructL();
};

TBuf<64> jstringToTBuf64(JNIEnv*, jstring);
TBuf<256> jstringToTBuf256(JNIEnv*, jstring);
