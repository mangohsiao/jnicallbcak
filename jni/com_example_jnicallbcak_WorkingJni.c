
#include "android/log.h"
#include "com_example_jnicallbcak_WorkingJni.h"

#include <stdio.h>


JNIEXPORT void JNICALL Java_com_example_jnicallbcak_WorkingJni_myJni (JNIEnv *env, jobject obj){

	char tChar[256];

	jclass jcls = (*env)->GetObjectClass(env,obj);
	jmethodID jmid = (*env)->GetMethodID(env,jcls, "callBackFunc", "(Ljava/lang/String;)V");
	if(jmid == 0){
		__android_log_print(ANDROID_LOG_WARN,"jniLog","%s","jmid == 0");
	}else{
		__android_log_print(ANDROID_LOG_WARN,"jniLog","%s","jmid != 0");
	}

	char c = 0x8f;
	__android_log_print(ANDROID_LOG_WARN,"jniLog","%d",c);

	int count = 0;

	while(1){

		sprintf(tChar, "num: %d", count++);
		__android_log_print(ANDROID_LOG_WARN,"jniLog","%s",tChar);
		jstring str = (*env)->NewStringUTF(env, tChar);
		(*env)->CallVoidMethod(env, obj, jmid, str);
		sleep(3);
	}


}
