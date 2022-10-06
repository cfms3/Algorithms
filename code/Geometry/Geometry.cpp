#include <bits/stdc++.h>

using namespace std;

/******************************| Basic Plane Geometry |******************************/

typedef long double ld;

const ld EPS = 1e-12;
const ld PI = acos(-1.0);

struct PT {
	ld x, y;
	
	PT(ld x = 0.0, ld y = 0.0) : x(x), y(y) {}
	PT(const PT &p) : x(p.x), y(p.y) {}
	
	inline PT operator + (const PT &p) const { return PT(x + p.x, y + p.y); }
	inline PT operator - (const PT &p) const { return PT(x - p.x, y - p.y); }
	inline PT operator * (const ld &c) const { return PT(x * c, y * c); }
	inline PT operator / (const ld &c) const { return PT(x / c, y / c); }
	
	inline PT operator  = (const PT &p) { return PT(x = p.x, y = p.y); }
	inline PT operator += (const PT &p) { return PT(x += p.x, y += p.y); }
	inline PT operator -= (const PT &p) { return PT(x -= p.x, y -= p.y); }
	inline PT operator *= (const ld &c) { return PT(x *= c, y *= c); }
	inline PT operator /= (const ld &c) { return PT(x /= c, y /= c); }

	inline bool operator  < (const PT &p) const { return (std::abs(x - p.x) > EPS) ? (x < p.x) : (y < p.y); }
	inline bool operator >= (const PT &p) const { return !(*this < p); }
	inline bool operator == (const PT &p) const { return (std::abs(x - p.x) < EPS) && (std::abs(y - p.y) < EPS); }
	inline bool operator <= (const PT &p) const { return (*this < p) || (*this == p); }
	inline bool operator  > (const PT &p) const { return !(*this <= p); }
};

inline PT abs(const PT &p) { return PT(std::abs(p.x), std::abs(p.y)); }

inline istream& operator >> (istream &is, PT &p) { return is >> p.x >> p.y; }
inline ostream& operator << (ostream &os, const PT p) { return os << '(' << p.x << ", " << p.y << ')'; }

ld dot(PT p, PT q) { return p.x * q.x + p.y * q.y; }			// Dot product, Scalar product;
ld dist2(PT p, PT q) { return dot(p - q, p - q); }				// Square of the distance;
ld dist(PT p, PT q) { return sqrt(dist2(p, q)); }				// Distance beetween two points;
ld cross(PT p, PT q) { return p.x * q.y - p.y * q.x; }			// Cross product, vector product. The abs of the vector product gives the trapezoid area formed by the vectors;
ld norm(PT p) { return sqrt(dot(p, p)); }						// Vector norm, module of a vector;
PT Resize(PT p, ld k) { return (p / norm(p)) * k; }				// Resize vector 'p' to 'k';
PT Rotate90CW(PT p) { return PT(p.y, -p.x); }					// Rotate 'p' 90 CW;
PT Rotate90CCW(PT p) { return PT(-p.y, p.x); }					// Rotate 'p' 90 CCW;
PT RotateCCW(PT p, const ld &t) { return PT(p.x * cos(t) - p.y * sin(t), p.x * sin(t) + p.y * cos(t)); } // Rotate t degrees CCW;

// Project point c onto line through a and b, assuming a != b;
PT ProjectPointLine(PT a, PT b, PT c) { return a + (b - a) * dot(c - a, b - a) / dot(b - a, b - a); }

// Project point c onto line segment through a and b;
PT ProjectPointSegment(PT a, PT b, PT c) {
	ld r = dot(b - a, b - a);
	if(std::abs(r) < EPS) return a;
	r = dot(c - a, b - a) / r;
	if(r < 0) return a;
	if(r > 1) return b;
	return a + (b - a) * r;
}

// Compute distance from c to segment between a and b;
ld DistancePointSegment(PT a, PT b, PT c) {
	return sqrt(dist2(c, ProjectPointSegment(a, b, c)));
}

// Compute distance between point (x, y, z) and plane ax + by + cz = d;
ld DistancePointPlane(ld x, ld y, ld z, ld a, ld b, ld c, ld d) {
	return std::abs(a * x + b * y + c * z - d) / sqrt(a * a + b * b + c * c);
}

// Determine if lines from a to b and c to d are parallel;
bool LinesParallel(PT a, PT b, PT c, PT d) { return std::abs(cross(b - a, c - d)) < EPS; }

// Determine if lines from a to b and c to d are collinear;
bool LinesCollinear(PT a, PT b, PT c, PT d) {
	if(!LinesParallel(a, b, c, d)) return false;
	if(std::abs(cross(a - b, a - c)) >= EPS || std::abs(cross(c-d, c-a)) >= EPS) return false;
	return true;
}

