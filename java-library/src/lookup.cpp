/*
 * Description: Automatically generated JNI lookup file. Do not modify manually.
 */

#include "javasymbianoslayer.h"
typedef void (*TFunc)();
#include "org_pigler_api_PiglerAPI.h"
const FuncTable funcTable[] = {
   { "Java_org_pigler_api_PiglerAPI__1close", (unsigned int) Java_org_pigler_api_PiglerAPI__1close},
   { "Java_org_pigler_api_PiglerAPI__1createAPI", (unsigned int) Java_org_pigler_api_PiglerAPI__1createAPI},
   { "Java_org_pigler_api_PiglerAPI__1createFunctionServer", (unsigned int) Java_org_pigler_api_PiglerAPI__1createFunctionServer},
   { "Java_org_pigler_api_PiglerAPI__1dispose", (unsigned int) Java_org_pigler_api_PiglerAPI__1dispose},
   { "Java_org_pigler_api_PiglerAPI__1getAPIVersion", (unsigned int) Java_org_pigler_api_PiglerAPI__1getAPIVersion},
   { "Java_org_pigler_api_PiglerAPI__1getLastTappedNotification", (unsigned int) Java_org_pigler_api_PiglerAPI__1getLastTappedNotification},
   { "Java_org_pigler_api_PiglerAPI__1init", (unsigned int) Java_org_pigler_api_PiglerAPI__1init},
   { "Java_org_pigler_api_PiglerAPI__1initRandom", (unsigned int) Java_org_pigler_api_PiglerAPI__1initRandom},
   { "Java_org_pigler_api_PiglerAPI__1removeAllNotifications", (unsigned int) Java_org_pigler_api_PiglerAPI__1removeAllNotifications},
   { "Java_org_pigler_api_PiglerAPI__1removeNotification", (unsigned int) Java_org_pigler_api_PiglerAPI__1removeNotification},
   { "Java_org_pigler_api_PiglerAPI__1setLaunchAppOnTap", (unsigned int) Java_org_pigler_api_PiglerAPI__1setLaunchAppOnTap},
   { "Java_org_pigler_api_PiglerAPI__1setNotification", (unsigned int) Java_org_pigler_api_PiglerAPI__1setNotification},
   { "Java_org_pigler_api_PiglerAPI__1setNotificationIcon", (unsigned int) Java_org_pigler_api_PiglerAPI__1setNotificationIcon},
   { "Java_org_pigler_api_PiglerAPI__1setRemoveNotificationOnTap", (unsigned int) Java_org_pigler_api_PiglerAPI__1setRemoveNotificationOnTap}
};

IMPORT_C TFunc jni_lookup(const char* name);
EXPORT_C TFunc jni_lookup(const char* name) {
    return (TFunc)findMethod(name, funcTable, sizeof(funcTable)/sizeof(FuncTable));
}
