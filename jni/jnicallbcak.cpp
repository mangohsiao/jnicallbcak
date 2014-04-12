
#include <jni.h>

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

#define mLOG(X) __android_log_print(ANDROID_LOG_WARN,"jniLog","%s\n",X)

#define mLOG_I(X,Y) __android_log_print(ANDROID_LOG_WARN,"jniLog","%s%d\n",X, Y)
#define mLOG_LU(X,Y) __android_log_print(ANDROID_LOG_WARN,"jniLog","%s%lu\n",X, Y)

pthread_t tid_01 = 0;
void* handler01(void* arg);
void handle_sig(int sig_num);
void android_sigcation(int signal, siginfo_t *info, void *reserved);

JavaVM * mVm = NULL;
jobject glbThiz = NULL;

bool show = false;
pthread_cond_t ui_ready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void mango(JNIEnv* env, jobject thiz);
void mango_continue(JNIEnv* env, jobject thiz);
void mango_stop(JNIEnv* env, jobject thiz);

static JNINativeMethod gMethods[] = {
		{
			"mangoInit","()V",(void*)mango
		},
		{
			"mangoStop","()V",(void*)mango_stop
		},
		{
			"mangoContinue","()V",(void*)mango_continue
		}
};

//MGNetClient *client;

#define JNIREG_CLASS "com/example/jnicallbcak/Mango"
void mango(JNIEnv* env, jobject thiz){
	mLOG("call mango().");
	glbThiz = env->functions->NewGlobalRef(env,thiz);
	if(tid_01 == 0){

		struct sigaction actions, oldact;
		memset(&actions,0,sizeof(actions));
//		sigemptyset(&actions.sa_mask);
		actions.sa_flags = 0;
		actions.sa_handler = handle_sig;
//		actions.sa_sigaction = android_sigcation;
		int rc = sigaction(SIGUSR2, &actions, &oldact);
		mLOG_I("rc = ", rc);
		mLOG("rc gogogo.");

		int rtvl = pthread_create(&tid_01, NULL, handler01, NULL);
		mLOG_I("creating...", rtvl);
	}else{
		mLOG("not created...");
	}


	return;
}

void mango_continue(JNIEnv* env, jobject thiz){
	pthread_mutex_lock(&lock);
	glbThiz = env->functions->NewGlobalRef(env,thiz);
	pthread_mutex_unlock(&lock);
	pthread_cond_signal(&ui_ready);
	mLOG("thread attach...");

	mango(env,thiz);
}

void mango_stop(JNIEnv* env, jobject thiz){
	mLOG_LU("stoping...id = ",pthread_self());
	mLOG_LU("stoping...thread = ",tid_01);
	int rc = pthread_kill(tid_01, SIGUSR2);
	mLOG_I("stoping...rc = ",rc);
//	rc = pthread_kill(pthread_self(), SIGUSR1);
//	mLOG_I("stoping...rc = ",rc);
	void *status;
	rc = pthread_join(tid_01,&status);
	mLOG_I("join rc = ", rc);

//	pthread_mutex_lock(&lock);
//	glbThiz = NULL;
//	pthread_mutex_unlock(&lock);
//	mLOG("glbThiz == NULL...");
//	pthread_join(tid_01,NULL);
}


void* handler01(void* arg){
	mLOG_LU("in hanler01 id = ", pthread_self());

	int rtvl = -1;
	JNIEnv *mEnv = NULL;
	if(mVm == NULL){
		return NULL;
	}


	int count = 0;
	jclass glbJcls = NULL;
	jmethodID glbJmid = NULL;
	while(1){
		rtvl = mVm->AttachCurrentThread(&mEnv, NULL);
		if(rtvl < 0){
			mLOG("attach failed...");
			return NULL;
		}
		mLOG("thread attach...");
		glbJcls = mEnv->functions->GetObjectClass(mEnv,glbThiz);
		glbJmid = mEnv->functions->GetMethodID(mEnv,glbJcls, "mangoCallBack", "(I)V");

		while(1){
			pthread_mutex_lock(&lock);
			if(glbThiz == NULL){
				mVm->DetachCurrentThread();
				mLOG("waiting...");
				pthread_cond_wait(&ui_ready,&lock);
				pthread_mutex_unlock(&lock);
				break;
			}else{
//				mLOG("in else...");
			}
			pthread_mutex_unlock(&lock);
//			mLOG_I("glbThiz != NULL  count = ", count);
			mEnv->functions->CallVoidMethod(mEnv, glbThiz, glbJmid, count++);
			sleep(1);
		}
		mLOG("out of inWhile...");
	}
	return (void*)NULL;
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


void handle_sig(int sig){
	mLOG("in handle_sig().");
	if(sig == SIGUSR2){
		mLOG("get signal SIGUSR2.");
		if(mVm != NULL){
			mVm->DetachCurrentThread();
		}
		mLOG("DetachCurrentThread.");
		pthread_exit((void*)19);
	}else {
		mLOG("get signal other.");
	}
}


void android_sigcation(int signal, siginfo_t *info, void *reserved){
	mLOG("in android_sigcation().");
	if(signal == SIGUSR1){
		mLOG("get signal SIGUSR1.");
//		mVm->DetachCurrentThread();
//		pthread_exit((void*)0);
	}else {
		mLOG("get signal other.");
	}
}


