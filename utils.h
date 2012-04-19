class Point {
public:
	float x, y, z;
	
	void set(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	
	Point() {}

	Point(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	Point(Point& other) {
		x = other.x; y = other.y; z = other.z;
	}

	Point operator- (Point other) {
		Point result(this->x - other.x, this->y - other.y, this->z - other.z);
		return result;
	}
	
	Point operator+ (Point other) {
		Point result(other.x + this->x, other.y + this->y, other.z + this->z);
		return result;
	}

	float operator* (Point other) {
		float result = this->x * other.x + this->y * other.y + this->z * other.z;
		return result;
	}

	Point operator* (float scalar) {
		Point result(this->x * scalar, this->y * scalar, this->z * scalar);
		return result;
	}

	float len() {
		float result = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		return result;
	}

	void normalize() {
		float len = this->len();
		this->x /= len;
		this->y /= len;
		this->z /= len;
	}
};