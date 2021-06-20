// Idea: Static To Dynamic, which allows insert operations for static DS like AhoCorasick or CHT
// use CHT as the example
// rebuid after insert all necessary set before answer queries
// note the pointers

const int LG = 20;

struct Point {
	long long x, y;
	bool operator < (Point &a) {
		if (x == a.x) return y < a.y;
		return x < a.x;
	}
};

struct CHT {
	vector<Point> pts;

	double cross (Point a, Point b) {return (double)(b.y - a.y) / (b.x - a.x);}

	void addLine(long long x, long long y) {
		if (pts.size() && pts.back().x == x && pts.back().y == y) return;

		while(pts.size() >= 2 && cross(pts[pts.size()-2], pts.back()) > cross(pts.back(), (Point){x, y})){
			pts.pop_back();	
		}
		pts.push_back({x, y});
	}

	long long query(long long x) {
		int s = 0, e = pts.size()-1;
		auto f = [&](int p) {
			return pts[p].x * x + pts[p].y;
		};

		while(s != e){
			int m = (s + e) / 2;
			if(f(m) <= f(m + 1)) e = m;
			else s = m + 1;
		}
		return pts[s].x * x + pts[s].y;
	}
};

CHT merge(CHT &a, CHT &b) {
	CHT res;
	vector<Point> tmp;
	merge(a.pts.begin(), a.pts.end(), b.pts.begin(), b.pts.end(), back_inserter(tmp));

	for (auto i: tmp) {
		res.addLine(i.x, i.y);
	}
	return res;
}

template <typename T>
struct Container {
	vector<T*> group[LG + 1];

	void insert(Container &c) {
		for (int i = 0; i <= LG; i++) {
			for (auto j: c.group[i]) {
				group[i].push_back(j);
			}
		}
	}

	void rebuidouble() {
		for (int i = 0; i <= LG; i++) {
			for (int j = 0; j + 1 < group[i].size(); j += 2) {
				T *tmp = new T();
				*tmp = merge(*group[i][j], *group[i][j + 1]);
				group[i + 1].push_back(tmp);
			}
			if (group[i].size() % 2) {
				auto tmp = group[i].back();
				group[i].clear();
				group[i].push_back(tmp);
			}
			else {
				group[i].clear();
			}
		}
	}

	void print() {
		for (int i = 0; i <= LG; i++) {
			if (group[i].size()) {
				cout << i << endl;
				for (auto j: group[i][0]->v) {
					cout << j.x << " " << j.y << endl;
				}
			}
		}
		cout << endl;
	}
};
