#include <iostream>
#include <vector>

/*
 * This class represents a slab with dimensions _horizontal x _vertical
 * and with a certain _value.
 */
class Slab {
	uint32_t _horizontal;
	uint32_t _vertical;
	uint32_t _value;

public:
	Slab(uint32_t horizontal, uint32_t vertical, uint32_t value)
		: _horizontal(horizontal), _vertical(vertical), _value(value) {}

	// Returns the horizontal size of the slab
	inline uint32_t horizontal() const { return _horizontal; }

	// Returns the vertical size of the slab
	inline uint32_t vertical() const { return _vertical; }

	// Returns the value of the slab
	inline uint32_t value() const { return _value; }

	// Sets the value of the slab
	inline void setValue(uint32_t value) { _value = value; }

	// Returns true if the slab is a square, false otherwise
	inline bool square() { return _horizontal == _vertical; }

	// Returns true if this slab can be fully contained
	// in a square of horizontal x vertical dimensions
	inline bool inside(uint32_t horizontal, uint32_t vertical) {
		return _horizontal <= horizontal && _vertical <= vertical;
	}

	// Returns a new slab rotated by 90 degrees
	Slab rotated() {
		return Slab(_vertical, _horizontal, _value);
	}

	// Reads a slab with no value in the form of:
	// <horizontal size> <vertical size>
	static Slab readFreeSlab(std::istream& in) {
		uint32_t length, width;
		in >> length >> width;
		return Slab(length, width, 0);
	}

	// Reads a slab with value in the form of:
	// <horizontal size> <vertical size> <value>
	static Slab readSlab(std::istream& in) {
		uint32_t length, width, value;
		in >> length >> width >> value;
		return Slab(length, width, value);
	}
};

/*
 * Sets the value of uncut to the maximum value possible, if it were
 * to be cut according to the items vector.
 * Assumes uncut's horizontal size is bigger than it's vertical size.
 */
void calculateMaximumValue(Slab& uncut, const std::vector<Slab>& items) {
	std::vector<std::vector<uint32_t>> maxes(uncut.horizontal() + 1,
					std::vector<uint32_t>(uncut.vertical() + 1, 0));
	// Put the values of each item in the matrix
	for (auto it = items.begin(); it != items.end(); ++it)
		maxes[it->horizontal()][it->vertical()] = std::max(it->value(),
							 maxes[it->horizontal()][it->vertical()]);
	// Calculate all cases up to the size of uncut
	for (uint32_t i = 1; i <= uncut.horizontal(); i++) {
		for (uint32_t j = i; j <= uncut.vertical(); j++) {
			uint32_t upperY = j / 2;
			uint32_t upperX = i / 2;
			uint32_t xCut = 0;
			uint32_t yCut = 0;
			// Find the best cut along the horizontal and vertical axis
			for (uint32_t x = 0; x <= upperX; x++)
				xCut = std::max(maxes[x][j] + maxes[i - x][j], xCut);
			for (uint32_t y = 0; y <= upperY; y++)
				yCut = std::max(maxes[i][y] + maxes[i][j - y], yCut);
			maxes[i][j] = std::max(xCut, yCut);
			if (j <= uncut.horizontal())
				maxes[j][i] = maxes[i][j];
		}
	}
	// the maximum value is in the spot filled last
	uncut.setValue(maxes[uncut.horizontal()][uncut.vertical()]);
}

int main() {
	std::ios::sync_with_stdio(false);
	Slab uncut = Slab::readFreeSlab(std::cin); // Read the slab to be cut
	int itemQuantity = -1;
	std::cin >> itemQuantity;
	std::vector<Slab> items;
	if (uncut.horizontal() > uncut.vertical())
		uncut = uncut.rotated(); // Ensures the condition of calculateMaximumValue
	for (int i = 0; i < itemQuantity; i++) { // read all items
		Slab item = Slab::readSlab(std::cin);
		if (item.inside(uncut.horizontal(), uncut.vertical()))
			items.push_back(item); // discard any bigger than the uncut slab
		Slab rotated = item.rotated(); // the rotated item is valid too
		if (!rotated.square() && rotated.inside(uncut.horizontal(), uncut.vertical()))
			items.push_back(rotated);
	}
	calculateMaximumValue(uncut, items);
	std::cout << uncut.value() << '\n';
	return 0;
}
