package xxl.cells;

import xxl.storage.CellStorage;
import xxl.exceptions.RangeException;
import xxl.exceptions.RangeOutOfBoundsException;
import xxl.exceptions.InvalidResultException;

/**
 * This class represents the contents of a cell that
 * executes an operation over an interval of cells
 * and returns a certain output.
 *
 * @param <T> expected type of the output
 */
public abstract class Range<T> extends Content<T> {
	/**
	 * @param interval interval of cells to work on
	 * @param parent storage to get the cells from
	 * @param name of the function being executed
	 */
	public Range(Interval interval, CellStorage parent, String name) throws RangeException {
		_interval = interval;
		_parent = parent;
		_name = name;
		if (!_parent.inBounds(interval))
			throw new RangeOutOfBoundsException(interval.toString());
	}

	/**
	 * Creates a copy from the given range.
	 *
	 * @param range to copy.
	 * @param dest associate the new range to another storage.
	 */
	protected Range(Range<T> range, CellStorage dest) {
		super(range.getContent());
		_interval = range._interval;
		_parent = dest;
		_name = range._name;
	}

	/** @return result of the operation */
	abstract protected T execute() throws InvalidResultException;

	/** @See Content#update */
	@Override
	public void update() { _updated = true; }

	/** Signals the cell has updated. */
	protected void done() { _updated = false; }
	
	/** @See Content#initialize */
	@Override
	public void initialize(Cell cell) {
		for (Position position : _interval)
			_parent.get(position).addObserver(cell);
	}

	/** @See Content#destroy */
	@Override
	public void destroy(Cell cell) {
		for (Position position : _interval) {
			Cell current = _parent.get(position);
			current.removeObserver(cell);
			_parent.deleteIfFree(position, current); 
		}
	}

	/** @See Content#CopyTo */
	@Override
	public Content copyTo(CellStorage dest, Position pos, boolean dependencies) {
		if (dependencies)
			for (Position position : _interval)
				_parent.get(position).copyTo(dest, position, dependencies);
		Content copied = copyRange(dest);
		dest.insert(pos, copied);
		return copied;
	}

	/** Simple template method to avoid repetition. */
	abstract protected Range<?> copyRange(CellStorage dest);


	/** @return the interval of this object. */
	public Interval getInterval() { return _interval; }

	/** @return the name of the function. */
	public String getName() { return _name; }

	/** @return the associated CellStorage */
	protected CellStorage getParent() { return _parent; }

	/** @return true if the result should be updated;
	 * false otherwise.
	 */
	protected boolean shouldExecute() { return _updated; }

	private boolean _updated = false;
	private Interval _interval;
	private CellStorage _parent;
	private String _name;
}
