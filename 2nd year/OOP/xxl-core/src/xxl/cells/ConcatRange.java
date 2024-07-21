package xxl.cells;

import xxl.exceptions.InvalidResultException;
import xxl.exceptions.RangeException;
import xxl.storage.CellStorage;

public class ConcatRange extends StringRange { 
	/** @See StringRange#StringRange */
	public ConcatRange(Interval interval, CellStorage parent) throws RangeException {
		super(interval, parent, "CONCAT");
	}

	/** @See StringRange#StringRange */
	protected ConcatRange(ConcatRange range, CellStorage dest) {
		super(range, dest);
	}

	/** @See Range#execute */
	@Override
	public String execute() throws InvalidResultException {
		String result = "";
		for (Position pos : getInterval()) {
			Cell cell = getParent().get(pos);
			String current = cell.getString();
			if (current != null)
				result += current;
		}
		return result;
	}

	/** @See Range#copyRange */
	@Override
	public ConcatRange copyRange(CellStorage dest) {
		return new ConcatRange(this, dest);
	}
}