// Determine if line segment from a to b intersects with line segment from c to d;
bool SegmentsIntersect(PT a, PT b, PT c, PT d) {
	if(LinesCollinear(a, b, c, d)) {
		if(dist2(a, c) < EPS || dist2(a, d) < EPS || dist2(b, c) < EPS || dist2(b, d) < EPS) return true;
		if(dot(c-a, c-b) > 0 && dot(d-a, d-b) > 0 && dot(c-b, d-b) > 0) return false;
		return true;
	}
	if(cross(d-a, b-a) * cross(c-a, b-a) > 0) return false;
	if(cross(a-c, d-c) * cross(b-c, d-c) > 0) return false;
	return true;
}

/*
 * Compute intersection of line passing through a and b
 * with line passing through c and d, assuming that unique
 * intersection exists; for segment intersection, check if
 * segments intersect first;
 */
PT ComputeLineIntersection(PT a, PT b, PT c, PT d) {
	b = b - a; d = c - d; c = c - a;
	assert(dot(b, b) > EPS && dot(d, d) > EPS);
	return a + b * cross(c, d) / cross(b, d);
}

// Compute center of circle given three points;
PT ComputeCircleCenter(PT a, PT b, PT c) {
	b = (a + b) / 2.0;
	c = (a + c) / 2.0;
	return ComputeLineIntersection(b, b + Rotate90CW(a - b), c, c + Rotate90CW(a - c));
}

// Determine if point is in a possibly non-convex polygon. Works well for strictly internal or external points;
bool PointInPolygon(const vector<PT> &p, PT q) {
	bool c = 0;
	for(int i = 0; i < p.size(); i++) {
		int j = (i + 1) % p.size();
		if((p[i].y <= q.y && q.y < p[j].y || p[j].y <= q.y && q.y < p[i].y) && q.x < p[i].x + (p[j].x - p[i].x) * (q.y - p[i].y) / (p[j].y - p[i].y)) {
			c = !c;
		}
	}
	return c;
}

// Compute intersection of line through points a and b with circle centered at c with radius r > 0;
vector<PT> CircleLineIntersection(PT a, PT b, PT c, ld r) {
	vector<PT> ret;
	b = b - a;
	a = a - c;
	ld A = dot(b, b);
	ld B = dot(a, b);
	ld C = dot(a, a) - r * r;
	ld D = B * B - A * C;
	if(D < -EPS) return ret;
	ret.push_back(c + a + b * (-B + sqrt(D + EPS)) / A);
	if(D > EPS) ret.push_back(c + a + b * (-B - sqrt(D)) / A);
	return ret;
}

// Compute intersection of circle centered at a with radius r with circle centered at b with radius R;
vector<PT> CircleCircleIntersection(PT a, PT b, ld r, ld R) {
	vector<PT> ret;
	ld d = sqrt(dist2(a, b));
	if(d > r + R || d + min(r, R) < max(r, R)) return ret;
	ld x = (d * d - R * R + r * r) / (2.0 * d);
	ld y = sqrt(r * r - x * x);
	PT v = (b - a) / d;
	ret.push_back(a + v * x + Rotate90CCW(v) * y);
	if(y > 0) ret.push_back(a + v * x - Rotate90CCW(v) * y);
	return ret;
}

// Computes the area of a (possibly nonconvex) polygon, assuming that the coordinates are listed in a CW or CCW;
ld ComputeSignedArea(const vector<PT> &p) {
	ld area = 0;
	for(int i = 0; i < p.size(); i++) {
		int j = (i+1) % p.size();
		area += p[i].x * p[j].y - p[j].x * p[i].y;
	}
	return area / 2.0;
}

ld ComputeArea(const vector<PT> &p) { return std::abs(ComputeSignedArea(p)); }

// Computes the centroid of a polygon. Centroid is often known as the "center of gravity" or "center of mass";
PT ComputeCentroid(const vector<PT> &p) {
	PT c(0,0);
	ld scale = 6.0 * ComputeSignedArea(p);
	for(int i = 0; i < p.size(); i++){
		int j = (i+1) % p.size();
		c = c + (p[i] + p[j]) * (p[i].x * p[j].y - p[j].x * p[i].y);
	}
	return c / scale;
}

// Tests whether or not a given polygon (in CW or CCW order) is simple;
bool IsSimple(const vector<PT> &p) {
	for (int i = 0; i < p.size(); i++) {
		for (int k = i+1; k < p.size(); k++) {
			int j = (i+1) % p.size();
			int l = (k+1) % p.size();
			if (i == l || j == k) continue;
			if (SegmentsIntersect(p[i], p[j], p[k], p[l])) return false;
		}
	}
	return true;
}

/*
- Law of Sines, R is the radius of its circumcircle:
	(A / sin(a)) == (B / sin(b)) == (C / sin(c)) == 2*R;

- Law of Cosines:
	A**2 == B**2 + C**2 - 2 * B * C * cos(bc);
*/
inline bool IsTriangle(const ld a, const ld b, const ld c) { return (a + b > c) && (a + c > b) && (b + c > a); }
inline ld Heron(const ld a, const ld b, const ld c) { ld sp = (a + b + c) / 2.0; return sqrt(sp * (sp - a) * (sp - b) * (sp - c)); }

/************************************************************************************/

int main() {
	ios::sync_with_stdio(0), cin.tie(0);
	
	return 0;
}
