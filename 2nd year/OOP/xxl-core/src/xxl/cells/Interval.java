package xxl.cells;

import java.io.Serializable;
import java.util.Iterator;
import xxl.cells.renderers.Renderer;
import xxl.cells.renderers.Renderable;

/**
 * This class represents an interval of positions, given
 * by a starting and ending position.
 */
public class Interval implements Serializable, Iterable<Position>, Renderable {
	/**
	 * The order of the parameters does not matter.
	 * 
	 * @param pos1 one position of the interval
	 * @param pos2 the other position of the interval
	 */
	public Interval(Position pos1, Position pos2) {
		int minRow = Math.min(pos1.getRow(), pos2.getRow());
		int minColumn = Math.min(pos1.getColumn(), pos2.getColumn());
		int maxRow = Math.max(pos1.getRow(), pos2.getRow());
		int maxColumn = Math.max(pos1.getColumn(), pos2.getColumn());
		
		_min = new Position(minRow, minColumn);
		_max = new Position(maxRow, maxColumn);
	}

	/** @return first position of the interval */
	public Position getMinPosition() {
		return _min;
	}

	/** @return last position of the interval */
	public Position getMaxPosition() {
		return _max;
	}

	/**
	 * @return true if the interval is a straight line; false
	 * otherwise
	 */
	public boolean isLinear() {
		return ((_min.getRow() == _max.getRow()) ||
		       (_min.getColumn() == _max.getColumn()));
	}

	/** @return iterator over all positions in the interval */
	@Override
	public Iterator<Position> iterator() {
		return new Iterator<Position>() {
			@Override
			public boolean hasNext() {
				return !_current.equals(_max);
			}

			@Override
			public Position next() {
				if (_current.getColumn() == _max.getColumn()) {
					_current.setColumn(_min.getColumn());
					_current.setRow(_current.getRow() + 1);
				} else {
					_current.setColumn(_current.getColumn() + 1);
				}
				return _current;
			}

			private Position _current = new Position(_min.getRow(),
							_min.getColumn() - 1);
		};
	}

	/** @See xxl.cells.renderers.Renderable#render */
	@Override
	public String render(Renderer renderer) {
		return renderer.renderInterval(this);
	}

	@Override
	public String toString() {
		return _min.toString() + ":" + _max.toString();
	}

	/**
	 * Offsets the current interval so it starts at the new position
	 * while mantaining it's size and shape.
	 *
	 * @param position new start position.
	 * @return new offset Interval.
	 */
	public Interval offset(Position position) {
		int rowOffset = position.getRow() - _min.getRow();
		int columnOffset = position.getColumn() - _min.getColumn();
		int maxRow = _max.getRow() + rowOffset;
		int maxColumn = _max.getColumn() + columnOffset;
		return new Interval(position, new Position(maxRow, maxColumn));
	}

	/**
	 * Executes the same operation as the other Interval#offset
	 * function, but only when interval is a position, otherwise
	 * just returns this object.
	 *
	 * @param interval where to offset to.
	 * @return this if interval is not a single position;
	 * new Interval starting at interval otherwise.
	 */
	public Interval offset(Interval interval) {
		if (!interval.isPosition())
			return interval;
		return offset(interval.getMinPosition());
	}

	/**
	 * @return true if the minimum position is equal
	 * to the maximum position; false otherwise.
	 */
	public boolean isPosition() {
		return _min.equals(_max);
	}

	@Override
	public boolean equals(Object obj) {
		if (obj == null)
			return false;
		if (obj instanceof Interval interval) {
			Interval offset = interval.offset(_min);
			return _min.equals(offset._min) && _max.equals(offset._max);
		}
		return false;
	}

	private Position _min;
	private Position _max;
}
