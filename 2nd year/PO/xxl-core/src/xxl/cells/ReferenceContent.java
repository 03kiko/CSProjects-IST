package xxl.cells;

import xxl.storage.CellStorage;
import xxl.exceptions.RangeException;
import xxl.exceptions.RangeOutOfBoundsException;
import xxl.cells.renderers.Renderer;
import xxl.cells.search.Search;

/**
 * This class represents a pointer to another cell.
 */
public class ReferenceContent extends Content<CellStorage> implements CellObserver {
	/**
	 * @param reference to copy from.
	 * @param dest storage to get the cell from.
	 */
	protected ReferenceContent(ReferenceContent reference, CellStorage dest) {
		super(dest);
		_position = reference._position;
	}

	/**
	 * @param position location of the referenced cell
	 * @param storage where to get the referenced cell
	 *
	 * @throws RangeException when the referenced position is out of bounds
	 */
	public ReferenceContent(Position position, CellStorage storage) throws RangeException {
		super(storage);
		if (!getContent().inBounds(position))
			throw new RangeOutOfBoundsException(position.toString());
		_position = position;
	}
		
	
	/** @return integer content of the reference (may be null) */
	@Override
	public Integer getInteger() {
		return getReferenced().getInteger();
	}

	/** @return string content of the reference (may be null) */
	@Override
	public String getString() {
		return getReferenced().getString();
	}

	/** @return the referenced cell */
	public Cell getReferenced() {
		return getContent().get(_position);
	}

	/** @return the position this content references */
	public Position getPosition() {
		return _position;
	}

	/** @See xxl.cells.renderers.Renderable#render */
	@Override
	public String render(Renderer renderer) {
		return renderer.renderReferenceContent(this);
	}

	/** @See xxl.cells.search.Search#ok */
	@Override
	public boolean ok(Search search) {
		return search.okReferenceContent(this);
	}

	/** @See Content#copyTo */
	@Override
	public Content copyTo(CellStorage dest, Position pos, boolean dependencies) {
		if (dependencies)
			getReferenced().copyTo(dest, _position, dependencies);
		Content copied = new ReferenceContent(this, dest);
		dest.insert(pos, copied);
		return copied;
	}

	/** @See Content#initialize */
	@Override
	public void initialize(Cell cell) {
		getReferenced().addObserver(cell);
	}

	/** @See Content#destroy */
	@Override
	public void destroy(Cell cell) {
		Cell referenced = getReferenced();
		referenced.removeObserver(cell);
		getContent().deleteIfFree(_position, referenced);
	}

	private Position _position;
}
