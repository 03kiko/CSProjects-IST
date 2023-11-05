package xxl.cells;

import xxl.storage.CellStorage;
import xxl.exceptions.RangeException;
import xxl.exceptions.InvalidResultException;
import xxl.cells.renderers.Renderer;
import xxl.cells.search.Search;

/**
 * This class represents the content of a cell that executes
 * over an interval of cells and returns a String.
 */
public abstract class StringRange extends Range<String> {
	/**
	 * @param interval cells to execute on
	 * @param parent spreadsheet to get the cells from
	 * @param name of the operation
	 * @throws NonLinearRangeException when the interval is not a
	 * straight line
	 */
	public StringRange(Interval interval, CellStorage parent, String name) throws RangeException {
		super(interval, parent, name);
	}

	/** @See Range#Range for more information. */
	protected StringRange(Range<String> range, CellStorage dest) {
		super(range, dest);
	}

	/**
	 * @return resulting value of the operation over the interval
	 */
	@Override
	public String getString() {
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
		return renderer.renderStringRange(this);
	}
	
	/** @See xxl.cells.search.Searchable#ok */
	public boolean ok(Search search) {
		return search.okStringRange(this);
	}
}
