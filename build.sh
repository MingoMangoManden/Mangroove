if [ -d /builds/ ]; then
    mkdir builds
fi

if [[ "$OSTYPE" == "darwin"* ]]; then
	g++ -o builds/Mangroove src/main.cpp
else
	g++ -o builds/Mangroove.exe src/main.cpp

fi
