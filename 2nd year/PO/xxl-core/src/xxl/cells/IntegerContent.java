package xxl.cells;

import xxl.cells.renderers.Renderer;
import xxl.cells.search.Search;
import xxl.storage.CellStorage;

/**
 * This class represents an Integer literal.
 */
public class IntegerContent extends Content<Integer> {
	/** @param integer integer literal to store */
	public IntegerContent(int integer) {
		super(integer);
	}
	
	/** @return the stored integer */
	@Override
	public Integer getInteger() {
		return getContent();
	}

	/** @See xxl.cells.renderers.Renderable#render */
	@Override
	public String render(Renderer renderer) {
		return renderer.renderIntegerContent(this);
	}

	/** @See xxl.cells.search.Searchable#ok */
	@Override
	public boolean ok(Search search) {
		return search.okIntegerContent(this);
	}

	/** @See Content#copyTo */
	@Override
	public Content copyTo(CellStorage dest, Position pos, boolean dependencies) {
		Content copied = new IntegerContent(getContent());
		dest.insert(pos, copied);
		return copied;
	}
}
