cd Hierarchical-locks-Tree/src
g++ main.cpp -std=c++11 -lpthread -o ../a.out

cd ../..
g++ Plotter.cpp -o Plotter

cd Hierarchical-locks-Tree
echo Step:1 of 4: Executing DomLock over tree......
	./domscript.sh

echo Step:4 of 4: Executing Intention Lock over graph......
cd ..
./Plotter
