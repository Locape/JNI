#include <jni.h>
#include <string>

#include <android/log.h>
#include <pthread.h>

#define LOG_TAG "System.out"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static JavaVM *JVM;
static jobject objectMainActivity;
static jmethodID methodCallMeBaby;


extern "C"
JNIEXPORT jstring JNICALL
Java_com_ray_myjnitest_MainActivity_helloJNI(JNIEnv *env, jobject instance) {

    std::string hello = "你好啊";


    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ray_myjnitest_JNI_callbackAdd(JNIEnv *env, jobject instance) {

    // 1.得到字节码
    jclass jclazz = env->FindClass((const char *) "com/ray/myjnitest/JNI");

    // 2.得到方法
    jmethodID jmethodIDs = env->GetMethodID(jclazz, "add", "(II)I");

    // 3.实例化该类
    jobject jobject1 = env->AllocObject(jclazz);

    // 4.调用方法
    jint value = env->CallIntMethod(jobject1, jmethodIDs, 99, 1);
    LOGE("value === %d\n", value);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_ray_myjnitest_JNI_callbackHelloFromJava(JNIEnv *env, jobject instance) {

    // 1.得到字节码
    jclass jclazz = env->FindClass((const char *) "com/ray/myjnitest/JNI");

    // 2.得到方法
    jmethodID jmethodIDs = env->GetMethodID(jclazz, "helloFromJava", "()V");

    // 3.实例化该类
    jobject jobject1 = env->AllocObject(jclazz);

    // 4.调用方法
    env->CallVoidMethod(jobject1, jmethodIDs);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ray_myjnitest_JNI_callbackPrintString(JNIEnv *env, jobject instance) {
    // 1.得到字节码
    jclass jclazz = env->FindClass((const char *) "com/ray/myjnitest/JNI");

    // 2.得到方法
    //最后一个参数是签名
    jmethodID jmethodIDs = env->GetMethodID(jclazz, "printString",
                                            (const char *) "(Ljava/lang/String;)V");

    // 3.实例化该类
    jobject jobject1 = env->AllocObject(jclazz);

    // 4.调用方法
    jstring jst = env->NewStringUTF("i am your mama!");
    env->CallVoidMethod(jobject1, jmethodIDs, jst);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_ray_myjnitest_JNI_callbackSayHello(JNIEnv *env, jobject instance) {

    // 1.得到字节码
    jclass jclazz = env->FindClass((const char *) "com/ray/myjnitest/JNI");

    // 2.得到方法
    //最后一个参数是签名
    jmethodID jmethodIDs = env->GetStaticMethodID(jclazz, "sayHello",
                                                  (const char *) "(Ljava/lang/String;)V");

    // 3.实例化该类
    jstring jst = env->NewStringUTF("i am your father!");
    env->CallStaticVoidMethod(jclazz, jmethodIDs, jst);

}
/**
 * instance:谁调用了当前的Java_com_ray_myjnitest_MainActivity_callBackShowToast对应的接java口
 * 就是谁的实例,当前就是MainActivity
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_ray_myjnitest_MainActivity_callBackShowToast(JNIEnv *env, jobject instance) {

    // 1.得到字节码
    jclass jclazz = env->FindClass((const char *) "com/ray/myjnitest/MainActivity");

    // 2.得到方法
    //最后一个参数是签名
    jmethodID jmethodIDs = env->GetMethodID(jclazz, "showToast", "()V");

    //3.调用方法
    env->CallVoidMethod(instance, jmethodIDs);
}





//下面为异步调用代码

// 注意：无论是jclass还是jobject都必须做强引用保存！
extern "C"
JNIEXPORT void JNICALL
Java_com_ray_myjnitest_MainActivity_init(JNIEnv *env, jobject instance) {
    //获取Java虚拟机，赋值给静态变量
    env->GetJavaVM(&JVM);
    //获取Java对象并做static强引用
    objectMainActivity = env->NewGlobalRef(instance);
    //获取该对象的Java类
    jclass clazz = env->GetObjectClass(objectMainActivity);
    methodCallMeBaby = env->GetMethodID(clazz, "callMeBaby", "(Ljava/lang/String;)V");
}

//通过Java虚拟机获取到当前线程的JNIEnv：
JNIEnv *getCurrentJNIEnv() {
    if (JVM != NULL) {
        JNIEnv *env_new;
        JVM->AttachCurrentThread(&env_new, NULL);
        return env_new;
    } else {
        return NULL;
    }
}

void *callingJava(void *arg) {
    JNIEnv *jniEnv = getCurrentJNIEnv();
    if (jniEnv != NULL) {
        std::string msg = "I'm your father!";
        jniEnv->CallVoidMethod(objectMainActivity, methodCallMeBaby,
                               jniEnv->NewStringUTF(msg.c_str()));
    }
    //用完后要释放！
    JVM->DetachCurrentThread();
    return NULL;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ray_myjnitest_MainActivity_startCThread(JNIEnv *env, jobject instance) {

    //创建一个C语言的线程,执行上面的callingJava方法
    pthread_t pthread;
    pthread_create(&pthread, NULL, callingJava, NULL);
}