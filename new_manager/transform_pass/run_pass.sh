if [[ $1 ]]; then
	opt -load-pass-plugin=./libMyPass.dylib -passes "my-pass" $1
else
    echo "usage: ./run_pass.sh <module.ll>"
fi
