package xxl.cutBuffer;

import java.io.Serializable;
import java.util.Iterator;

import xxl.cells.Interval;
import xxl.cells.Position;
import xxl.cells.Cell;
import xxl.cells.renderers.Renderer;
import xxl.storage.CellStorage;
import xxl.storage.TreeStorage;

public class CutBuffer implements Serializable {
	/**
	 * @param storage establishes the maximum bounds of this buffer.
	 */
	public CutBuffer(CellStorage storage) {
		Position maxPosition = storage.getSpan();
		int rows = maxPosition.getRow();
		int columns = maxPosition.getColumn();
		_storage = new TreeStorage(rows, columns);
	}

	/**
	 * Copies all cells from the interval into this buffer.
	 *
	 * @param interval where to copy from.
	 * @param storage source (where to get the cells from).
	 */
	public void copy(Interval interval, CellStorage storage) {
		_storage.clear();
		_interval = interval;
		for (Position pos : interval) {
			Cell cell = storage.view(pos);
			if (cell != null)
				cell.copyTo(_storage, pos, true);
		}
	}

	/**
	 * Pastes all cells from this buffer into the given storage.
	 *
	 * @param interval where to copy to.
	 * @param storage destination (where to copy into).
	 */
	public void paste(Interval interval, CellStorage storage) {
		if (isEmpty())
			return;
		if (!interval.isPosition() && !_interval.equals(interval))
			return;
		Interval destination = _interval.offset(interval);
		Iterator<Position> source = _interval.iterator();
		for (Position pos : destination) {
			if (!storage.inBounds(pos))
				break;
			Position getFrom = source.next();
			Cell cell = _storage.view(getFrom);
			if (cell != null)
				_storage.get(getFrom).copyTo(storage, pos, false);
			else
				storage.delete(pos);
		}
	}

	/**
	 * Shows all the contents of the buffer.
	 *
	 * @param renderer how to render the contents.
	 * @return string displaying the contents.
	 */
	public String view(Renderer renderer) {
		String result = "";
		boolean first = true;
		if (isEmpty())
			return result;
		Interval interval = _interval.offset(new Position(0, 0));
		Iterator<Position> displayedPos = interval.iterator();
		for (Position pos : _interval) {
			if (!first)
				result += "\n";
			else
				first = false;
			result += renderer.combine(displayedPos.next(), _storage.view(pos));
		}
		return result;
	}

	/** @return true if this buffer has not been used; false otherwise. */
	private boolean isEmpty() {
		return _interval == null;
	}

	private Interval _interval = null;
	private CellStorage _storage;
}
