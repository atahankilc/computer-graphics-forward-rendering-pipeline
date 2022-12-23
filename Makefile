all:
	g++ *.cpp Transformation/*.cpp Vector/*.cpp -o raytracer -std=c++11 -O3
