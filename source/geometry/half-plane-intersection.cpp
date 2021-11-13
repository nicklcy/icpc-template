using ld = long double;
const ld eps = 1E-14;

int sign(ld x) {
  return x < -eps ? -1 : x > eps ? 1 : 0;
}

struct Point {
  ld x, y;
  Point(ld x = 0, ld y = 0): x(x), y(y) {}
  Point operator + (const Point &p) const {
    return Point(x + p.x, y + p.y);
  }
  Point operator - (const Point &p) const {
    return Point(x - p.x, y - p.y);
  }
  Point operator * (const ld &k) const {
    return Point(x * k, y * k);
  }
  Point operator / (const ld &k) const {
    return Point(x / k, y / k);
  }
  int quad() const {
    return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0);
  }
  Point turn90() const {
    return Point(-y, x);
  }
  ld mod() const {
    return sqrt(x * x + y * y);
  }
  Point norm() const {
    ld m = mod();
    if (sign(m) == 0) return Point(0, 0);
    return Point(x / m, y / m);
  }
  void input() {
    x = read();
    y = read();
  }
};

ostream& operator << (ostream &os, const Point &p) {
  return os << "(" << p.x << ", " << p.y << ")";
}

bool operator == (const Point &a, const Point &b) {
  return sign(a.x - b.x) == 0 && sign(a.y - b.y) == 0;
}

bool operator < (const Point &a, const Point &b) {
  return sign(a.x - b.x) ? a.x < b.x : sign(a.y - b.y) ? a.y < b.y : false;
}

bool operator != (const Point &a, const Point &b) {
  return !(a == b);
}

ld dot(const Point &a, const Point &b) {
  return a.x * b.x + a.y * b.y;
}

ld det(const Point &a, const Point &b) {
  return a.x * b.y - a.y * b.x;
}

struct Line {
  Point a, b;
  Line(Point a = Point(), Point b = Point()) : a(a), b(b) {}
  bool include(const Point &p) const {
    return sign(det(b - a, p - a)) > 0;
  }
  Line push() const {
    Point delta = (b - a).turn90().norm() * eps;
    return Line(a - delta, b - delta);
  }
};

bool on_seg(const Line &l, const Point &p) {
  return sign(det(p - l.a, l.b - l.a)) == 0 &&
    sign(dot(p - l.a, p - l.b)) <= 0;
}

bool parallel(const Line &l1, const Line &l2) {
  return sign(det(l1.b - l1.a, l2.b - l2.a)) == 0;
}

Point intersect(const Line &l1, const Line &l2) {
  double s1 = det(l2.b - l2.a, l1.a - l2.a);
  double s2 = -det(l2.b - l2.a, l1.b - l2.a);
  return (l1.a * s2 + l1.b * s1) / (s1 + s2);
}

bool same_dir(const Line &l0, const Line &l1) {
  return parallel(l0, l1) && sign(dot(l0.b - l0.a, l1.b - l1.a)) == 1;
}

bool sp_comp_point(const Point &a, const Point &b) {
  if (a.quad() != b.quad()) {
    return a.quad() < b.quad();
  } else {
    return sign(det(a, b)) > 0;
  }
}

bool operator < (const Line &l0, const Line &l1) {
  if (same_dir(l0, l1)) {
    return l1.include(l0.a);
  } else {
    return sp_comp_point(l0.b - l0.a, l1.b - l1.a);
  }
}

bool check(const Line &u, const Line &v, const Line &w) {
  return w.include(intersect(u, v));
}

vector<Point> intersection(vector<Line> l) {
  sort(begin(l), end(l));
  deque<Line> q;
  for (int i = 0; i < (int) l.size(); ++i) {
    if (i && same_dir(l[i], l[i - 1])) continue;
    while (q.size() > 1 && !check(q[q.size() - 2], q.back(), l[i])) q.pop_back();
    while (q.size() > 1 && !check(q[1], q[0], l[i])) q.pop_front();
    q.emplace_back(l[i]);
  }
  while (q.size() > 2 && !check(q[q.size() - 2], q.back(), q[0])) q.pop_back();
  while (q.size() > 2 && !check(q[1], q[0], q.back())) q.pop_front();
  vector<Point> ret;
  for (int i = 0; i < (int) q.size(); ++i) {
    ret.emplace_back(intersect(q[i], q[(i + 1) % q.size()]));
  }
  return ret;
}

ld calc_area(const vector<Point> &vc) {
  ld ret = 0;
  for (int i = 0; i < (int) vc.size(); ++i) {
    ret += det(vc[i], vc[(i + 1) % vc.size()]);
  }
  return ret * .5;
}
