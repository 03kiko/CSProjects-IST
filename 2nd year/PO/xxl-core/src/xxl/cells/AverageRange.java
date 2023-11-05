package xxl.cells;

import xxl.exceptions.InvalidResultException;
import xxl.exceptions.RangeException;
import xxl.storage.CellStorage;

public class AverageRange extends IntegerRange { 
	/** @See IntegerRange#IntegerRange */
	public AverageRange(Interval interval, CellStorage parent) throws RangeException {
		super(interval, parent, "AVERAGE");
	}

	/** @See IntegerRange#IntegerRange */
	protected AverageRange(AverageRange range, CellStorage dest) {
		super(range, dest);
	}

	/** @See Range#execute */
	@Override
	protected Integer execute() throws InvalidResultException {
		int count = 0;
		int sum = 0;
		for (Position pos : getInterval()) {
			Cell cell = getParent().get(pos);
			Integer current = cell.getInteger();
			if (current == null)
				throw new InvalidResultException();
			sum += current;
			count++;
		}
		return sum / count;
	}

	/** @See Range#copyRange */
	@Override
	public AverageRange copyRange(CellStorage dest) {
		return new AverageRange(this, dest);
	}
}
