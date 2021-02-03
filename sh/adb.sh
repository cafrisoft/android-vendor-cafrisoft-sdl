
adb="adb"

binpath="system/bin"
libpath="system/lib64"
#host_OUTT="out/target/product/odroidn2"
host_OUTT="OUTT"

echo "------------------------------------------------------------------"
echo "copy SDLTest App"
echo "------------------------------------------------------------------"
SDLTestApps=("sdltestyuv" )
echo "SDLTestApps = ${SDLTestApps[@]}"
echo SDLTestApps count : ${#SDLTestApps[@]}
for (( i = 0 ; i < ${#SDLTestApps[@]} ; i++ )) ; do
        cmd="$adb $1 $2 push $host_OUTT/$binpath/${SDLTestApps[$i]} /$binpath"
        echo $cmd
        $cmd
done
echo ""

echo "------------------------------------------------------------------"
echo "copy SDL Shared Library"
echo "------------------------------------------------------------------"
SDL_SHARED_LIBS=("libSDL2.so" "libhidapi.so" "libSDL2_ttf.so")
echo "SDL_SHARED_LIBS = ${SDL_SHARED_LIBS[@]}"
echo SDL_SHARED_LIBS count : ${#SDL_SHARED_LIBS[@]}
for (( i = 0 ; i < ${#SDL_SHARED_LIBS[@]} ; i++ )) ; do
        cmd="$adb $1 $2 push $host_OUTT/$libpath/${SDL_SHARED_LIBS[$i]} /$libpath"
        echo $cmd
        $cmd
done
echo ""

