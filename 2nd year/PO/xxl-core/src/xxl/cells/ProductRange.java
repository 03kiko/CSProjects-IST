package xxl.cells;

import xxl.exceptions.InvalidResultException;
import xxl.exceptions.RangeException;
import xxl.storage.CellStorage;

public class ProductRange extends IntegerRange {
	/** @See IntegerRange#IntegerRange */
	public ProductRange(Interval interval, CellStorage parent) throws RangeException {
		super(interval, parent, "PRODUCT");
	}

	/** @See ProductRange#ProductRange */
	protected ProductRange(ProductRange range, CellStorage dest) {
		super(range, dest);
	}

	/** @See Range#execute */
	@Override
	protected Integer execute() throws InvalidResultException {
		int result = 1;
		for(Position pos: getInterval()) {
			Cell cell = getParent().get(pos);
			Integer current = cell.getInteger();
			if(current == null)
				throw new InvalidResultException();
			result *= current;
		}
		return result;
	}

	/** @See Range#copyRange */
	@Override
	protected ProductRange copyRange(CellStorage dest) {
		return new ProductRange(this, dest);
	}
}
	
