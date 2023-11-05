package xxl.storage;

import java.util.TreeMap;
import java.util.Map;
import xxl.cells.Cell;
import xxl.cells.Position;
import xxl.cells.Content;

/**
 * This class stores Cells in a TreeMap of TreeMaps
 */
public class TreeStorage extends CellStorage {
	/**
	 * @param rows how many rows the storage has
	 * @param columns how many columns the storage has
	 */
	public TreeStorage(int rows, int columns) { super(rows, columns); }

	/** @See CellStorage#view */
	@Override
	public Cell view(Position position) {
		var row = _storage.get(position.getRow());
		if (row == null)
			return null;
		return row.get(position.getColumn());
	}

	/** @See CellStorage#get */
	@Override
	public Cell get(Position position) {
		var row = getRow(position.getRow());
		Cell cell = row.get(position.getColumn());
		if (cell != null)
			return cell;
		cell = new Cell();
		row.put(position.getColumn(), cell);
		return cell;
	}

	/** @See CellStorage#insert */
	@Override
	public void insert(Position position, Content content) {
		Cell cell = get(position);
		cell.insert(content);
	}

	/** @See CellStorage#delete */
	@Override
	public void delete(Position position) {
		var row = getRow(position.getRow());
		Cell cell = row.get(position.getColumn());
		if (cell == null) return;
		cell.delete();
		deleteIfFree(position, cell);
	}

	/** @See CellStorage#deleteIfFree */
	@Override
	public void deleteIfFree(Position position, Cell cell) {
		if (!cell.deletable())
			return;
		var row = getRow(position.getRow());
		row.remove(position.getColumn());
		if (row.isEmpty())
			_storage.remove(position.getRow());
	}

	/** @See CellStorage#clear */
	@Override
	public void clear() { _storage.clear(); }

	/**
	 * Gets the Map of the given row (creates if not yet instantiated)
	 *
	 * @param row to get
	 * @return the Map in associated to that row
	 */
	private Map<Integer, Cell> getRow(int row) {
		Map<Integer, Cell> rowMap = _storage.get(row);
		if (rowMap != null) return rowMap;
		rowMap = new TreeMap<>();
		_storage.put(row, rowMap);
		return rowMap;
	}

	private Map<Integer, Map<Integer, Cell>> _storage = new TreeMap<>();
}
