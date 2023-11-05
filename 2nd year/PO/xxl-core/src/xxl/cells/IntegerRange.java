package xxl.cells;

import xxl.storage.CellStorage;
import xxl.exceptions.RangeException;
import xxl.exceptions.InvalidResultException;
import xxl.cells.renderers.Renderer;
import xxl.cells.search.Search;

/**
 * This class represents the content of a cell that executes
 * over an interval of cells and returns an Integer.
 */
public abstract class IntegerRange extends Range<Integer> {
	/**
	 * @param interval cells to execute on
	 * @param parent spreadsheet to get the cells from
	 * @param name of the function
	 * @throws NonLinearRangeException when the interval is not a
	 * straight line
	 */
	public IntegerRange(Interval interval, CellStorage parent, String name) throws RangeException {
		super(interval, parent, name);
	}

	/** @See Range#Range for more information. */
	protected IntegerRange(Range<Integer> range, CellStorage dest) {
		super(range, dest);
	}

	/**
	 * @return the result of the operation over the interval
	 */
	@Override
	public Integer getInteger() {
		try {
			if (shouldExecute()) {
				done();
				setContent(execute());
			}
		} catch (InvalidResultException e) {
			setContent(null);
		}
		return getContent();
	}

	/** @See xxl.cells.renderers.Renderable#render */
	@Override
	public String render(Renderer renderer) {
		return renderer.renderIntegerRange(this);
	}
	
	/** @See xxl.cells.search.Searchable#ok */
	@Override
	public boolean ok(Search search) {
		return search.okIntegerRange(this);
	}
}
