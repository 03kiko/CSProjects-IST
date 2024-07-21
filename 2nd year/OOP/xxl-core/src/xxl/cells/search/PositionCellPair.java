package xxl.cells.search;

import xxl.cells.Position;
import xxl.cells.Cell;

public class PositionCellPair {
	private Position _position;
	private Cell _cell;

	public PositionCellPair(Position pos, Cell cell) {
		_position = pos;
		_cell = cell;
	}

	public Cell getCell() { return _cell; }

	public Position getPosition() { return _position; }
}
