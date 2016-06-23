cd Hierarchical-locks-Tree/src
echo Compiting binary-tree ...
g++ main.cpp -std=gnu++0x -lpthread -o ../a.out

cd ../..
g++ Plotter.cpp -o Plotter

cd ./Hierarchical-locks-Tree
echo Step:1 of 4: Executing DomLock over tree......
	./domscript_small.sh

cd ..
./Plotter
