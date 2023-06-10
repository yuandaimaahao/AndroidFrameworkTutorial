package com.example.ndk;

public class NativeTest {
    static {
      System.loadLibrary("nativetest"); 
    }

    public native void init();

    public native void init(int age);

    public native boolean init(String name);

    public native void update();

    public static void main(String[] args) {
      NativeTest test = new NativeTest();
      test.init();
      test.init(2);
      test.init("hello");
      test.update();
   }
}