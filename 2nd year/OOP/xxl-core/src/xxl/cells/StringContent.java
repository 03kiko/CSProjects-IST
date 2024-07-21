package xxl.cells;

import xxl.cells.renderers.Renderer;
import xxl.cells.search.Search;
import xxl.storage.CellStorage;

/**
 * This class represents a String literal.
 */
public class StringContent extends Content<String> {
	/**
	 * @param string string literal to store
	 */
	public StringContent(String string) {
		super(string);
	}
	
	/** @return the stored string literal */
	@Override
	public String getString() {
		return getContent();
	}

	/** @See xxl.cells.renderers.Renderable#render */
	@Override
	public String render(Renderer renderer) {
		return renderer.renderStringContent(this);
	}
	
	/** @See xxl.cells.search.Searchable#ok */
	@Override
	public boolean ok(Search search) {
		return search.okStringContent(this);
	}

	/** @See Content#copyTo */
	@Override
	public Content copyTo(CellStorage dest, Position pos, boolean dependencies) {
		Content copied = new StringContent(getContent());
		dest.insert(pos, copied);
		return copied;
	}
}
