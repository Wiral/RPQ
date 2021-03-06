#ifndef RPQproc_H_
#define RPQproc_H_

#include <string>
#include <fstream>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <queue>
#include "RPQtask.h"

// Class solving RPQ problem for included tasks
class RPQproc {
	RPQtask* _tasks = nullptr;
	int _numTasks = 0;
	int _numCol = 0;

public:
	RPQproc(std::fstream& file);	//init structure with tasks using the file
	~RPQproc() {
		delete[] _tasks;
	}
	int procTime();	//calculates the time needed to finish and close all the tasks (cMax)
	int numTasks() {
		return _numTasks;
	}	//number of tasks in the _task array
	int numCol() {
		return _numCol;
	}
	void refresh();	//relaods _start flied of RPQtasks in array with their _r values
	void quickSort();		//sorts _tasks array using quicksort (by _r values)
	int twoOpt();		//sorts _tasks array using 2opt method and returns cMax
	void schrageSort();					//sort _tasks using shrege algoritm
	void schrageSort_prmt();				//sort _task using schrage algoritm (prmt variation)
	void printTasks();
};

//initialize structure with tasks specified in a file
RPQproc::RPQproc(std::fstream& file) {
	int r, p, q, i;
	file >> _numTasks;
	file >> _numCol;
	_tasks = new RPQtask[_numTasks];

	for (i = 0; i < _numTasks; i++) {
		file >> r;
		file >> p;
		file >> q;
		_tasks[i] = RPQtask(r, p, q);
	}
}

int RPQproc::procTime() {
	int cMax = 0;	//time needed to finish and close all the tasks
	int i;

	for (i = 1; i < _numTasks; i++) //task[0] is already calculated, proceed with the rest
		_tasks[i].setStart(std::max(_tasks[i].r(), _tasks[i - 1].start() + _tasks[i - 1].p())); //update starting time

	for (i = 0; i < _numTasks; i++)	//get the slowest task's time to obtain overall time
		cMax = std::max(cMax, _tasks[i].finishTime());

	refresh();	//refresh tasks' _start fields with their _r values
	return cMax;
}

void RPQproc::quickSort() {
	std::qsort(_tasks, _numTasks, sizeof(RPQtask), compareR);

}

void RPQproc::refresh() {
	for (int i = 0; i < _numTasks; i++)
		_tasks[i].setStart(_tasks[i].r());
}

int RPQproc::twoOpt() {
	int cMax, tmp;
	int i, j;
	cMax = procTime();
	i = 0;
	j = 1;
	while (i < _numTasks - 1) {
		std::swap(_tasks[i], _tasks[j]);
		tmp = procTime();

		if (tmp >= cMax) { //time got worse, revert change and proceed
			std::swap(_tasks[i], _tasks[j]);
			j++;
		} else {
			cMax = tmp;
			i = 0;
			j = 1;
		}

		if (j >= _numTasks) {	//iterator got to the end of array, proceed
			i++;
			j = i + 1;
		}
	}

	return cMax;
}

void RPQproc::schrageSort() {
	int i = 0;
	int j = 0;
	int time_stamp = 0;
	RPQtask *sorted_tasks = new RPQtask[_numTasks];
	std::priority_queue<RPQtask, std::vector<RPQtask>, RPQtask::LessThanQ> q; // queue sorting elements from biggest _q to lowest

	quickSort(); // sort _tasks by _r value

	while (i < _numTasks || !q.empty()) {
		while (i < _numTasks) { //add to queue all tasks that are already prepared
			if (_tasks[i].r() <= time_stamp) {
				q.push(_tasks[i]);
				i++;
			} else {
				break;
			}
		}

		if (!q.empty()) {
			sorted_tasks[j] = q.top(); 	//copy the task with the biggest _q
			q.pop(); 	//delete from queue
			if (j > 0)
				sorted_tasks[j].setStart(std::max(sorted_tasks[j].r(), sorted_tasks[j - 1].start() + sorted_tasks[j - 1].p())); //update start time in new array

			time_stamp = sorted_tasks[j].start() + sorted_tasks[j].p(); //update time stamp
			j++;

		} else { //if no tasks were prepared after the last one finished processsing
			time_stamp = _tasks[i].r();
		}

	} //end while

	delete[] _tasks;
	_tasks = sorted_tasks;	//make new array the original one
}

void RPQproc::printTasks() {
	for (int i = 0; i < _numTasks; i++) {
		std::cout << "task[" << i << "] : " << std::endl;
		std::cout << " r: " << _tasks[i].r() << ", p: " << _tasks[i].p() << ", q: " << _tasks[i].q() << std::endl;
	}

}

void RPQproc::schrageSort_prmt() {
	int i = 0;
	int j = 0;
	int time_stamp = 0;
	std::vector<RPQtask> sorted_tasks;
	std::priority_queue<RPQtask, std::vector<RPQtask>, RPQtask::LessThanQ> q; // queue sorting elements from biggest _q to lowest

	quickSort(); // sort _tasks by _r value

	while (i < _numTasks || !q.empty()) {
		while (i < _numTasks) { //add to queue all tasks that are already prepared
			if (_tasks[i].r() <= time_stamp) {
				q.push(_tasks[i]);
				i++;
			} else {
				break;
			}
		}

		if (!q.empty()) {
			sorted_tasks.push_back(q.top()); 	//copy the task with the biggest _q
			q.pop(); 	//delete from queue
			if (j > 0) {
				if (sorted_tasks[j].q() > sorted_tasks[j - 1].q() && sorted_tasks[j].r() < time_stamp) { //if the task with the biggest _q could start earlier
					RPQtask t(0, sorted_tasks[j - 1].start() + sorted_tasks[j - 1].p() - sorted_tasks[j].r(), sorted_tasks[j - 1].q());	//interrupt earlier task and let the bigger _q
					sorted_tasks[j - 1].setP(sorted_tasks[j].r() - sorted_tasks[j - 1].start());										//task start instantly
					sorted_tasks[j - 1].setQ(0);
					q.push(t);						//add remaining part of the interrupted task back to the queue
				}
				sorted_tasks[j].setStart(std::max(sorted_tasks[j].r(), sorted_tasks[j - 1].start() + sorted_tasks[j - 1].p())); //update start time in new array

			}
			time_stamp = sorted_tasks[j].start() + sorted_tasks[j].p(); //update time stamp
			j++;

		} else { //if no tasks were prepared after the last one finished processsing
			time_stamp = _tasks[i].r();
		}

	} //end while

	//delete old tasks and copy new one
	delete[] _tasks;
	_tasks = new RPQtask[sorted_tasks.size()];
	_numTasks = sorted_tasks.size();
	for(i = 0; i < _numTasks;  i++)
		_tasks[i] = sorted_tasks[i];
}


#endif /* RPQproc_H_ */



