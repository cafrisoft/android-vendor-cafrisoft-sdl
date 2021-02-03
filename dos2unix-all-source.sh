

file_type=("h" "c" "hpp" "cpp" "mk" "sh" "py")


for (( i = 0 ; i < ${#file_type[@]} ; i++ )) ; do
	echo "------------------------------------------------------------------"
	echo "dos2unix file :  ${file_type[$i]}"
	cmd="find . -name "*.${file_type[$i]}" -exec dos2unix  {} \;"
	echo $cmd
	echo "------------------------------------------------------------------"
	find . -name "*.${file_type[$i]}" -exec dos2unix  {} \;
	echo ""
done

