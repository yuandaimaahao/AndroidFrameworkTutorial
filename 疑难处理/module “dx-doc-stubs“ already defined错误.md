
编译报错：

```bash
FAILED: out/soong/build.ninja
out/soong/.bootstrap/bin/soong_build -t -l out/.module_paths/Android.bp.list -b out/soong -n out -d out/soong/build.ninja.d -globFile out/soong/.bootstrap/build-globs.ninja -o out/soong/build.ninja Android.bp
error: dalvik/dx/src/Android.bp:15:1: module "dx-doc-stubs" already defined
       dalvik/dx/bin/Android.bp:15:1 <-- previous definition here
error: dalvik/dx/src/Android.bp:32:1: module "dx-docs" already defined
       dalvik/dx/bin/Android.bp:32:1 <-- previous definition here
15:41:30 soong bootstrap failed with: exit status 1
```

删除 dalvik/dx/bin 文件夹即可