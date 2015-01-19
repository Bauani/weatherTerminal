thread: *cpp *h
	clang++ -std=c++11 -Wall -pthread main.cpp network.cpp systemsInterface.cpp weatherData.cpp -o weather.out
