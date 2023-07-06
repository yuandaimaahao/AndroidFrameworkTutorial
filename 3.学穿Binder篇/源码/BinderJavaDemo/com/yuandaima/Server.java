package com.yuandaima;

import android.util.Log;
import android.os.ServiceManager;

public class Server {

    private static final String TAG = "BinderServer";

    public static void main(String args[]) {
        /* add Service */
        Log.i(TAG, "add hello service");
        ServiceManager.addService("hello", new HelloService());

        //app_process 启动时，会启动 binder 线程用于获取和解析 binder 消息，应用程序无需关心
        while (true) {
            try {
            	Thread.sleep(100);
          	} catch (Exception e){}
        }   
    }
}