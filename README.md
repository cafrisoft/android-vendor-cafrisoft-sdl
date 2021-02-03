# android-vendor-cafrisoft-sdl

-------------------------------------------------------------------------
How to build on AOSP, sdk_phone_x86_64
-------------------------------------------------------------------------

$cd [Your Android]
$vi build/make/target/product/sdk_phone_x86_64.mk
   
   ** inser below statement.

    $(call inherit-product-if-exists, vendor/cafrisoft/sdl/device-vendor.mk)

$cd [Your Android]
$source build/envsetup
$lunch sdk_phone_x86_64
$make 

-------------------------------------------------------------------------
How to Run AVD with writable mode
-------------------------------------------------------------------------

$emulator.exe -avd Pixel3XL-Android9-API_28-X64-CafriSoft -writable-system
$adb root
$adb remount


