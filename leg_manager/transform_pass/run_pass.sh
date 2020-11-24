if [[ $1 ]]; then
	opt -load ./libMyPass.dylib -my-pass $1
else
    echo "usage: ./run_pass.sh <module.ll>"
fi
