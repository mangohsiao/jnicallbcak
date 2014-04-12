
#include <jni.h>

#include "MGNet.h"

#ifdef __cplusplus
extern "C"
{
#endif


#include <android/log.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#ifdef __cplusplus
}
#endif

using namespace mango;
using namespace std;

#define mLOG(X) __android_log_print(ANDROID_LOG_WARN,"jniLog","%s\n",X)

#define mLOG_I(X,Y) __android_log_print(ANDROID_LOG_WARN,"jniLog","%s %d\n",X, Y)

void mango_init(JNIEnv* env, jobject thiz);
void mango_continue(JNIEnv* env, jobject thiz);
void mango_stop(JNIEnv* env, jobject thiz);
void mango_recon(JNIEnv* env, jobject thiz);
void mango_send(JNIEnv* env, jobject thiz);
jboolean mango_check(JNIEnv* env, jobject thiz);

int onStateTrigger(int code);
void recv_callback(char *buf, int len);
int stat_callback(int status, int code);

MGNet *mNet;
int m_status = 0, count=0;
int stat_callback(int status, int code){
	m_status = status;
	mLOG_I("STATE: ",status);
	if(status == 400){
		//reconnect
		mNet->reconnectNet();
	}
	return status;
}

void recv_callback(char *buf, int len){
//	write(STDOUT_FILENO, buf, len);
	buf[len]='\0';
	mLOG(buf);
}

JavaVM * mVm = NULL;
jobject glbThiz = NULL;
pthread_cond_t ui_ready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static JNINativeMethod gMethods[] = {
		{
			"mangoInit","()V",(void*)mango_init
		},
		{
			"mangoStop","()V",(void*)mango_stop
		},
		{
			"mangoSend","()V",(void*)mango_send
		},
		{
			"mangoRecon","()V",(void*)mango_recon
		},
		{
			"mangoCheck","()Z",(void*)mango_check
		},
		{
			"mangoContinue","()V",(void*)mango_continue
		}
};

#define JNIREG_CLASS "com/example/jnicallbcak/Mango"
void mango_init(JNIEnv* env, jobject thiz){
	mLOG("call mango().");
	glbThiz = env->functions->NewGlobalRef(env,thiz);

	mNet = MGNet::ins();
	mNet->setRemoteIp("125.216.243.235");
	mNet->setRemotePort(8002);
	mNet->set_heart_break_str("#b\n");
	mNet->set_recv_callback(recv_callback);
	mNet->set_stat_callback(stat_callback);
	mNet->start();
//	n_ptr = new MGNet(8002,"125.216.243.235", 8992);
//	n_ptr->setOnStateTrigger(onStateTrigger);
//	n_ptr->start();
//	n_ptr->send("hi i'm mango");

	return;
}

void mango_continue(JNIEnv* env, jobject thiz){
/*	pthread_mutex_lock(&lock);
	glbThiz = env->functions->NewGlobalRef(env,thiz);
	pthread_mutex_unlock(&lock);
	pthread_cond_signal(&ui_ready);*/
//	mNet->ActReadThreadCmd(1);
	mLOG("thread attach...");
}

void mango_stop(JNIEnv* env, jobject thiz){
//	pthread_kill(tid_01, SIGUSR1);
	mLOG("stoping...");
	mNet->stop();
/*	pthread_mutex_lock(&lock);
	glbThiz = NULL;
	pthread_mutex_unlock(&lock);*/
	mLOG("glbThiz == NULL...");
//	pthread_join(tid_01,NULL);
}

void mango_recon(JNIEnv* env, jobject thiz){
	mNet->ActReadThreadCmd(1);
}

jboolean mango_check(JNIEnv* env, jobject thiz) {
	return (jboolean)mNet->checkIsConnect();
}

void mango_send(JNIEnv* env, jobject thiz){
	mLOG("send ..");
	string str("##\n");
	mNet->send(str);
}

static int register_mango_native(JNIEnv *env){
	jclass clazz = env->functions->FindClass(env,JNIREG_CLASS);
	if(clazz == NULL){
		mLOG("register clazz == null. ");
		return -1;
	}
	if(env->functions->RegisterNatives(env,clazz,gMethods,sizeof(gMethods)/sizeof(gMethods[0])) < 0){
		return -1;
	}
	return 0;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved){
	JNIEnv* env = NULL;

	if(vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK){
		return -1;
	}

	if(register_mango_native(env) < 0){
		return -2;
	}

	mVm = vm;
	return JNI_VERSION_1_4;
}

int onStateTrigger(int code){
	mLOG_I("code = ", code);
	return 0;
}
