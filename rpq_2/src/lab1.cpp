#include <iostream>
#include <fstream>
#include "../inc/RPQproc.h"
#include "../inc/Timer.h"

int main() {
	std::string path_names, path;
	std::fstream file_names, file, output;
	std::cout << "Wskaz sciezke do pliku z nazwami: \n";
	std::cin >> path_names;
	TIMER_INIT
	int val;

	output.open("output.txt");
	file_names.open(path_names.c_str());

	if (file_names.is_open()) {
		while (!file_names.eof()) {	//read file names to open
			file_names >> path;
			file.open(path.c_str());
			if (file.is_open()) {//open file and read data. Save it to the txt file.
				std::cout << "==== " << path << " ====" << std::endl;
				RPQproc process(file);
				TIMER_START
				val = process.twoOpt();
				TIMER_STOP

				output << process.numTasks() << " & " << val << " & " << elapsedTime << " & ";

				TIMER_START
				process.quickSort();
				val = process.procTime();
				TIMER_STOP

				output << val << " & " << elapsedTime << " \\\\" << std::endl;
				file.close();
			}
		}

	}

	return 0;
}
