package xxl.cells;

import java.io.Serializable;
import xxl.cells.renderers.Renderer;
import xxl.cells.renderers.Renderable;

/**
 * This class represents a two dimensional integer position.
 */
public class Position implements Serializable, Renderable {
	/**
	 * @param row row of the position
	 * @param column column of the position
	 */
	public Position(int row, int column) {
		_row = row;
		_column = column;
	}
	
	/** @return row of the position */
	public int getRow() {
		return _row;
	}
	
	/** @return column of the position */
	public int getColumn() {
		return _column;
	}
	
	/** @param row value to set the row to */
	public void setRow(int row) {
		_row = row;
	}
	
	/** @param column value to set the column to */
	public void setColumn(int column) {
		_column = column;
	}
	
	/** @return true if rows and columns are equal; false otherwise */
	@Override
	public boolean equals(Object obj) {
		if (obj == null)
			return false;
		if (obj instanceof Position pos) {
			boolean equalRows = (_row == pos.getRow());
			boolean equalColumns = (_column == pos.getColumn());
			return (equalRows && equalColumns);
		}
		
		return false;
	}

	/**
	 * Renders this position in form of a string.
	 *
	 * @param renderer how to render.
	 * @return the rendered position.
	 */
	@Override
	public String render(Renderer renderer) {
		return renderer.renderPosition(this);
	}

	@Override
	public String toString() {
		return (_row + 1) + ";" + (_column + 1);
	}

	private int _row;
	private int _column;
}
