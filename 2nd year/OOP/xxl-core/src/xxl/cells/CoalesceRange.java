package xxl.cells;

import xxl.exceptions.InvalidResultException;
import xxl.exceptions.RangeException;
import xxl.storage.CellStorage;

public class CoalesceRange extends StringRange { 
	/** @See StringRange#StringRange */
	public CoalesceRange(Interval interval, CellStorage parent) throws RangeException {
		super(interval, parent, "COALESCE");
	}

	/** @See StringRange#StringRange */
	protected CoalesceRange(CoalesceRange range, CellStorage dest) {
		super(range, dest);
	}

	/** @See Range#execute */
	@Override
	public String execute() throws InvalidResultException {
		for (Position pos : getInterval()) {
			Cell cell = getParent().get(pos);
			String current = cell.getString();
			if (current != null)
				return current;
		}
		return "";
	}

	/** @See Range#copyRange */
	@Override
	public CoalesceRange copyRange(CellStorage dest) {
		return new CoalesceRange(this, dest);
	}
}
