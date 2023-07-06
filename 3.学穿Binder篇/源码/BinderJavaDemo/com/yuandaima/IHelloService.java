/*
 * This file is auto-generated.  DO NOT MODIFY.
 */
package com.yuandaima;
public interface IHelloService extends android.os.IInterface
{
  /** Default implementation for IHelloService. */
  public static class Default implements com.yuandaima.IHelloService
  {
    @Override public void sayhello() throws android.os.RemoteException
    {
    }
    @Override public int sayhello_to(java.lang.String name) throws android.os.RemoteException
    {
      return 0;
    }
    @Override
    public android.os.IBinder asBinder() {
      return null;
    }
  }
  /** Local-side IPC implementation stub class. */
  public static abstract class Stub extends android.os.Binder implements com.yuandaima.IHelloService
  {
    private static final java.lang.String DESCRIPTOR = "com.yuandaima.IHelloService";
    /** Construct the stub at attach it to the interface. */
    public Stub()
    {
      this.attachInterface(this, DESCRIPTOR);
    }
    /**
     * Cast an IBinder object into an com.yuandaima.IHelloService interface,
     * generating a proxy if needed.
     */
    public static com.yuandaima.IHelloService asInterface(android.os.IBinder obj)
    {
      if ((obj==null)) {
        return null;
      }
      android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
      if (((iin!=null)&&(iin instanceof com.yuandaima.IHelloService))) {
        return ((com.yuandaima.IHelloService)iin);
      }
      return new com.yuandaima.IHelloService.Stub.Proxy(obj);
    }
    @Override public android.os.IBinder asBinder()
    {
      return this;
    }
    @Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
    {
      java.lang.String descriptor = DESCRIPTOR;
      switch (code)
      {
        case INTERFACE_TRANSACTION:
        {
          reply.writeString(descriptor);
          return true;
        }
        case TRANSACTION_sayhello:
        {
          data.enforceInterface(descriptor);
          this.sayhello();
          reply.writeNoException();
          return true;
        }
        case TRANSACTION_sayhello_to:
        {
          data.enforceInterface(descriptor);
          java.lang.String _arg0;
          _arg0 = data.readString();
          int _result = this.sayhello_to(_arg0);
          reply.writeNoException();
          reply.writeInt(_result);
          return true;
        }
        default:
        {
          return super.onTransact(code, data, reply, flags);
        }
      }
    }
    private static class Proxy implements com.yuandaima.IHelloService
    {
      private android.os.IBinder mRemote;
      Proxy(android.os.IBinder remote)
      {
        mRemote = remote;
      }
      @Override public android.os.IBinder asBinder()
      {
        return mRemote;
      }
      public java.lang.String getInterfaceDescriptor()
      {
        return DESCRIPTOR;
      }
      @Override public void sayhello() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_sayhello, _data, _reply, 0);
          if (!_status && getDefaultImpl() != null) {
            getDefaultImpl().sayhello();
            return;
          }
          _reply.readException();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
      }
      @Override public int sayhello_to(java.lang.String name) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeString(name);
          boolean _status = mRemote.transact(Stub.TRANSACTION_sayhello_to, _data, _reply, 0);
          if (!_status && getDefaultImpl() != null) {
            return getDefaultImpl().sayhello_to(name);
          }
          _reply.readException();
          _result = _reply.readInt();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      public static com.yuandaima.IHelloService sDefaultImpl;
    }
    static final int TRANSACTION_sayhello = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
    static final int TRANSACTION_sayhello_to = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
    public static boolean setDefaultImpl(com.yuandaima.IHelloService impl) {
      if (Stub.Proxy.sDefaultImpl == null && impl != null) {
        Stub.Proxy.sDefaultImpl = impl;
        return true;
      }
      return false;
    }
    public static com.yuandaima.IHelloService getDefaultImpl() {
      return Stub.Proxy.sDefaultImpl;
    }
  }
  public void sayhello() throws android.os.RemoteException;
  public int sayhello_to(java.lang.String name) throws android.os.RemoteException;
}
