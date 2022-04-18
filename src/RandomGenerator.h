#include <chrono>
class Random {
public:
	static int getRand(int numLessThan) {
		auto time = static_cast<int>(chrono::high_resolution_clock::now()
			.time_since_epoch().count());
		srand(time);
		return rand() % numLessThan;
	}
};