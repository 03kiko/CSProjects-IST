package xxl.storage;

import java.io.Serializable;
import xxl.cells.Position;
import xxl.cells.Interval;
import xxl.cells.Cell;
import xxl.cells.Content;

/**
 * This class represents where Cells are stored
 */
public abstract class CellStorage implements Serializable {
	/**
	 * @param rows how many rows the storage has
	 * @param columns how many columns the storage has
	 */
	public CellStorage(int rows, int columns) {
		_rows = rows;
		_columns = columns;
	}

	/**
	 * Gets a cell from the storage only if it exists
	 *
	 * @param position where to get the cell from
	 * @return cell at the position (null if does not exist)
	 */
	abstract public Cell view(Position position);

	/**
	 * Gets a cell from the storage
	 *
	 * @param position where to get the cell from
	 * @return cell at the position
	 */
	abstract public Cell get(Position position);

	/**
	 * Inserts new content at the position (may replace old content)
	 * 
	 * @param position where to insert
	 * @param content what to insert
	 */
	abstract public void insert(Position position, Content content);

	/**
	 * Deletes the content at the position
	 *
	 * @param position where to delete
	 */
	abstract public void delete(Position position);

	/**
	 * Deletes a cell if it is no longer in use
	 *
	 * @param position location of the cell
	 * @param cell to be deleted
	 */
	abstract public void deleteIfFree(Position position, Cell cell);

	/**
	 * Clears the storage.
	 */
	abstract public void clear();

	/** @return An interval representing the entire spreadsheet */
	public Interval getInterval() {
		return new Interval(new Position(0,0),
					    new Position(_rows -1, _columns -1));
	}

	/**
	 * Checks if the position is within the bounds of the storage
	 *
	 * @param position to check
	 * @return true if in bounds; false otherwise
	 */
	public boolean inBounds(Position pos) {
		boolean resultRow = (pos.getRow() > -1) && (pos.getRow() < _rows);
		boolean resultColumn = (pos.getColumn() > -1) && (pos.getColumn() < _columns);
		return (resultRow && resultColumn);
	}

	/**
	 * Checks if the interval is within the bounds of the storage
	 *
	 * @param interval to check
	 * @return true if in bounds; false otherwise
	 */
	public boolean inBounds(Interval interval) {
		return inBounds(interval.getMinPosition()) &&
			   inBounds(interval.getMaxPosition());
	}

	/**
	 * Returns the size of this storage as a position
	 * like (max Row, max Column).
	 *
	 * @return the position containing the size.
	 */
	public Position getSpan() {
		return new Position(_rows, _columns);
	}

	private int _rows;
	private int _columns;
}
