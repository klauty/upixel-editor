
#!/bin/zsh

if [[ $1 == "mac" ]] then
	echo "Mac"
	clang main.c -I/usr/local/include/ -o Upixel-Editor -L/usr/local/lib/ -lSDL3 -rpath /usr/local/lib/ 
fi

if [[ $1 == "linux" ]] then
	echo "Linux"
	clang main.c  -o Upixel-Editor  -I/usr/include/ -L/usr/lib/ -lSDL3 
fi

