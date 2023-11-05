package xxl.cells;

import java.io.Serializable;
import xxl.cells.renderers.Renderable;
import xxl.cells.search.Searchable;
import xxl.storage.CellStorage;

/**
 * This class represents the content a Cell can have.
 * It provides a default implementation for all possible getters.
 * By default, a this generic content always returns null to every
 * query, so every child class should implement the getters related
 * to it's content type.
 *
 * @param <T> type of the value the single holds
 */
public abstract class Content<T> implements Serializable, CellObserver, Renderable, Searchable {
	/**
	 * @param content what to store here.
	 */
	public Content(T content) {
		_content = content;
	}

	/**
	 * Sets the content to null, used by
	 * child classes that may not pre-calculate their value.
	 */
	protected Content() { _content = null; }

	/** Default implementation of the Integer query */
	public Integer getInteger() { return null; }

	/** Default implementation of the String query */
	public String getString() { return null; }

	/**
	 * Executes a "start-up" process when the content
	 * is put in a cell.
	 * Does nothing by default.
	 */
	 public void initialize(Cell cell) {}

	/**
	 * Executes a "clean-up" process before the content
	 * is exchanged for another in a cell.
	 * Does nothing by default.
	 */
	 public void destroy(Cell cell) {}

	 /**
	  * Signals the content it must update.
	  * Does nothing by default.
	  */
	 public void update() {}

	/**
	 * Copies this content to new cells in the destination storage.
	 *
	 * @param dest storage to copy to.
	 * @param pos position to copy to.
	 * @param dependencies should this contents dependencies be copied?
	 *
	 * @return a reference to the copied content.
	 */
	abstract public Content copyTo(CellStorage dest, Position pos, boolean dependencies);

	/** @return the actual content */
	protected T getContent() {
		return _content;
	}

	/**
	 * Sets the content.
	 *
	 * @param content the content to set to
	 */
	protected void setContent(T content) {
		_content = content;
	}
	
	/**
	 * The stored content.
	 */
	private T _content;
}
