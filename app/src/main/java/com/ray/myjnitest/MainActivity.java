package com.ray.myjnitest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

/**
 * by Lu1
 */
public class MainActivity extends AppCompatActivity {

    private JNI jni;
    private Button mBtn;
    private TextView mTv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        jni = new JNI();
        mBtn = findViewById(R.id.sample_text);
        mBtn.setText(helloJNI());


        mTv = findViewById(R.id.textView);
        init();
        startCThread();
    }

    public native String helloJNI();

    public native void callBackShowToast();

    public void click(View view) {

        jni.callbackAdd();
        jni.callbackHelloFromJava();
        jni.callbackPrintString();
        jni.callbackSayHello();
        this.callBackShowToast();
    }

    public void showToast() {
        System.out.println("--showToast--");
        Toast.makeText(this, "--showToast--", Toast.LENGTH_SHORT).show();
    }

    private void callMeBaby(final String msg) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mTv.setText(msg);
            }
        });
    }


    //初始化JNI库
    public native void init();

    private native void startCThread();
}
