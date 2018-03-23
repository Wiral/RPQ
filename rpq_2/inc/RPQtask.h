#ifndef RPQtask_H_
#define RPQtask_H_

//Task class
class RPQtask {
	 int _r; 	// preparing time
	 int _p ;	// processing time
	 int _q;	// quiting time
	 int _start;	// actual starting time in RPQ problem
public:
	RPQtask( int r, int p, int q): _r(r), _start(r), _p(p), _q(q) {} //init with default times for the current process, init starting time with the preparing time
	RPQtask(): _r(0), _start(0), _p(0), _q(0){}
	void setR( int r) { _r = r;}
	void setP( int p) { _p = p;}
	void setQ( int q) { _q = q;}
	void setStart( int start){_start = start;}
	const int r() const {return _r;}
	const int p() const {return _p;}
	const int q() const {return _q;}
	const int start(){return _start;}
	int finishTime() {return _start + _p+ _q;} //actual finish time considering RPQ problem
	struct LessThanQ{
	bool operator() (const RPQtask& t1, const RPQtask& t2) const{
		return t1.q() <= t2.q();
	}
	};
};


int compareR(const void* a, const void* b) {
	RPQtask* p1 = (RPQtask*) a;
	RPQtask* p2 = (RPQtask*) b;
	if (p1->r() > p2->r())
		return 1;
	if (p1->r() < p2->r())
		return -1;
	if (p1->r() == p2->r())
		return 0;

}

#endif /* RPQtask_H_ */
